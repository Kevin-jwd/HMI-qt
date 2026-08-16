#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSize>
#include <QString>
#include <opencv2/core.hpp>

class CaptureThread;
class DatabaseManager;
class DisplayThread;
class FaceEngine;
class RecognitionThread;
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
    static constexpr int kSampleTarget = 20;   // 등록 시 모을 얼굴 샘플 수

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

    // ---- 얼굴 인식 ----
    void setupFaceRecognition();
    void toggleRecognition(bool checked);
    void startRegistration();
    void onSampleCaptured(const cv::Mat &grayFace, int count, int target);
    void onRegisterFinished(int count);
    void onAuthConfirmed(int driverId, double confidence);
    void showDriverList();
    void retrainFaceModel();
    void goToPage(QWidget *page);   // nav 선택과 페이지 전환을 함께 처리
    void setDrivingEnabled(bool enabled);   // 미인증 상태에서 주행 조작 차단
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

    FaceEngine *m_faceEngine = nullptr;
    RecognitionThread *m_recognizer = nullptr;
    QString m_registerName;
    int m_registerDriverId = -1;
    bool m_faceReady = false;
    bool m_authenticated = false;
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