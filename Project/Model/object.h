#pragma once 

#include "mesh.h"
#include <iostream>

namespace Renderer {
class Object {
    public:
    using ColorFunction = std::function<Color(const Triangle&, const Vector3D&)>;
    using MeshConstIterator = std::vector<Mesh>::const_iterator;
    Object();
    void AddTriangle(const Mat3D&, size_t index = 0);
    void AddTriangle(const Mat3D&, const Mat3D&, size_t index = 0);
    Iterable<MeshConstIterator> GetMeshes() const;
    void AddMesh(Color ambient, Color diffuse, Color specular);
    Mesh& GetMesh(size_t index = 0);

private:
    std::vector<Mesh> meshes_;
};

class Sphere {
    public:
    using ColorFunction = std::function<Color(const Triangle&, const Eigen::Vector3d&)>;
    using MeshConstIterator = std::vector<Mesh>::const_iterator;
    Sphere(Color ambient, Color diffuse, Color specular, double radius = 1.0,
           size_t max_triangle_count = 320);
    Iterable<MeshConstIterator> GetMeshes() const;
    Mesh& GetMesh(size_t index = 0);

private:
    static std::list<Mat3D> MakeIcosahedron();
    static void SubDivideAndExtrude(Mat3D, std::list<Mat3D>*);
    static Mat3D MakeMatrix(Vector3D, Vector3D, Vector3D);
    void AddMesh(Color ambient, Color diffuse, Color specular);
    std::vector<Mesh> meshes_;
};
}
