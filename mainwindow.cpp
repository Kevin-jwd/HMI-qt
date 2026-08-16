#include "mainwindow.h"
#include "capturethread.h"
#include "databasemanager.h"
#include "displaythread.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QCloseEvent>
#include <QImage>
#include <QLabel>
#include <QPixmap>
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
}

MainWindow::~MainWindow()
{
    // 소비자(표시) 먼저, 생산자(캡처) 나중에
    if (m_userView)    m_userView->stop();
    if (m_rearView)    m_rearView->stop();
    if (m_userCapture) m_userCapture->stop();
    if (m_rearCapture) m_rearCapture->stop();

    delete m_queryModel;
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
