#include "mainwindow.h"
#include "capturethread.h"
#include "databasemanager.h"
#include "displaythread.h"
#include "driverlistdialog.h"
#include "faceengine.h"
#include "recognitionthread.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QDir>
#include <QFileInfo>
#include <QCloseEvent>
#include <QImage>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPixmap>
#include <opencv2/imgcodecs.hpp>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSqlQueryModel>
#include <QStatusBar>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_databaseManager(new DatabaseManager)
{
    ui->setupUi(this);
    const QDate today = QDate::currentDate();
    ui->startDateTimeEdit->setDateTime(QDateTime(today.addDays(-1), QTime(0, 0)));
    ui->endDateTimeEdit->setDateTime(QDateTime(today, QTime(23, 59, 59)));
    ui->resultTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QString error;
    if (!m_databaseManager->openDatabase(&error) || !m_databaseManager->initializeSchema(&error)) {
        QMessageBox::critical(this, tr("DB 연결 오류"), error);
        ui->databaseTab->setEnabled(false);
        return;
    }
    statusBar()->showMessage(tr("DB 연결됨: %1").arg(m_databaseManager->databasePath()));
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::queryDatabase);

    m_databaseReady = true;
    if (!m_databaseManager->insertSystemEvent(QStringLiteral("SYSTEM_START_REQUEST"), &error)) {
        showDatabaseError(tr("시작 이벤트 저장"), error);
        return;
    }

    m_sensorTimer = new QTimer(this);
    connect(m_sensorTimer, &QTimer::timeout, this, &MainWindow::generateSensorSnapshot);
    m_sensorTimer->start(1000);

    m_vehicleTimer = new QTimer(this);
    connect(m_vehicleTimer, &QTimer::timeout, this, &MainWindow::generateVehicleState);
    m_vehicleTimer->start(2000);

    generateSensorSnapshot();
    generateVehicleState();
    queryDatabase();

    setupCameras();
    setupFaceRecognition();

    // 시나리오: 탑승 -> 운전자 인식 -> 인증 완료 -> 계기판
    // 시작 화면은 운전자 인식 탭이다. ('인식 시작' 버튼이 문 열림을 대신한다)
    goToPage(ui->pageDriver);
    ui->lblRecogResult->setText(tr("인식 시작을 눌러 운전자 인증을 진행하세요"));
    setDrivingEnabled(false);   // 인증 전에는 주행 조작 불가
}

MainWindow::~MainWindow()
{
    // 소비자(표시) 먼저, 생산자(캡처) 나중에
    if (m_recognizer)  m_recognizer->stop();
    if (m_userView)    m_userView->stop();
    if (m_rearView)    m_rearView->stop();
    if (m_userCapture) m_userCapture->stop();
    if (m_rearCapture) m_rearCapture->stop();

    delete m_queryModel;
    delete m_faceEngine;
    delete m_databaseManager;
    delete ui;
}

void MainWindow::setupCameras()
{
    // ---- 사용자(내장) 카메라 : 운전자 인식 탭 ----
    m_userCapture = new CaptureThread(kUserCamSrc, 640, 480, this);
    m_userView = new DisplayThread(m_userCapture, 30, /*mirror=*/true, this);
    connect(m_userCapture, &CaptureThread::opened, this, &MainWindow::onCameraOpened);
    connect(m_userView, &DisplayThread::sendImage, this,
            [this](const QImage &image) { showImage(ui->lblCamInternal, image); });

    // ---- 후방(USB) 카메라 : 후방 카메라 탭 ----
    m_rearCapture = new CaptureThread(kRearCamSrc, 640, 480, this);
    m_rearView = new DisplayThread(m_rearCapture, 30, /*mirror=*/true, this);
    connect(m_rearCapture, &CaptureThread::opened, this, &MainWindow::onCameraOpened);
    connect(m_rearView, &DisplayThread::sendImage, this,
            [this](const QImage &image) { showImage(ui->lblCamRear, image); });

    m_userCapture->start();
    m_userView->start();
    m_rearCapture->start();
    m_rearView->start();

    ui->btnCamRearToggle->setChecked(true);
    ui->btnCamRearToggle->setText(tr("후방 카메라 끄기"));
    connect(ui->btnCamRearToggle, &QPushButton::toggled, this, &MainWindow::toggleRearCamera);
}

void MainWindow::showImage(QLabel *label, const QImage &image)
{
    if (image.isNull())
        return;
    // 고정 크기로 스케일한다. label->size() 를 기준으로 삼으면
    // 픽스맵이 커질 때마다 라벨 sizeHint 도 커져 화면이 계속 확대된다.
    label->setPixmap(QPixmap::fromImage(image).scaled(m_videoSize, Qt::KeepAspectRatio,
                                                      Qt::SmoothTransformation));
}

void MainWindow::onCameraOpened(bool ok, const QString &message)
{
    auto *capture = qobject_cast<CaptureThread *>(sender());
    const bool isRear = capture && capture->source() == kRearCamSrc;

    if (ok) {
        logEvent(isRear ? QStringLiteral("REAR_CAMERA_ON") : QStringLiteral("USER_CAMERA_ON"));
    } else {
        (isRear ? ui->lblCamRear : ui->lblCamInternal)->setText(message);
        logEvent(QStringLiteral("CAMERA_ERROR"));
    }
    statusBar()->showMessage(message, 5000);
}

void MainWindow::toggleRearCamera(bool checked)
{
    m_rearView->setPaused(!checked);
    ui->btnCamRearToggle->setText(checked ? tr("후방 카메라 끄기") : tr("후방 카메라 켜기"));
    if (!checked)
        ui->lblCamRear->setText(tr("후방 카메라 꺼짐"));
    logEvent(checked ? QStringLiteral("REAR_CAMERA_ON") : QStringLiteral("REAR_CAMERA_OFF"));
}

void MainWindow::logEvent(const QString &eventType)
{
    if (!m_databaseReady)
        return;
    QString error;
    if (!m_databaseManager->insertSystemEvent(eventType, &error))
        showDatabaseError(tr("%1 이벤트 저장").arg(eventType), error);
    else
        refreshDatabaseView();
}

// ---------------- 얼굴 인식 ----------------

void MainWindow::setupFaceRecognition()
{
    m_faceEngine = new FaceEngine;
    QString error;
    if (!m_faceEngine->initialize(
            QFileInfo(m_databaseManager->databasePath()).absolutePath() + "/lbph_model.yml",
            &error)) {
        QMessageBox::warning(this, tr("얼굴 인식 초기화 실패"), error);
        ui->grpDriverManage->setEnabled(false);
        return;
    }
    m_faceEngine->setDriverNames(m_databaseManager->driverNames());
    m_faceEngine->train(*m_databaseManager);
    m_faceReady = true;

    m_recognizer = new RecognitionThread(m_userCapture, m_faceEngine, 15, this);
    connect(m_recognizer, &RecognitionThread::sendImage, this,
            [this](const QImage &image) { showImage(ui->lblCamInternal, image); });
    connect(m_recognizer, &RecognitionThread::sendStatus, this,
            [this](const QString &text) { ui->lblRecogResult->setText(tr("인식 결과: %1").arg(text)); });
    connect(m_recognizer, &RecognitionThread::authConfirmed, this, &MainWindow::onAuthConfirmed);
    connect(m_recognizer, &RecognitionThread::sampleCaptured, this, &MainWindow::onSampleCaptured);
    connect(m_recognizer, &RecognitionThread::registerFinished, this, &MainWindow::onRegisterFinished);
    m_recognizer->start();

    connect(ui->btnRecogToggle, &QPushButton::toggled, this, &MainWindow::toggleRecognition);
    connect(ui->btnRegisterFace, &QPushButton::clicked, this, &MainWindow::startRegistration);
    connect(ui->btnDriverList, &QPushButton::clicked, this, &MainWindow::showDriverList);

    statusBar()->showMessage(tr("등록 운전자 %1명 / 얼굴 모델 학습됨: %2")
                                 .arg(m_databaseManager->listDrivers().size())
                                 .arg(m_faceEngine->isTrained() ? tr("예") : tr("아니오")), 5000);
}

void MainWindow::toggleRecognition(bool checked)
{
    if (!m_faceReady)
        return;

    if (checked) {
        if (!m_faceEngine->isTrained()) {
            QMessageBox::information(this, tr("안내"),
                                     tr("등록된 얼굴이 없습니다. 먼저 얼굴을 등록하세요."));
            ui->btnRecogToggle->setChecked(false);
            return;
        }
        m_userView->setPaused(true);        // 표시 스레드와 화면 충돌 방지
        m_recognizer->startRecognize();
        ui->btnRecogToggle->setText(tr("인식 중지"));
        logEvent(QStringLiteral("FACE_DETECTION_START"));
    } else {
        m_recognizer->setIdle();
        m_userView->setPaused(false);
        ui->lblRecogResult->setText(tr("인식 결과: -"));
        ui->btnRecogToggle->setText(tr("인식 시작"));
    }
}

void MainWindow::onAuthConfirmed(int driverId, double confidence)
{
    QString error;
    if (!m_databaseManager->insertAuthLog(driverId, confidence, &error)) {
        showDatabaseError(tr("인증 로그 저장"), error);
        return;
    }
    logEvent(QStringLiteral("FACE_DETECTED"));

    const QString name = m_databaseManager->driverName(driverId);
    ui->lblAuth->setText(tr("인증: %1").arg(name));
    statusBar()->showMessage(tr("%1 인증 완료 (score %2)").arg(name).arg(confidence, 0, 'f', 0), 5000);

    // 인증이 끝나면 인식을 멈추고 계기판으로 넘어간다.
    // setChecked(false) 가 toggleRecognition() 을 호출해 표시 스레드도 되살린다.
    ui->btnRecogToggle->setChecked(false);
    ui->lblRecogResult->setText(tr("%1 님 인증 완료").arg(name));
    setDrivingEnabled(true);
    goToPage(ui->pageDashboard);
}

void MainWindow::startRegistration()
{
    if (!m_faceReady)
        return;

    const QString name = ui->editDriverName->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, tr("안내"), tr("운전자 이름을 입력하세요."));
        return;
    }
    if (m_recognizer->mode() == RecognitionThread::Register)
        return;

    QString error;
    m_registerDriverId = m_databaseManager->addDriver(name, &error);
    if (m_registerDriverId < 0) {
        QMessageBox::critical(this, tr("등록 실패"), error);
        return;
    }
    m_registerName = name;

    ui->btnRecogToggle->setChecked(false);
    ui->btnRecogToggle->setEnabled(false);
    m_userView->setPaused(true);
    m_recognizer->startRegister(kSampleTarget);
    ui->lblRecogResult->setText(tr("정면을 봐주세요 (0/%1)").arg(kSampleTarget));
}

void MainWindow::onSampleCaptured(const cv::Mat &grayFace, int count, int target)
{
    // DB 쓰기와 파일 저장은 UI 스레드에서 처리한다
    const QString dir = QStringLiteral("%1/%2").arg(m_databaseManager->faceDirectory())
                            .arg(m_registerDriverId);
    QDir().mkpath(dir);
    const QString path = QStringLiteral("%1/%2.png").arg(dir)
                             .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_zzz"));

    if (!cv::imwrite(path.toStdString(), grayFace)) {
        showDatabaseError(tr("얼굴 샘플 저장"), tr("이미지 파일 저장 실패: %1").arg(path));
        return;
    }
    QString error;
    if (!m_databaseManager->insertFaceSample(m_registerDriverId, path, &error)) {
        showDatabaseError(tr("얼굴 샘플 저장"), error);
        return;
    }
    ui->lblRecogResult->setText(tr("%1 등록 중 %2/%3").arg(m_registerName).arg(count).arg(target));
}

void MainWindow::onRegisterFinished(int count)
{
    ui->btnRecogToggle->setEnabled(true);
    m_userView->setPaused(false);
    retrainFaceModel();
    logEvent(QStringLiteral("DRIVER_REGISTERED"));
    ui->editDriverName->clear();
    ui->lblRecogResult->setText(tr("인식 결과: -"));
    QMessageBox::information(this, tr("등록 완료"),
                             tr("'%1' 얼굴 샘플 %2장을 등록했습니다.").arg(m_registerName).arg(count));
}

void MainWindow::showDriverList()
{
    DriverListDialog dialog(m_databaseManager, this);
    connect(&dialog, &DriverListDialog::driverDeleted, this, &MainWindow::retrainFaceModel);
    dialog.exec();
}

void MainWindow::goToPage(QWidget *page)
{
    const int index = ui->stackMain->indexOf(page);
    if (index < 0)
        return;
    ui->navList->setCurrentRow(index);   // nav 선택이 바뀌면 stackMain 도 따라간다
}

void MainWindow::setDrivingEnabled(bool enabled)
{
    m_authenticated = enabled;

    // 인증 = 시동. 인증 전에는 운전자 인식 탭 외에는 아무것도 쓸 수 없다.
    // 페이지 단위로 잠그므로 나중에 위젯을 추가해도 이 함수는 그대로 둔다.
    for (int i = 0; i < ui->stackMain->count(); ++i) {
        QWidget *page = ui->stackMain->widget(i);
        if (page == ui->pageDriver)      // 인증 화면은 항상 열어둔다
            continue;
        page->setEnabled(enabled);

        // 잠긴 페이지는 목록에서도 눌리지 않게 한다
        if (QListWidgetItem *item = ui->navList->item(i)) {
            Qt::ItemFlags flags = item->flags();
            flags.setFlag(Qt::ItemIsEnabled, enabled);
            item->setFlags(flags);
        }
    }

    ui->lblMode->setText(enabled ? tr("모드: 수동") : tr("모드: 시동 꺼짐 (미인증)"));
    if (!enabled)
        ui->lblAuth->setText(tr("인증: 미인증"));
}

void MainWindow::retrainFaceModel()
{
    QString error;
    const bool ok = m_faceEngine->train(*m_databaseManager, &error);
    m_faceEngine->setDriverNames(m_databaseManager->driverNames());
    statusBar()->showMessage(ok ? tr("얼굴 모델 재학습 완료") : error, 5000);
}

void MainWindow::queryDatabase()
{
    if (ui->startDateTimeEdit->dateTime() > ui->endDateTimeEdit->dateTime()) {
        QMessageBox::warning(this, tr("조회 조건 오류"), tr("시작 시간은 종료 시간보다 늦을 수 없습니다."));
        return;
    }
    QString error;
    QSqlQueryModel *model = m_databaseManager->queryLogs(ui->logTypeComboBox->currentIndex(),
        ui->startDateTimeEdit->dateTime(), ui->endDateTimeEdit->dateTime(), this, &error);
    if (!model) { QMessageBox::critical(this, tr("DB 조회 오류"), error); return; }
    delete m_queryModel;
    m_queryModel = model;
    ui->resultTableView->setModel(m_queryModel);
    ui->resultCountLabel->setText(tr("조회 결과: %1건").arg(m_queryModel->rowCount()));
}

void MainWindow::generateSensorSnapshot()
{
    if (!m_databaseReady)
        return;

    QRandomGenerator *random = QRandomGenerator::global();
    m_dummyTemperature += (random->bounded(11) - 5) / 10.0;
    m_dummyHumidity += (random->bounded(11) - 5) / 10.0;
    m_dummyTemperature = qBound(18.0, m_dummyTemperature, 35.0);
    m_dummyHumidity = qBound(30.0, m_dummyHumidity, 80.0);
    if (random->bounded(10) == 0)
        m_dummyFanState = (m_dummyFanState == QStringLiteral("ON"))
                ? QStringLiteral("OFF") : QStringLiteral("ON");

    QString error;
    if (!m_databaseManager->insertSensorLog(m_dummyTemperature, m_dummyHumidity,
                                             m_dummyFanState, &error)) {
        showDatabaseError(tr("센서 로그 저장"), error);
        return;
    }
    refreshDatabaseView();
}

void MainWindow::generateVehicleState()
{
    if (!m_databaseReady)
        return;

    static const QStringList directions = {
        QStringLiteral("FWD"), QStringLiteral("BACK"), QStringLiteral("LEFT"),
        QStringLiteral("RIGHT"), QStringLiteral("STOP")
    };
    QRandomGenerator *random = QRandomGenerator::global();
    const QString nextDirection = directions.at(random->bounded(directions.size()));
    const int nextSpeed = nextDirection == QStringLiteral("STOP") ? 0 : random->bounded(10, 81);

    if (nextDirection == m_dummyDirection && nextSpeed == m_dummySpeed)
        return;
    m_dummyDirection = nextDirection;
    m_dummySpeed = nextSpeed;

    QString error;
    if (!m_databaseManager->insertVehicleLog(m_dummyDirection, m_dummySpeed, &error)) {
        showDatabaseError(tr("차량 로그 저장"), error);
        return;
    }
    refreshDatabaseView();
}

void MainWindow::refreshDatabaseView()
{
    if (ui->stackMain->currentWidget() == ui->databaseTab)
        queryDatabase();
}

void MainWindow::showDatabaseError(const QString &operation, const QString &error)
{
    m_databaseReady = false;
    if (m_sensorTimer)
        m_sensorTimer->stop();
    if (m_vehicleTimer)
        m_vehicleTimer->stop();
    QMessageBox::critical(this, tr("DB 저장 오류"), tr("%1에 실패했습니다.\n%2").arg(operation, error));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_databaseReady && !m_stopEventSaved) {
        QString error;
        if (!m_databaseManager->insertSystemEvent(QStringLiteral("SYSTEM_STOP"), &error)) {
            QMessageBox::critical(this, tr("DB 저장 오류"),
                                  tr("종료 이벤트 저장에 실패했습니다.\n%1").arg(error));
            event->ignore();
            return;
        }
        m_stopEventSaved = true;
    }
    QMainWindow::closeEvent(event);
}