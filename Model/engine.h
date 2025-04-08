#pragma once

#include "rasterizer.h"
#include "../Observer/Observer.h"
#include "frame.h"
#include "utility.h"
#include "Eigen/src/Core/Matrix.h"
#include "primitive.h"
#include "object.h"
#include "OBJ-Loader/Source/OBJ_Loader.h"
#include <algorithm>
#include <cmath>
#include <memory>

namespace kernel {
class Engine {
public:
    using World = Renderer::World;
    using Frame = Renderer::Frame;
    using Color = Renderer::Color;
    using HolderPointer = Renderer::AnyHolderPointer;
    using QuatD = Eigen::Quaterniond;
    using Vector3D = Eigen::Vector3d;
    
    Engine(int width, int height);
    void InitializeSphereEnv();
    void InitializeUserEnv();
    HolderPointer GetSelected();
    void TiltUp(double shift, HolderPointer selected);
    void TiltDown(double shift, HolderPointer selected);
    void TiltLeft(double shift, HolderPointer selected);
    void TiltRight(double shift, HolderPointer selected);

    void MoveUp(double shift, HolderPointer selected);
    void MoveDown(double shift, HolderPointer selected);
    void MoveLeft(double shift, HolderPointer selected);
    void MoveRight(double shift, HolderPointer selected);
    void MoveForward(double shift, HolderPointer selected);
    void MoveBackward(double shift, HolderPointer selected);

    void Subscribe(observer::CObserver<const Frame>* obs);
    void Update();
    void SwitchLightingModel();
    void ToggleLightMarkers();
    void SwitchObject();
    void SwitchCamera();
    void SwitchLight();

    void AddPointLight();
    void ChangeSelectedLight(double c_attenuation, double l_attenuation, double q_attenuation);
    void ToggleSelectedLight();

private:
    void ResetEditingStates();
    using PortReturnType = observer::CObservable<const Frame>::CReturn;

    const Frame& GetCurrentFrame();
    Renderer::Rasterizer rasterizer_;
    World world_;
    observer::CObservable<const Frame> update_port_;
    Frame current_frame_;
    int width_;
    int height_;
    std::vector<Color> test_colors_ = {{0.5, 0.5, 0.5}, {0, 0, 0}};
    bool is_changing_objects_ = false;
    bool is_changing_camera_ = true;
    bool is_changing_point_lights_ = false;
    size_t selected_obj_ = 0;
    size_t selected_point_light_ = 0;
    HolderPointer selected_;
};
}