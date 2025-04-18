#include "rasterizer.h"
#include "primitive.h"
#include <cassert>
#include <iostream>

namespace Renderer {


namespace {
Mat3D TransformToScreenSpace(Mat3D triangle, size_t width,
                             size_t height) {
    assert(width != 0);
    assert(height != 0);
    double dwidth = static_cast<double>(width);
    double dheight = static_cast<double>(height);

    triangle.col(0) += Vector3D::Ones();
    triangle.col(0) *= dwidth / 2;
    triangle.col(1) = -triangle.col(1);
    triangle.col(1) += Vector3D::Ones();
    triangle.col(1) *= dheight / 2;
    return triangle;
}

Mat3D TransformToCameraSpace(Mat3D triangle, size_t width,
                             size_t height) {
    assert(width != 0);
    assert(height != 0);
    double dwidth = static_cast<double>(width);
    double dheight = static_cast<double>(height);

    triangle.col(0) /= dwidth / 2;
    triangle.col(0) -=Vector3D::Ones();
    triangle.col(1) /= dheight / 2;
    triangle.col(1) -= Vector3D::Ones();
    triangle.col(1) = -triangle.col(1);
    return triangle;
}

Vector2D TransformVectorToCameraSpace(Vector2D vec, size_t width, size_t height) {
    assert(width != 0);
    assert(height != 0);
    double dwidth = width;
    double dheight = height;

    vec.x() /= dwidth / 2;
    vec.x() -= 1;

    vec.y() /= dheight / 2;
    vec.y() -= 1;
    vec.y() = -vec.y();
    return vec;
}

Vector3D ConvertToBarycentric(Vector2D coordinates, const Mat3D &triangle) {
    Vector3D result;
    coordinates -= triangle.row(2).topLeftCorner<1, 2>();
    Mat2D temp;
    temp = triangle.topLeftCorner<2, 2>().transpose();
    temp.col(0) -= triangle.row(2).topLeftCorner<1, 2>();
    temp.col(0) -= triangle.row(2).topLeftCorner<1, 2>();
    result.topLeftCorner<2, 1>() =temp.inverse() * coordinates;
    result.z() = 1 - result.x() - result.y();
    return result;
}

bool CheckIfInside(const Vector3D &b_coords) {
    return b_coords.x() <= 1.0 && b_coords.x() >= 0.0 && b_coords.y() <= 1.0 &&
           b_coords.y() && b_coords.z() <= 1.0 && b_coords.z() >= 0;
}

size_t RoundDown(double coordinate) {
    return static_cast<size_t>(std::max(0.0, std::floor(coordinate)));
}
size_t RoundUp(double coordinate) {
    return static_cast<size_t>(std::ceil(coordinate));
}
};

void Rasterizer::SwitchLightModel() {
    use_blinn_phong_ = !use_blinn_phong_;
}

void Rasterizer::ToggleLightMarkers() {
    render_light_markers_ = !render_light_markers_;
}


void Rasterizer::ShiftLightToAlignCamera(const World &world, LightSourcesDescription *desc) {
    Mat34D transformation_matrix = Rasterizer::MakeHomogeneousTransformationMatrix(
        world.GetCameraRotation().inverse(),
        world.GetCameraRotation().inverse() * -world.GetCameraPosition());
    for (int i = 0; i < world.GetPointLightSources().size(); ++i) {
        Vector4D temp = Vector4D::Ones();
        temp.topLeftCorner<3, 1>() = world.GetPointLightSources()[i].GetCoordinates();
        desc->point_light_coordinates_.push_back(transformation_matrix * temp);
    }
}
void Rasterizer::ShiftTriangleCoordinates(AnyConstHolderPointer owner, Triangle *vertices) {
    assert(vertices != nullptr);
    Mat34D transformation_matrix =
        Rasterizer::MakeHomogeneousTransformationMatrix(owner->GetAngle(), owner->GetCoordinates());
    Rasterizer::ApplyMatrix(transformation_matrix, &(*vertices));
}

void Rasterizer::ShiftTriangleToAlignCamera(const World &world, Triangle *vertices) {
    assert(vertices != nullptr);
    Mat34D transformation_matrix = Rasterizer::MakeHomogeneousTransformationMatrix(
        world.GetCameraRotation().inverse(),
        world.GetCameraRotation().inverse() * -world.GetCameraPosition());
    Rasterizer::ApplyMatrix(transformation_matrix, vertices);
}

std::unique_ptr<Frame> Rasterizer::Draw(const World& world, size_t width, size_t height) {
    std::unique_ptr<Frame> frame(new Frame(width, height));
    LightSourcesDescription light_desc;
    ShiftLightToAlignCamera(world, &light_desc);
    for (const auto &object : world.GetObjectsIterable()) {
        for (const auto &mesh : object->GetMeshes()) {
            for (const auto &triangle : mesh.GetPolygons()) {
                DrawTriangle(triangle, mesh, &object, world, light_desc, frame.get());
            }
        }
    }
    if (!render_light_markers_) {
        return frame;
    }
    for (const auto &point_lights : world.GetPointLightSources()) {
        for (const auto &triangle  : point_lights.GetRepresentingMesh().GetPolygons()) {
            DrawTriangle(triangle, point_lights.GetRepresentingMesh(), &point_lights, world,
                         light_desc, frame.get());
        }
    }
    return frame;
}

void Rasterizer::DrawTriangle(const Mesh::Polygon &current, const Mesh &mesh,
                              AnyConstHolderPointer owner_object, const World &world,
                              const LightSourcesDescription &light_desc, Frame *frame) {
    Triangle vertices = mesh.MakeTriangleFromPolygon(current);
    ShiftTriangleCoordinates(owner_object, &vertices);
    ShiftTriangleToAlignCamera(world, &vertices);
    Mat34D transformed_vertices =
        world.GetCamera().PerspectiveTransformation(
            vertices.GetVertsHomoCoords());
    vertices.CalculateNorm();
    BarycentricSystem system(vertices, transformed_vertices);
    std::list<Mat3D> triangles;
    triangles.push_back(vertices.GetVertsHomoCoords().topLeftCorner<3,3>());
    for (int i = 0; i < world.GetCamera().GetClippingPlanes().rows(); ++i) {
        ClipAllTriangles(world.GetCamera().GetClippingPlanes().row(i), &triangles);
    }

    for (const auto &curr : triangles) {
        Mat34D homogeneous_coords;
        homogeneous_coords.topLeftCorner<3, 3>() = curr;
        homogeneous_coords.col(3) = Vector3D::Ones();
        homogeneous_coords = world.GetCamera().PerspectiveTransformation(homogeneous_coords);
        RasterizeTriangle(system, homogeneous_coords.topLeftCorner<3, 3>(), world, light_desc,
                          frame);
    }
    std::cout.flush();
}

void Rasterizer::RasterizeTriangle(const BarycentricSystem &system,
                                   const Mat3D &coordinates, const World &world,
                                   const LightSourcesDescription &desc, Frame *frame) {
    size_t height = frame->GetHeight();
    size_t width = frame->GetWidth();
    Mat3D frame_triangle = TransformToScreenSpace(coordinates, width, height);
    Mat2D inverse_matrix =
        BarycentricSystem::MakeBarycentricTransformationMatrix(coordinates);
    
    size_t min_x = -1;
    size_t min_y = -1;
    size_t max_x = 0;
    size_t max_y = 0;

    for(int i = 0; i < 3; ++i) {
        min_x = std::min(RoundDown(frame_triangle.row(i).x()), min_x);
        min_y = std::min(RoundDown(frame_triangle.row(i).y()), min_y);
        max_x = std::max(RoundUp(frame_triangle.row(i).x()), max_x);
        max_y = std::max(RoundUp(frame_triangle.row(i).y()), max_y);
    }
    assert("bounded dimensions are not OK" && max_x < 2 * frame->GetWidth() &&
           max_y < 2 * frame->GetHeight());
    max_x = std::min(static_cast<size_t>(frame->GetWidth() - 1), max_x);
    max_y = std::min(static_cast<size_t>(frame->GetHeight() - 1), max_y);

    Vector3D inv_z_coords = system.GetOriginalCoordsMatrix().col(2).cwiseInverse();
    for (size_t x = min_x; x <= max_x; ++x) {
        for (size_t y = min_y; y <= max_y; ++y) {
            Vector2D point_frame_space = {static_cast<double>(x) + 0.5,
                                          static_cast<double>(y) + 0.5};
            Vector2D point_camera_space =
                TransformVectorToCameraSpace(point_frame_space, width, height);
            Vector3D b_local_coordinates = BarycentricSystem::TransformToBarycentric(
                inverse_matrix, coordinates, point_camera_space);
            if (!CheckIfInside(b_local_coordinates)) {
                continue;
            }
            Vector3D b_coordinate = system.ConvertToBarycentricCoordinates(point_camera_space);
            double z = system.GetNewCoordinates(b_coordinate).z();
            if (frame->GetZ(y, x) != 0 && z >= frame->GetZ(y, x)) {
                continue;
            }
            frame->SetZ(y, x, z);
            Vector3D new_b_coordinate = b_coordinate;
            double real_z_inv = (b_coordinate.dot(inv_z_coords));
            new_b_coordinate = (1 / real_z_inv) * new_b_coordinate.cwiseProduct(inv_z_coords);
            if (use_blinn_phong_) {
                frame->SetPixel(
                    y, x,
                    CalculateBlinnPhong(
                    system.GetTriangle().GetAmbientColor(b_coordinate),
                    system.GetTriangle().GetDiffuseColor(new_b_coordinate),
                    system.GetTriangle().GetSpecularColor(new_b_coordinate), desc,
                    world, new_b_coordinate, system.GetTriangle()));
            } else {
                Color color = system.GetColor(new_b_coordinate);
                frame->SetPixel(y, x, color);
            }
        }
    }
}

bool Rasterizer::DetermineSide(const Vector4D &plane, const Vector3D &point) {
    assert("Plane is not zero" && (!(plane.x() == 0 && plane.y() == 0 && plane.z() == 0)));
    Vector3D point_on_plane = plane.topLeftCorner<3, 1>() * (-plane.w());
    Vector3D point_vector = point - point_on_plane;
    double scalar_prod = plane.topLeftCorner<3, 1>().dot(point_vector);
    return scalar_prod > 0;
}

Vector3D Rasterizer::PlaneLineIntersection(const Vector4D &plane,
                                           const Vector3D &point1,
                                           const Vector3D &point2) {
    Vector3D result;
    Vector3D v = point2 - point1;
    double t = (-(plane.topLeftCorner<3, 1>().dot(point1) + plane.w())) /
               (plane.topLeftCorner<3, 1>().dot(v));
    result = point1 + t * v;
    return result;
}

bool Rasterizer::ClipOneTriangle(const Vector4D &plane, const Mat3D &triangle,
                                 std::list<Mat3D> *triangles) {
    int cnt = 0;
    Mat3D points_inside;
    Mat3D points_outside;
    for (int i = 0; i < 3; ++i) {
        bool is_inside = DetermineSide(plane, triangle.row(i));
        if (is_inside) {
            ++cnt;
            points_inside.row(cnt - 1) = triangle.row(i);
        } else {
            points_outside.row(i - cnt) = triangle. row(i);
        }
    }
    if (cnt == 3) {
        return false;
    }
    if (cnt == 0) {
        return true;
    }
    if (cnt == 1) {
        Vector3D intersection1 =
            PlaneLineIntersection(plane,points_outside.row(0),points_inside.row(0));
        Vector3D intersection2 =
            PlaneLineIntersection(plane,points_outside.row(1),points_inside.row(0));
        Mat3D result;
        result.row(0) = intersection1;
        result.row(1) = intersection2;
        result.row(2) = points_inside.row(0);
        triangles->push_back(result);
    }
    if (cnt == 2) {
        Vector3D intersection1 =
            PlaneLineIntersection(plane, points_outside.row(0), points_inside.row(0));
        Vector3D intersection2 =
            PlaneLineIntersection(plane, points_outside.row(0), points_inside.row(1));
        Mat3D result1;
        Mat3D result2;
        
        result1.row(0) = intersection1;
        result1.row(1) = points_inside.row(0);
        result1.row(2) = points_inside.row(1);

        result2.row(0) = intersection1;
        result2.row(1) = intersection2;
        result2.row(2) = points_inside.row(1);

        triangles->push_back(result1);
        triangles->push_back(result2);
    }
    return true;
}

void Rasterizer::ClipAllTriangles(const Vector4D &plane,
                                  std::list<Mat3D> *triangles) {
    if (triangles->empty()) {
        return;
    }
    auto it = triangles->end();
    --it;
    while (true) {
        bool is_clipped = ClipOneTriangle(plane, *it, triangles);
        if (is_clipped) {
            bool is_beginning = it == triangles->begin();
            auto new_it = triangles->erase(it);
            if (new_it == triangles->begin()) {
                break;
            }
            it = new_it;
            --it;
            continue;
        }
    }
}
Color Rasterizer::CalculateBlinnPhong(const Color  &ambient_color, const Color &diffuse_color,
                                      const Color &specular_color, const LightSourcesDescription &desc,
                                      const World &world, const Vector3D &b_coordinates,
                                      const Triangle &triangle) {
    Color result{0, 0, 0};
    Vector3D position = b_coordinates.transpose() * triangle.GetVertsCoords();
    Vector3D normal = triangle.GetNormal(b_coordinates);
    const Vector3D &real_normal = triangle.GetRealNormal();
    for (const auto &light: world.GetAmbientLightSources()) {
        result += ambient_color * light.GetIntensity();
    }
    Color diffuse =
        CalculateBlinnPhongDiffusion(diffuse_color, desc, world, position, normal, real_normal);
    Color specular =
        CalculateBlinnPhongSpecular(specular_color, desc, world, position, normal, real_normal);
    return result + diffuse + specular;
}

Color Rasterizer::CalculateBlinnPhongDiffusion(const Color &diffuse_color,
                                             const LightSourcesDescription &desc, const World &world,
                                             const Vector3D &coordinates, const Vector3D &normal,
                                             const Vector3D &real_normal) {
Color ans = {0, 0, 0};
for (int i = 0; i < world.GetPointLightSources().size(); ++i) {
Vector3D direction = (desc.point_light_coordinates_[i] - coordinates).eval().normalized();
if ((-(-1 + 2 * std::signbit(normal.dot(direction)))) < 0) {
continue;
}
const auto &light = world.GetPointLightSources()[i];
ans += light.GetColor() *
(light.GetIntencity((coordinates - desc.point_light_coordinates_[i]).norm()) *
std::abs(direction.dot(normal)));
}
ans.Normalize();
ans *= diffuse_color;

return ans;
}


Color Rasterizer::CalculateBlinnPhongSpecular(const Color &initial_color,
   const LightSourcesDescription &desc, const World &world,
   const Vector3D &coordinates, const Vector3D &normal,
   const Vector3D &real_normal) {
Color ans = {0, 0, 0};
constexpr int kM = 25;
for (int i = 0; i < world.GetPointLightSources().size(); ++i) {
Vector3D direction = (desc.point_light_coordinates_[i] - coordinates).eval().normalized();
if (-(-1 + 2 * std::signbit(normal.dot(direction))) < 0) {
continue;
}
Vector3D viewer = (-coordinates).normalized();
Vector3D halfway = (direction + viewer) / (direction + viewer).norm();
const auto &light = world.GetPointLightSources()[i];
ans += light.GetColor() *
(light.GetIntencity((coordinates - desc.point_light_coordinates_[i]).norm()) *
std::pow(std::abs(halfway.dot(normal)), kM));
}
ans.Normalize();
ans *= initial_color;

return ans;
}

Mat34D Rasterizer::MakeHomogeneousTransformationMatrix(const QuatD &rotation,
                                                     const Vector3D &offset) {

Mat34D transformation_matrix = Mat34D::Zero();
transformation_matrix.topLeftCorner<3, 3>() = rotation.toRotationMatrix();
transformation_matrix.col(3).topLeftCorner<3, 1>() = offset;
return transformation_matrix;
}
void Rasterizer::ApplyMatrix(const Mat34D &transformation_matrix, Triangle *vertices) {
    assert(vertices != nullptr);
    for (auto &ver : vertices->GetVerts()) {
        ver.coordinates = (transformation_matrix * ver.coordinates.GetHomogeneousCoordinates());
        ver.normal = transformation_matrix * ver.normal.GetHomogeneousCoordinates();
    }
}
}
