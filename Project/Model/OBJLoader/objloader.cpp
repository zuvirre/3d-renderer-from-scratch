#include "../../../OBJ-Loader/Source/OBJ_Loader.h"
#include "objloader.h"
#include "linearalgebra.h"
#include <iostream>
#include <filesystem>

namespace kernel {
using Color = Renderer::Color;
using Vector3D = Eigen::Vector3d;
using Mat3D = Eigen::Matrix3d;

namespace {
Color MakeColor(const objl::Vector3 &vec) {
    Color ans;
    ans.SetRed(vec.X);
    ans.SetGreen(vec.Y);
    ans.SetBlue(vec.Z);
    return ans;
}
Vector3D MakeVector3d(const objl::Vector3 &vec) {
    Vector3D ans;
    ans.x() = vec.X;
    ans.y() = vec.Y;
    ans.z() = vec.Z;
    return ans;
}

}

void ReadAllFromDirectory(std::string path, Renderer::World *world) {
    std::filesystem::create_directory(path);
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        std::string file_path = entry.path().string();
        if (file_path.substr(file_path.size() - 4, file_path.size()) == ".obj") {
            world->AddObject(ReadObject(file_path));
        }
    }
}
Object ReadObject(const std::string &path) {
    Object ans;
    objl::Loader loader;
    std::cout << loader.LoadFile(path) << std::endl;
    std::cout << path;
    for (int i = 0; i < loader.LoadedMeshes.size(); ++i) {
        const auto &mesh = loader.LoadedMeshes[i];
        Color ambient, diffuse, specular;
        ambient = MakeColor(mesh.MeshMaterial.Ka);
        std::cout << mesh.MeshMaterial.Ka.X << ' ' << mesh.MeshMaterial.Ka.Y << ' '
                  << mesh.MeshMaterial.Ka.Z << std::endl;
        diffuse = MakeColor(mesh.MeshMaterial.Kd);
        specular = MakeColor(mesh.MeshMaterial.Ks);
        ans.AddMesh(ambient, diffuse, specular);
        for (int j = 0; j < mesh.Indices.size() - mesh.Indices.size() % 3; j += 3) {
            Mat3D coords;
            coords.row(0) = MakeVector3d(mesh.Vertices[mesh.Indices[j]].Position);
            coords.row(2) = MakeVector3d(mesh.Vertices[mesh.Indices[j + 1]].Position);
            coords.row(1) = MakeVector3d(mesh.Vertices[mesh.Indices[j + 2]].Position);

            Mat3D normals;
            normals.row(0) = MakeVector3d(mesh.Vertices[mesh.Indices[j]].Normal);
            normals.row(2) = MakeVector3d(mesh.Vertices[mesh.Indices[j + 1]].Normal);
            normals.row(1) = MakeVector3d(mesh.Vertices[mesh.Indices[j + 2]].Normal);
            ans.AddTriangle(coords, normals, i);
        }
        ans.GetMesh(i).SetNormalFunction(
            [](const Renderer::Triangle &tr, const Vector3D &b_coords) -> Vector3D {
                Vector3D ans = Vector3D::Zero();
                for (int k = 0; k < tr.GetVerts().size(); ++k) {
                    ans += tr.GetVerts()[k].normal.GetCoordinates() * b_coords[k];
                }
                return ans.normalized();
            });
    }
    return ans;
}
}
