#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSize>
#include <QString>

class CaptureThread;
class DatabaseManager;
class DisplayThread;
class QCloseEvent;
class QImage;
class QLabel;
class QSqlQueryModel;
class QTimer;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // list_cameras 로 확인한 인덱스 (카메라 2대)
    static constexpr int kUserCamSrc = 0;   // 내장 카메라 -> 운전자 인식 탭
    static constexpr int kRearCamSrc = 1;   // USB 카메라  -> 후방 카메라 탭

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void queryDatabase();
    void setupCameras();
    void showImage(QLabel *label, const QImage &image);
    void onCameraOpened(bool ok, const QString &message);
    void toggleRearCamera(bool checked);
    void logEvent(const QString &eventType);
    void generateSensorSnapshot();
    void generateVehicleState();
    void refreshDatabaseView();
    void showDatabaseError(const QString &operation, const QString &error);

    Ui::MainWindow *ui;
    DatabaseManager *m_databaseManager;
    CaptureThread *m_userCapture = nullptr;
    DisplayThread *m_userView = nullptr;
    CaptureThread *m_rearCapture = nullptr;
    DisplayThread *m_rearView = nullptr;
    // 영상 표시 크기를 고정한다. 라벨 크기에 맞춰 스케일하면
    // [픽스맵 -> sizeHint -> 라벨 확대] 순환이 생겨 화면이 점점 커진다.
    const QSize m_videoSize = QSize(480, 360);
    QSqlQueryModel *m_queryModel = nullptr;
    QTimer *m_sensorTimer = nullptr;
    QTimer *m_vehicleTimer = nullptr;
    double m_dummyTemperature = 26.0;
    double m_dummyHumidity = 55.0;
    QString m_dummyFanState = QStringLiteral("OFF");
    QString m_dummyDirection;
    int m_dummySpeed = -1;
    bool m_databaseReady = false;
    bool m_stopEventSaved = false;
};
#endif // MAINWINDOW_H
