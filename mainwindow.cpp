#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "capturethread.h"
#include "displaythread.h"

#include <QCloseEvent>
#include <QLabel>
#include <QRegularExpression>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (QLabel *label : findChildren<QLabel *>(QRegularExpression(QStringLiteral("^lblCam")))) {
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label->setMinimumSize(160, 120);
        label->setAlignment(Qt::AlignCenter);
        label->setScaledContents(false);
    }
    styleNavAsToolBox();
    ui->navList->setCurrentRow(0);

    // ---- 사용자(내장) 카메라 : 인덱스 0 -> 운전자 인식 탭 ----
    m_userCapture = new CaptureThread(kUserCamSrc, 640, 480, this);
    m_userView = new DisplayThread(m_userCapture, 30, /*mirror=*/true, this);
    connect(m_userCapture, &CaptureThread::opened, this, &MainWindow::onCameraOpened);
    connect(m_userView, &DisplayThread::sendImage, this,
            [this](const QImage &img) { showImage(ui->lblCamInternal, img); });

    // ---- USB 카메라 : 인덱스 1 -> 후방 카메라 탭 ----
    m_rearCapture = new CaptureThread(kRearCamSrc, 640, 480, this);
    m_rearView = new DisplayThread(m_rearCapture, 30, /*mirror=*/true, this);
    connect(m_rearCapture, &CaptureThread::opened, this, &MainWindow::onCameraOpened);
    connect(m_rearView, &DisplayThread::sendImage, this,
            [this](const QImage &img) { showImage(ui->lblCamRear, img); });

    m_userCapture->start();
    m_userView->start();
    m_rearCapture->start();
    m_rearView->start();

    ui->btnCamRearToggle->setChecked(true);
    ui->btnCamRearToggle->setText(QStringLiteral("후방 카메라 끄기"));
    connect(ui->btnCamRearToggle, &QPushButton::toggled, this, &MainWindow::toggleRear);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showImage(QLabel *label, const QImage &image)
{
    if (image.isNull())
        return;
    // label->size() 를 쓰면 [픽스맵 -> sizeHint -> 라벨 확대] 순환이 생긴다.
    // 고정 크기로 스케일하면 순환이 끊긴다.
    label->setPixmap(QPixmap::fromImage(image).scaled(640, 480,
                     Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::onCameraOpened(bool ok, const QString &message)
{
    if (!ok) {
        auto *cap = qobject_cast<CaptureThread *>(sender());
        if (cap && cap->source() == kRearCamSrc)
            ui->lblCamRear->setText(message);
        else
            ui->lblCamInternal->setText(message);
    }
    statusBar()->showMessage(message, 5000);
}

void MainWindow::toggleRear(bool checked)
{
    m_rearView->setPaused(!checked);
    ui->btnCamRearToggle->setText(checked ? QStringLiteral("후방 카메라 끄기")
                                          : QStringLiteral("후방 카메라 켜기"));
    if (!checked)
        ui->lblCamRear->setText(QStringLiteral("후방 카메라 꺼짐"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 소비자(표시) 먼저, 생산자(캡처) 나중에
    m_userView->stop();
    m_rearView->stop();
    m_userCapture->stop();
    m_rearCapture->stop();
    event->accept();
}

void MainWindow::styleNavAsToolBox()
{
    ui->navList->setFocusPolicy(Qt::NoFocus);
    ui->navList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->navList->setStyleSheet(QStringLiteral(R"(
        QListWidget { background: palette(window); border: none; outline: none; }
        QListWidget::item {
            padding: 16px 12px; margin: 2px 4px;
            border: 1px solid palette(mid); border-radius: 4px;
            background: palette(button);
        }
        QListWidget::item:hover { background: palette(midlight); }
        QListWidget::item:selected {
            background: palette(highlight); color: palette(highlighted-text); font-weight: bold;
        }
    )"));
}
