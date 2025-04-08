#pragma once

#include "Eigen/Dense"
#include "Eigen/src/Core/Matrix.h"
#include "Eigen/src/Geometry/Quaternion.h"
#include "primitive.h"
#include "world.h"
#include "frame.h"

namespace Renderer {
class Rasterizer {
public:
    Rasterizer() = default;
    struct LightSourcesDescription {
        std::vector<Vector3D> point_light_coordinates_;
    };
    void SwitchLightModel();
    void ToggleLightMarkers();

    std::unique_ptr<Frame> Draw(const World &world, size_t width, size_t height);
    void DrawTriangle(const Mesh::Polygon &current, const Mesh &,
        AnyConstHolderPointer owner_object, const World &world,
        const LightSourcesDescription &, Frame *screen);

    void ShiftLightToAlignCamera(const World &, LightSourcesDescription *desc);
    void ShiftTriangleCoordinates(const AnyConstHolderPointer owner, Triangle *);
    void ShiftTriangleToAlignCamera(const World &, Triangle *);
    void RasterizeTriangle(const BarycentricSystem &, const Mat3D &,
                           const World &, const LightSourcesDescription &, Frame *);

    static bool DetermineSide(const Vector4D &plane, const Vector3D &point);
    static Vector3D PlaneLineIntersection(const Vector4D &plane,
                                                 const Vector3D &point1,
                                                 const Vector3D &point2);

    static bool ClipOneTriangle(const Vector4D &plane, const Eigen::Matrix3d &triangle,
                                std::list<Mat3D> *triangles);
    static void ClipAllTriangles(const Vector4D &plane,
                                 std::list<Mat3D> *triangles);

    Color CalculateBlinnPhong(const Color &ambient_color, const Color &diffuse_color,
                            const Color &specular_color, const LightSourcesDescription &desc,
                            const World &world, const Vector3D &b_coordinates,
                            const Triangle &triangle);
    Color CalculateBlinnPhongDiffusion(const Color &initial_color, const LightSourcesDescription &desc,
                                     const World &world, const Vector3D &coordinates,
                                     const Vector3D &normal, const Vector3D &real_normal);
    Color CalculateBlinnPhongSpecular(const Color &initial_color, const LightSourcesDescription &desc,
                                    const World &world, const Vector3D &coordinates,
                                    const Vector3D &normal, const Vector3D &real_normal);
private:
    bool use_blinn_phong_ = true;
    bool render_light_markers_ = true;
    static Mat34D MakeHomogeneousTransformationMatrix(const QuatD &rotation,
                                                     const Vector3D &offset);
    static void ApplyMatrix(const Mat34D &, Triangle *);
};
}
