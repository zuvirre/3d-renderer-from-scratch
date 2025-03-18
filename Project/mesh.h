#pragma once

#include "primitive.h"
#include "utility.h"
#include <Eigen/Dense>
#include <vector>
#include <functional>

namespace Renderer {
class Mesh {
public:
    using Vector3I = Eigen::Vector3i;
    struct Polygon {
        Vector3I points_;
    };
    using Vector3D = Eigen::Vector3d;
    using Mat3D = Eigen::Matrix3d;
    using ColorFunction = std::function<Color(const Triangle &, const Vector3D &)>;
    using NormalFunction = std::function<Vector3D(const Triangle &, const Vector3D &)>;
    using VertsConstIterator = std::vector<Vertex>::const_iterator;
    using PolygonConstIterator = std::vector<Polygon>::const_iterator;

    Mesh(ColorFunction ambient, ColorFunction diffuse, ColorFunction specular);
    void SetNormalFunction(NormalFunction normal_function);

    Iterable<VertsConstIterator> GetVerts() const;
    Iterable<PolygonConstIterator> GetPolygons() const;

    Triangle MakeTriangleFromPolygon(const Polygon &) const;
    void AddTriangle(const Mat3D &);
    void AddTriangle(const Mat3D &, const Mat3D &);
private:
    std::vector<Vertex> vertices_;
    std::vector<Polygon> polygons_;
    ColorFunction ambient_color_function_;
    ColorFunction diffuse_color_function_;
    ColorFunction specular_color_function_;
    NormalFunction normal_function_;
};
}
