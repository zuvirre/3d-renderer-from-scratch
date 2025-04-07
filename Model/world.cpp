#include "world.h"


namespace Renderer {
World::World(int width, int height) {
    Camera curr(width, height);
    cameras_.push_back(curr);
}

void World::AddObject(AnyObject object) {
    objects_.emplace_back(std::move(object), World::GetOrigin(),
                          Eigen::AngleAxis<double>(0,Vector3D::UnitX()));
}

void World::AddObject(AnyObject object, const Vector3D &global_coordinates) {
    objects_.emplace_back(std::move(object), World::GetOrigin(),
                          Eigen::AngleAxis<double>(0, Vector3D::UnitX()));                          
}

void World::AddAmbientLight() {
    ambient_lights_.emplace_back();
}

void World::AddPointLight(const Vector3D &coordinates) {
    point_lights_.emplace_back(coordinates);
}

Vector3D World::GetOrigin() {
    return {0,0,0};
}

const QuatD &World::GetCameraRotation() const {
    return cameras_[current_camera_ind_].GetAngle();
}

const  Vector3D &World::GetCameraPosition() const {
    return cameras_[current_camera_ind_].GetCoordinates();
}

const Camera &World::GetCamera() const {
    return cameras_[current_camera_ind_];
}

const std::vector<AmbientLight> &World::GetAmbientLightSources() const {
    return ambient_lights_;
}

const std::vector<World::TLightHolder<PointLight>> &World::GetPointLightSources() const {
    return point_lights_;
}
std::vector<World::TLightHolder<PointLight>> &World::GetPointLightSources() {
    return point_lights_;
}
void World::SetCameraRotation(const QuatD &rotation) {
    cameras_[current_camera_ind_].SetAngle(rotation);
}
void World::SetCameraPosition(const Vector3D &position) {
    cameras_[current_camera_ind_].SetCoordinates(position);
}

const Vector3D &World::CameraHolder::GetCoordinates() const {
    return coordinates_;
}
const QuatD &World::CameraHolder::GetAngle() const {
    return rotation_;
}
void World::CameraHolder::SetCoordinates(const Vector3D &coordinates) {
    coordinates_ = coordinates;
}
void World::CameraHolder::SetAngle(const QuatD &rotation) {
    rotation_ = rotation;
}
Vector3D World::CameraHolder::GetDirection() {
    return rotation_ * Vector3D{0, 0, -1};
}
Vector3D World::CameraHolder::GetNormal() {
    return rotation_ * Vector3D{0, 1, 0};
}

World::ObjectHolder::ObjectHolder(AnyObject &&obj, const Vector3D &coords,
                                  const Eigen::AngleAxisd &rotation)
    : AnyObject(std::move(obj)), coordinates_(coords), rotation_(rotation) {
}
World::ObjectHolder::ObjectHolder(const AnyObject &obj) : AnyObject(obj) {
}
World::ObjectHolder::ObjectHolder(AnyObject &&obj) noexcept : AnyObject(std::move(obj)) {
}
const Vector3D &World::ObjectHolder::GetCoordinates() const {
    return coordinates_;
}
const QuatD &World::ObjectHolder::GetAngle() const {
    return rotation_;
}
void World::ObjectHolder::SetCoordinates(const Vector3D &coordinates) {
    coordinates_ = coordinates;
}
void World::ObjectHolder::SetAngle(const QuatD &angle) {
    rotation_ = angle;
}
void World::ObjectHolder::SetAngle(const Eigen::AngleAxis<double> &angle) {
    rotation_ = angle;
}
World::CameraHolder::CameraHolder(const Camera &camera) : Camera(camera) {
}
std::vector<World::ObjectHolder> &World::GetObjects() {
    return objects_;
}

std::vector<World::TLightHolder<PointLight>> &World::GetLights() {
    return point_lights_;
}
World::CameraHolder &World::GetCameraHolder() {
    return cameras_[current_camera_ind_];
};
Vector3D World::ObjectHolder::GetDirection() const {
    return {0, 0, -1};
}
Vector3D World::ObjectHolder::GetNormal() const {
    return {0, 1, 0};
}
}
