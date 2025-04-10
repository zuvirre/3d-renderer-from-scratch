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

Iterable<Object::MeshConstIterator> Renderer::Sphere::GetMeshes() const {
    return Iterable<MeshConstIterator>(meshes_.begin(), meshes_.end());
}

Renderer::Sphere::Sphere(Color ambient, Color diffuse, Color specular, double radius,
    size_t max_triangle_count) {
std::list<Mat3D> triangles = MakeIcosahedron();
// subdivide
AddMesh(ambient, diffuse, specular);
while (triangles.size() * 4 <= max_triangle_count) {
auto it = triangles.end();
--it;
while (true) {
SubDivideAndExtrude(*it, &triangles);
auto it1 = it;

if (it == triangles.begin()) {
triangles.erase(it1);
break;
}
--it;
triangles.erase(it1);
}
}
for (const auto &triangle : triangles) {
meshes_[0].AddPolygon(triangle * radius, triangle);
}

meshes_[0].SetNormalFunction(
[](const Renderer::Triangle &tr, const Vector3D &b_coords) -> Vector3D {
Vector3D ans = Vector3D::Zero();
for (int k = 0; k < tr.GetVerts().size(); ++k) {
ans += tr.GetVerts()[k].normal.GetCoordinates() * b_coords[k];
}
return ans.normalized();
});
}

void Renderer::Sphere::AddMesh(Color ambient, Color diffuse, Color specular) {
meshes_.emplace_back(
[ambient](const Triangle &, const Vector3D &b_coordinates) { return ambient; },

[diffuse](const Triangle &, const Vector3D &b_coordinates) { return diffuse; },

[specular](const Triangle &, const Vector3D &b_coordinates) { return specular; });
}
std::list<Mat3D> Sphere::MakeIcosahedron() {
std::list<Mat3D> triangles;
Mat3D triangle;
double phi = (1.0f + sqrt(5.0f)) * 0.5f;  // golden ratio
double a = 1.0f;
double b = 1.0f / phi;

// add vertices
Vector3D v1(0, b, -a);
Vector3D v2(b, a, 0);
Vector3D v3(-b, a, 0);
Vector3D v4(0, b, a);
Vector3D v5(0, -b, a);
Vector3D v6(-a, 0, b);
Vector3D v7(0, -b, -a);
Vector3D v8(a, 0, -b);
Vector3D v9(a, 0, b);
Vector3D v10(-a, 0, -b);
Vector3D v11(b, -a, 0);
Vector3D v12(-b, -a, 0);

// add triangles
triangles.push_back(MakeMatrix(v3, v2, v1));
triangles.push_back(MakeMatrix(v2, v3, v4));
triangles.push_back(MakeMatrix(v6, v5, v4));
triangles.push_back(MakeMatrix(v5, v9, v4));
triangles.push_back(MakeMatrix(v8, v7, v1));
triangles.push_back(MakeMatrix(v7, v10, v1));
triangles.push_back(MakeMatrix(v12, v11, v5));
triangles.push_back(MakeMatrix(v11, v12, v7));
triangles.push_back(MakeMatrix(v10, v6, v3));
triangles.push_back(MakeMatrix(v6, v10, v12));
triangles.push_back(MakeMatrix(v9, v8, v2));
triangles.push_back(MakeMatrix(v8, v9, v11));
triangles.push_back(MakeMatrix(v3, v6, v4));
triangles.push_back(MakeMatrix(v9, v2, v4));
triangles.push_back(MakeMatrix(v10, v3, v1));
triangles.push_back(MakeMatrix(v2, v8, v1));
triangles.push_back(MakeMatrix(v12, v10, v7));
triangles.push_back(MakeMatrix(v8, v11, v7));
triangles.push_back(MakeMatrix(v6, v12, v5));
triangles.push_back(MakeMatrix(v11, v9, v5));
for (auto &triangle : triangles) {
Vector3D normal =
(triangle.row(0) - triangle.row(1)).cross(triangle.row(0) - triangle.row(2));
normal = -normal;
if (normal.dot(triangle.row(0)) > 0) {
Vector3D r1 = triangle.row(0);
Vector3D r2 = triangle.row(1);
triangle.row(0) = r2;
triangle.row(1) = r1;
}
for (size_t i = 0; i < 3; ++i) {
triangle.row(i).normalize();
}
}

return triangles;
}
void Sphere::SubDivideAndExtrude(Mat3D triangle, std::list<Mat3D> *triangles) {
Vector3D middle01 = Vector3D{0.5, 0.5, 0}.transpose() * triangle;
Vector3D middle12 = Vector3D{0, 0.5, 0.5}.transpose() * triangle;
Vector3D middle02 = Vector3D{0.5, 0., 0.5}.transpose() * triangle;
middle01.normalize();
middle02.normalize();
middle12.normalize();
Mat3D res_triangle;
res_triangle.row(0) = triangle.row(0);
res_triangle.row(1) = middle01;
res_triangle.row(2) = middle02;
triangles->push_back(res_triangle);

res_triangle.row(0) = middle01;
res_triangle.row(1) = triangle.row(1);
res_triangle.row(2) = middle12;
triangles->push_back(res_triangle);

res_triangle.row(0) = middle01;
res_triangle.row(1) = middle12;
res_triangle.row(2) = middle02;
triangles->push_back(res_triangle);

res_triangle.row(0) = middle02;
res_triangle.row(1) = middle12;
res_triangle.row(2) = triangle.row(2);
triangles->push_back(res_triangle);
}
Mat3D Sphere::MakeMatrix(Vector3D a, Vector3D b, Vector3D c) {
Mat3D ans;
ans.row(0) = a;
ans.row(1) = b;
ans.row(2) = c;
return ans;
}
}
