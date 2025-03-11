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

    Point::Point(const Vector4D& data) {
        data_(data);
    }
    Point::Point(const Vector3D& data) {
        data_.topLeftCorner<3,1>() = data;
    }

    Point::Vector3D Point::GetCoordinates() const{
        return data_.topLeftCorner<3,1>();
    }
    const Point::Vector4D &Point::GetHomogenousCoordinates() const {
        return data_;
    }

    double Point::w() {
        return data_.w();
    }

    Point &Point::operator=(const Vector4D& coords) {
        data_ = coords;
        return *this;
    
    Point &Point::operator=(const Vector3D& coords) {
        data_.topLeftCorner<3,1>() = coords;
        data_.w() = 1;
        return *this;
    }

    Direction::Direction(const Vector3D& data) {
        data_(data);
    }
    Direction::Direction(const Vector4D& data) {
        data_(data.topLeftCorner<3,1>());
    }

    Direction::Vector3D Direction::GetCoordinates() const {
        return data_;
    }
    Direction::Vector4D Direction::GetHomogenousCoordinates() const {
        Vector4D result;
        result.topLeftCorner<3,1>() = data_;
        result.w() = 0;
        return result;
    }

    Direction& Direction::operator=(const Vector3D& coords) {
        data_ = coords;
        return *this;
    }
    Direction& Direction::operator=(const Vector4D& coords) {
        data_ = coords.topLeftCorner<3,1>();
        return *this;
    }
}
