#pragma once
#include "Eigen/Dense"
#include "Eigen/src/Core/Matrix.h"
#include "linearalgebra.h"
#include <initializer_list>
#include <list>
#include <type_traits>

namespace Renderer {
class Color {
    public:
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

template <typename T>
class Iterable {
public:
    Iterable(T begin, T end) : begin_(begin), end_(end) {
    }
    T begin() {
        return begin_;
    }
    T end() {
        return end_;
    }
 private:
    T begin_;
    T end_;
};
}
