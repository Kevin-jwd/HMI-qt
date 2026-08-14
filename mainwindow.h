#pragma once
#include <QImage>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CaptureThread;
class DisplayThread;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    // list_cameras 로 확인한 인덱스
    static constexpr int kUserCamSrc = 0;   // 내장 카메라 -> 운전자(사용자) 캠
    static constexpr int kRearCamSrc = 1;   // USB 카메라  -> 후방 캠

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onCameraOpened(bool ok, const QString &message);
    void toggleRear(bool checked);

private:
    void styleNavAsToolBox();
    void showImage(QLabel *label, const QImage &image);

    Ui::MainWindow *ui;

    CaptureThread *m_userCapture = nullptr;
    DisplayThread *m_userView = nullptr;
    CaptureThread *m_rearCapture = nullptr;
    DisplayThread *m_rearView = nullptr;
};
