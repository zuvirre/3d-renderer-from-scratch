#pragma once

#include "linearalgebra.h"
#include "utility.h"
#include "point.h"

namespace Renderer {
class Direction {
public:
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
    
struct Vertex {
    Point coordinates;
    Direction normal;
};
}
