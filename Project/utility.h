#pragma once
#include "Eigen/Dense"
#include "Eigen/src/Core/Matrix.h"
#include <initializer_list>
#include <list>
#include <type_traits>

namespace Renderer {
class Color {
    public:
        using Vector3D = Eigen::Vector3d;

        Color(std::initializer_list<double> init) {
        }
        Color() : value_{{0.0,0.0,0.0}} {
        }
        double GetRed() const;
        double GetGreen() const;
        double GetBlue() const;
        void SetRed(double);
        void SetGreen(double);
        void SetBlue(double);

        Color operator+(const Color &other) const;
        Color operator*(const Color &other) const;
        Color operator*(double a) const;
        Color &operator+=(const Color &other);
        Color &operator*=(const Color &other);
        Color &operator*=(double a);

        void Normalize();

        uint8_t GetRedInt() const;
        uint8_t GetGreenInt() const;
        uint8_t GetBlueInt() const;

    private:
        static uint8_t DoubleConvert(double value);
        Vector3D value_;
};

class Point {
public:
    using Vector3D = Eigen::Vector3d;
    using Vector4D = Eigen::Vector4d;

    Point() = default;
    Point(const Vector3D &);
    Point(const Vector4D &);

    Vector3D GetCoordinates() const;
    const Vector4D &GetHomogenousCoordinates() const;
    double w();
    Point &operator=(const Vector3D &);
    Point &operator=(const Vector4D &);

private:
    Vector4D data_ = Vector4D::Ones();

};

class Direction {
public:
    using Vector3D = Eigen::Vector3d;
    using Vector4D = Eigen::Vector4d;

    Direction() = default;
    Direction(const Vector3D &);
    Direction(const Vector4D &);
    
    Vector3D GetCoordinates() const;
    Vector4D GetHomogenousCoordinates() const;

    Direction& operator=(const Vector3D &);
    Direction& operator=(const Vector4D &);

private:
    Vector3D data_ = Vector3D::Zero();
};

class Vertex {
    Point coords;
    Direction normal;
};

}