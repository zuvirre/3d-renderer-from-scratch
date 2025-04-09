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
    Frame(int width, int height);
    Frame(Frame &&) noexcept = default;
    Frame(const Frame &) = default;
    Frame &operator=(const Frame &) = default;
    Frame &operator=(Frame &&) noexcept = default;

    //void clear(Color color = {0, 0, 0});
    void SetPixel(int row, int col, const Color &color);
    void SetZ(int row, int col, double z);
    double GetZ(int row, int col) const;

    const Color &GetPixel(int row, int col) const;
    int GetWidth() const;
    int GetHeight() const;

    const Eigen::Matrix<Color, Eigen::Dynamic, Eigen::Dynamic> &GetFrameMatrix() const;

private:
    Eigen::Matrix<Color, Eigen::Dynamic, Eigen::Dynamic> frame_matrix_;
    Eigen::MatrixXd z_buffer_;
};
}
