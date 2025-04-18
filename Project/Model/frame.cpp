#include "frame.h"
#include <fstream>
#include <iostream>

namespace Renderer {
Frame::Frame(int width, int height)
    : frame_matrix_(height, width), z_buffer_(height, width) {
    z_buffer_.setZero();
}

void Frame::SetPixel(int row, int col, const Color &color) {
    frame_matrix_(row, col) = color;
}

void Frame::SetZ(int row, int col, double z) {
    z_buffer_(row, col) = z;
}

double Frame::GetZ(int row, int col) const {
    return z_buffer_(row, col);
}
const Color &Frame::GetPixel(int row, int col) const {
    return frame_matrix_(row,col);
}

int Frame::GetWidth() const {
    return frame_matrix_.cols();
}

int Frame::GetHeight() const {
    return frame_matrix_.rows();
}

const Eigen::Matrix<Color, Eigen::Dynamic, Eigen::Dynamic> &Frame::GetFrameMatrix()
    const {
        return frame_matrix_;
    }

}
