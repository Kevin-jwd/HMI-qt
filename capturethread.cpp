#include "capturethread.h"

#include <QMutexLocker>
#include <QPair>
#include <QVector>
#include <opencv2/videoio.hpp>

CaptureThread::CaptureThread(int src, int width, int height, QObject *parent)
    : QThread(parent), m_src(src), m_width(width), m_height(height)
{
}

void CaptureThread::run()
{
    // 백엔드를 순서대로 시도한다. Windows 는 DSHOW 가 가장 잘 붙는다.
#if defined(Q_OS_WIN)
    const QVector<QPair<QString, int>> backends{
        {QStringLiteral("DSHOW"), cv::CAP_DSHOW}, {QStringLiteral("MSMF"), cv::CAP_MSMF},
        {QStringLiteral("ANY"), cv::CAP_ANY}};
#elif defined(Q_OS_LINUX)
    const QVector<QPair<QString, int>> backends{
        {QStringLiteral("V4L2"), cv::CAP_V4L2}, {QStringLiteral("ANY"), cv::CAP_ANY}};
#else
    const QVector<QPair<QString, int>> backends{{QStringLiteral("ANY"), cv::CAP_ANY}};
#endif

    cv::VideoCapture cap;
    QString usedBackend;

    // 다른 카메라가 초기화 중이면 실패할 수 있어 몇 번 재시도한다
    for (int attempt = 0; attempt < kOpenRetries && !cap.isOpened(); ++attempt) {
        for (const auto &b : backends) {
            if (cap.open(m_src, b.second) && cap.isOpened()) {
                usedBackend = b.first;
                break;
            }
        }
        if (!cap.isOpened())
            msleep(400);
    }

    if (!cap.isOpened()) {
        emit opened(false, QStringLiteral("카메라 %1 열기 실패").arg(m_src));
        return;
    }

    cap.set(cv::CAP_PROP_BUFFERSIZE, 1);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, m_width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, m_height);
    emit opened(true, QStringLiteral("카메라 %1 연결됨 (%2)").arg(m_src).arg(usedBackend));

    while (m_running) {
        cv::Mat frame;
        if (!cap.read(frame) || frame.empty()) {
            msleep(5);
            continue;
        }
        QMutexLocker lock(&m_mutex);
        m_frame = frame;    // 밀린 프레임은 그냥 버림
    }
    cap.release();
}

cv::Mat CaptureThread::readLatest() const
{
    QMutexLocker lock(&m_mutex);
    return m_frame.empty() ? cv::Mat() : m_frame.clone();
}

void CaptureThread::stop()
{
    m_running = false;
    wait(2000);
}
