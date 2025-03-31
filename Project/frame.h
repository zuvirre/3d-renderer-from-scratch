#pragma once

#include "utility.h"
#include "primitive.h"
#include <Eigen/Dense>
#include <vector>
#include <string>
#include <cstdint>

namespace Renderer {
class Frame {
public:
    Frame(size_t width, size_t height);
    Frame(Frame &&) noexcept = default;
    Frame(const Frame &) = default;
    Frame &operator=(const Frame &) = default;
    Frame &operator=(Frame &&) noexcept = default;

    //void clear(Color color = {0, 0, 0});
    void SetPixel(size_t row, size_t col, const Color &color);
    void SetZ(size_t row, size_t col, double z);
    double GetZ(size_t row, size_t col) const;

    const Color &GetPixel(size_t row, size_t col) const;
    size_t GetWidth() const;
    size_t GetHeight() const;

    const Eigen::Matrix<Color, Eigen::Dynamic, Eigen::Dynamic> &GetFrameMatrix() const;

private:
    Eigen::Matrix<Color, Eigen::Dynamic, Eigen::Dynamic> frame_matrix_;
    Eigen::MatrixXd z_buffer_;
};
}
