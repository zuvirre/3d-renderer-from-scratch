#pragma once

#include "linearalgebra.h"
#include "utility.h"
namespace Renderer {
class Point {
public:
    Point() = default;
    Point(const Vector3D &);
    Point(const Vector4D &);
    
    Vector3D GetCoordinates() const;
    const Vector4D &GetHomogeneousCoordinates() const;
    double w();
    Point &operator=(const Vector3D &);
    Point &operator=(const Vector4D &);
    
    template <typename T, class = typename std::enable_if<!std::is_same_v<Vector3D, T> &&
                                                          std::is_convertible_v<T, Vector3D>>>
    Point &operator=(const T &val) {
        return *this = static_cast<Vector3D>(val.eval());
    }    
    template <typename T, typename F,
                class = typename std::enable_if<!std::is_same_v<Vector4D, T> &&
                                                !std::is_convertible_v<T, Vector3D> &&
                                                std::is_convertible_v<T, Vector4D>>>
    Point &operator=(const T &val) {
        return *this = static_cast<Vector4D>(val.eval());
    }
    
private:
    Vector4D data_ = Vector4D::Ones();
    
};
}
