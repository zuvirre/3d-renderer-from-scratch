#include "mesh.h"

namespace Renderer {
Mesh::Mesh(ColorFunction ambient, ColorFunction diffuse, ColorFunction specular)
        : ambient_color_function_(ambient),
          diffuse_color_function_(diffuse),
          specular_color_function_(specular) {
}

void Mesh::SetNormalFunction(NormalFunction normal_function) {
  normal_function_ = normal_function;
}

Iterable<Mesh::VertsConstIterator> Mesh::GetVerts() const {
  return Iterable(vertices_.begin(),vertices_.end());
}

Iterable<Mesh::PolygonConstIterator> Mesh::GetPolygons() const {
  return Iterable(polygons_.begin(),polygons_.end());
}

Triangle Mesh::MakeTriangleFromPolygon(const Polygon &polygon) const {
  Triangle result;
  for (size_t i = 0; i < 3; ++i) {
    result.GetVerts()[i] = vertices_[polygon.points_(i)];
  }
  result.SetColorFunction(&ambient_color_function_, &diffuse_color_function_,
                          &specular_color_function_);
  if (normal_function_) {
    result.SetNormalFunction(&normal_function_);
  }
  return result;
}

void Mesh::AddPolygon(const Mat3D &coords) {
  Vector3D first_vec = coords.row(1) - coords.row(0);
  Vector3D second_vec = coords.row(2) - coords.row(0);
  Vector3D normal = first_vec.cross(second_vec);
  normal.normalize();
  int curr = vertices_.size();
  for (int i = 0; i < 3; ++i) {
    Vertex vert{.coordinates = static_cast<Vector3D>(coords.row(i).eval()),
                .normal = normal};
    vertices_.push_back(vert);
  }
  polygons_.push_back(Polygon{Vector3I{curr, curr + 1, curr + 2}});
}

void Mesh::AddPolygon(const Mat3D & coords, const Mat3D &normals) {
  
}

}  // namespace Renderer
