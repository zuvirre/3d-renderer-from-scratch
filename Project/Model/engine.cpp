#include "engine.h"
#include "OBJLoader/objloader.h"
#include "Eigen/src/Core/Matrix.h"
#include "primitive.h"
#include "frame.h"
#include "utility.h"
#include "object.h"
#include <algorithm>
#include <cmath>
#include <memory>

namespace kernel {
Engine::Engine(int width, int height)
    : width_(width),
      height_(height),
      current_frame_(width, height),
      update_port_([this]() -> PortReturnType { return this->current_frame_; }),
      world_(width, height) {
}
void Engine::InitializeSphereEnv() {
    selected_ = &world_.GetCameraHolder();
    world_.AddAmbientLight();
    world_.AddPointLight({0,0,4});
    world_.SetCameraPosition({0, 0, 4});
    world_.AddObject(Renderer::Sphere{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}});
    world_.AddObject(Renderer::Sphere{{1, 1, 0}, {1, 1, 0}, {1, 1, 0}, 0.5}, Vector3D{1, 1, 1});
    world_.AddObject(Renderer::Sphere{{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, 1.3, 1200},
                     Vector3D{-1, 1, 1});
    world_.AddObject(Renderer::Sphere{{0, 1, 1}, {0, 1, 1}, {0, 1, 1}, 1.3, 1200},
                     Vector3D{-1, 1, 7});
    Update();
}

void Engine::InitializeUserEnv() {
    selected_ = &world_.GetCameraHolder();
    ReadAllFromDirectory("Models", &world_);
    world_.AddAmbientLight();
    world_.AddPointLight({0, 0, 4});
    world_.SetCameraPosition({0, 0, 4});
    Update();
}

const Renderer::Frame &Engine::GetCurrentFrame() {
    return current_frame_;
}

void Engine::Subscribe(observer::CObserver<const Renderer::Frame>* obs) {
    update_port_.subscribe(obs);
}
void Engine::Update() {
    Frame frame(*rasterizer_.Draw(world_, width_, height_));
    current_frame_ = std::move(frame);
    update_port_.notify();
}
Engine::HolderPointer Engine::GetSelected() {
    return selected_;
}

void Engine::TiltUp(double shift, HolderPointer selected) {
    QuatD rotation = selected->GetAngle();
    QuatD new_quatenion =
        QuatD(rotation.matrix() * Eigen::AngleAxisd(shift, Vector3D{1, 0, 0}).matrix());
    new_quatenion.normalize();
    selected->SetAngle(new_quatenion);
}
void Engine::TiltDown(double shift, HolderPointer selected) {
    QuatD rotation = selected->GetAngle();
    QuatD new_quatenion =
        QuatD(rotation.matrix() * Eigen::AngleAxisd(-shift, Vector3D{1, 0, 0}).matrix());
    new_quatenion.normalize();
    selected->SetAngle(new_quatenion);
}
void Engine::TiltRight(double shift, HolderPointer selected) {
    QuatD rotation = selected->GetAngle();
    QuatD new_quatenion = QuatD(
        rotation.matrix() * Eigen::AngleAxisd(-shift, selected->GetDefaultNormal()).matrix());
    new_quatenion.normalize();
    selected->SetAngle(new_quatenion);
}
void Engine::TiltLeft(double shift, HolderPointer selected) {

    QuatD rotation = selected->GetAngle();
    QuatD new_quatenion = QuatD(
        rotation.matrix() * Eigen::AngleAxisd(shift, selected->GetDefaultNormal()).matrix());
    new_quatenion.normalize();
    selected->SetAngle(new_quatenion);
}

void Engine::MoveUp(double shift, HolderPointer selected) {

    QuatD rotation = selected->GetAngle();
    Vector3D normal = selected->GetNormal();
    selected->SetCoordinates(selected->GetCoordinates() + normal * shift);
}
void Engine::MoveDown(double shift, HolderPointer selected) {
    QuatD rotation = selected->GetAngle();
    Vector3D normal = selected->GetNormal();
    selected->SetCoordinates(selected->GetCoordinates() - normal * shift);
}
void Engine::MoveRight(double shift, HolderPointer selected) {
    QuatD rotation = selected->GetAngle();
    Vector3D direction = selected->GetDirection();
    Vector3D normal = selected->GetNormal();
    Vector3D axis_of_rotation = direction.cross(normal);
    selected->SetCoordinates(selected->GetCoordinates() + axis_of_rotation * shift);
}
void Engine::MoveLeft(double shift, HolderPointer selected) {
    QuatD rotation = selected->GetAngle();
    Vector3D direction = selected->GetDirection();
    Vector3D normal = selected->GetNormal();
    Vector3D axis_of_rotation = direction.cross(normal);
    selected->SetCoordinates(selected->GetCoordinates() - axis_of_rotation * shift);
}
void Engine::MoveForward(double shift, HolderPointer selected) {
    QuatD rotation = selected->GetAngle();
    Vector3D direction = selected->GetDirection();
    selected->SetCoordinates(selected->GetCoordinates() + direction * shift);
}
void Engine::MoveBackward(double shift, HolderPointer selected) {
    QuatD rotation = selected->GetAngle();
    Vector3D direction = selected->GetDirection();
    selected->SetCoordinates(selected->GetCoordinates() - direction * shift);
}
void Engine::SwitchLightingModel() {
    rasterizer_.SwitchLightModel();
}
void Engine::ToggleLightMarkers() {
    rasterizer_.ToggleLightMarkers();
}
void Engine::SwitchObject() {
    if (world_.GetObjects().empty()) {
        return;
    }
    if (is_changing_objects_) {
        ++selected_obj_;
        if (selected_obj_ == world_.GetObjects().size()) {
            selected_obj_ = 0;
        }
    } else {
        ResetEditingStates();
        is_changing_objects_ = true;
    }
    selected_ = &world_.GetObjects()[selected_obj_];
}
void Engine::SwitchCamera() {
    ResetEditingStates();
    is_changing_camera_ = true;
    selected_ = &world_.GetCameraHolder();
}
void Engine::SwitchLight() {
    if (world_.GetPointLightSources().empty()) {
        return;
    }
    if (is_changing_point_lights_) {
        ++selected_point_light_;
        if (selected_point_light_ == world_.GetPointLightSources().size()) {
            selected_point_light_ = 0;
        }
    } else {
        ResetEditingStates();
        is_changing_point_lights_ = true;
    }
    selected_ = &world_.GetPointLightSources()[selected_point_light_];
}
void Engine::ResetEditingStates() {
    is_changing_objects_ = false;
    is_changing_camera_ = false;
    is_changing_point_lights_ = false;
}
void Engine::AddPointLight() {
    world_.AddPointLight(world_.GetCameraPosition());
}
void Engine::ChangeSelectedLight(double c_attenuation, double l_attenuation,
                                        double q_attenuation) {
    auto& lights = world_.GetLights();
    lights[selected_point_light_].SetLight(c_attenuation, l_attenuation, q_attenuation);
}
void Engine::ToggleSelectedLight() {
    auto& lights = world_.GetLights();
    lights[selected_point_light_].Switch ();
}
}
