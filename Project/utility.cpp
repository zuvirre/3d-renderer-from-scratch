#include <utility.h>
#include <cassert>
#include <iostream>

namespace Renderer {
    double Color::GetRed() const {
        return value_(0);
    }

    double Color::GetGreen() const {
        return value_(1);
    }
    
    double Color::GetBlue() const {
        return value_(2);
    }

    void Color::SetRed(double value) {
        value_(0) = value;
    }

    void Color::SetGreen(double value) {
        value_(1) = value;
    }

    void Color::SetBlue(double value) {
        value_(2) = value;
    }
    
    void Renderer::Color::Normalize() {
        value_ = value_.cwiseMin(1.0);
        value_ = value_.cwiseMax(0.0);
    }

    Color Color::operator+(const Color& other) const {
        Color result = *this;
        result.value_ += other.value_;
        return result;
    }
    Color Color::operator*(const Color& other) const {
        Color result = *this;
        result.value_.cwiseProduct(other.value_);
        return result;
    }
    Color Color::operator*(double a) const {
        Color result = *this;
        result.value_ *= a;
        return result;
    }
    Color& Color::operator+=(const Color& other) {
        this->value_ += other.value_;
        return *this;
    }
    Color& Color::operator*=(const Color& other) {
        value_ = this->value_.cwiseProduct(other.value_);
        return *this;
    }
    Color& Color::operator*=(double a) {
        value_ *= a;
        return *this;
    }

    uint8_t Color::DoubleConvert(double value) {
        return std::min(std::max(static_cast<uint8_t>(value * 255.0), uint8_t{0}), uint8_t{255});
    }

    uint8_t Color::GetRedInt() const {
        return Color::DoubleConvert(GetRed());
    }

    uint8_t Color::GetGreenInt() const {
        return Color::DoubleConvert(GetGreen());
    }

    uint8_t Color::GetBlueInt() const {
        return Color::DoubleConvert(GetBlue());
    }
}