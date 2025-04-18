#pragma once

#include "../linearalgebra.h"
#include "utility.h"
#include "point.h"

namespace Renderer {
class Direction {
public:
    Direction() = default;
    Direction(const Vector3D &);
    Direction(const Vector4D &);
    template<typename T>
    Direction(const T &evaluable) {
        *this = evaluable.eval();
    }  
    Vector3D GetCoordinates() const;
    Vector4D GetHomogeneousCoordinates() const;
    template <typename T>
    Direction &operator=(const T &evaluable) {
        return *this = evaluable.eval();
    }
    Direction& operator=(const Vector3D &);
    Direction& operator=(const Vector4D &);
    
private:
    Vector3D data_ = Vector3D::Zero();
};
    
struct Vertex {
    Point coordinates;
    Direction normal;
};
}
