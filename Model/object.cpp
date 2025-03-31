#include "object.h"

namespace Renderer {
void Object::AddTriangle(const Mat3D&coordinates, size_t index) {
    meshes_[index].AddPolygon(coordinates);
}
void Object::AddTriangle(const Mat3D &coordinates,
                         const Mat3D &normals, size_t index) {
    meshes_[index].AddPolygon(coordinates, normals);
}
Object::Object() {
}
Mesh &Object::GetMesh(size_t index) {
        return meshes_[index];
}
Iterable<Object::MeshConstIterator> Object::GetMeshes() const {
    return Iterable<MeshConstIterator>(meshes_.begin(), meshes_.end());
}
    
void Object::AddMesh(Color ambient, Color diffuse, Color specular) {
    meshes_.emplace_back(
        [ambient](const Triangle &, const Vector3D &b_coordinates) { return ambient; },
        [diffuse](const Triangle &, const Vector3D &b_coordinates) { return diffuse; },
        [specular](const Triangle &, const Vector3D &b_coordinates) { return specular; });
}   
}