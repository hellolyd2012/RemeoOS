// 创建于2024.10.20
// RemeoOS 内核帧缓冲头文件

#ifndef _FBC_HPP_
#define _FBC_HPP_

#include "UEFI.hpp"

enum PixelFormat {
    kRGBR, // 带k表示内核会用到
    kBGRR
}; // only支持这两种

struct FrameBufferConfig {
    UINT8 *frame_buffer;
    UINT32 pixels_per_scan_line;
    UINT32 horizontal_resolution;
    UINT32 vertical_resolution;
    enum PixelFormat pixel_format;
};

#endif