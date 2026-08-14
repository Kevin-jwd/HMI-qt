#pragma once
#include <opencv2/core.hpp>

// 프레임 공급자 인터페이스.
class FrameSource
{
public:
    virtual ~FrameSource() = default;
    virtual cv::Mat readLatest() const = 0;   // 없으면 empty
};
