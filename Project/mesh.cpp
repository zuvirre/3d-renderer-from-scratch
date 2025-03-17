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

}  // namespace Renderer
