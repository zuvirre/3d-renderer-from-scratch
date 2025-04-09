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
}