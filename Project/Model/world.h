#pragma once

#include <vector>
#include <iostream>
#include "AnyObject.h"
#include "camera.h"
#include "HolderPointer.h"
#include "object.h"
#include "LightSources/PointLight.h"
#include "LightSources/AmbientLight.h"
#include "Eigen/src/Core/Matrix.h"

namespace Renderer {
class World {
public:
    template <typename T>
    class TLightHolder;
    class CameraHolder;
    class ObjectHolder;

    World(int width, int height);

    auto GetObjectsIterable() const {
        return Iterable(objects_.begin(), objects_.end());
    };

    void AddObject(AnyObject);
    void AddObject(AnyObject, const Vector3D &);
    void AddAmbientLight();
    void AddPointLight(const Vector3D &);
    const QuatD &GetCameraRotation() const;
    const Vector3D &GetCameraPosition() const;
    const Camera &GetCamera() const;
    CameraHolder &GetCameraHolder();
    const std::vector<AmbientLight> &GetAmbientLightSources() const;
    const std::vector<TLightHolder<PointLight>> &GetPointLightSources() const;
    std::vector<World::TLightHolder<PointLight>> &GetPointLightSources();
    void SetCameraRotation(const QuatD &);
    void SetCameraPosition(const Vector3D &);

    static Vector3D GetOrigin();
    std::vector<ObjectHolder> &GetObjects();
    std::vector<TLightHolder<PointLight>> &GetLights();

    template <typename T>
    class TLightHolder : public T {
    public:
        TLightHolder(const Vector3D &coordinates) : coordinates_(coordinates) {
            representing_triangle_.AddMesh({0, 10, 10}, {1, 1, 1}, {1, 1, 1});
            representing_triangle_.AddTriangle(GetSmallTriangle());
        }
        TLightHolder(T &&obj, const Vector3D &coordinates, const Eigen::AngleAxisd &angle)
            : T(obj), coordinates_(coordinates), rotation_(angle) {
            representing_triangle_.AddMesh({0, 10, 10}, {1, 1, 1}, {1, 1, 1});
            representing_triangle_.AddTriangle(GetSmallTriangle());
        }
        TLightHolder(const T &obj) : T(obj) {
        }
        TLightHolder(T &&obj) noexcept : T(obj) {
        }
        const Mesh &GetRepresentingMesh() const {
            return *(representing_triangle_.GetMeshes().begin());
        }
        const Vector3D &GetCoordinates() const {
            return coordinates_;
        }
        const QuatD &GetAngle() const {
            return rotation_;
        }

        void SetCoordinates(const Vector3D &coordinates) {
            coordinates_ = coordinates;
        }
        void SetAngle(const QuatD &rotation) {
            rotation_ = rotation;
        }
        void SetAngle(const Eigen::AngleAxis<double> &rotation) {
            rotation_ = rotation;
        }
        Vector3D GetDirection() const {
            return {0, 0, -1};
        }
        Vector3D GetNormal() const {
            return {0, 1, 0};
        }
        Vector3D GetDefaultDirection() const {
            return {0, 0, -1};
        }
        Vector3D GetDefaultNormal() const {
            return {0, 1, 0};
        }

    private:
        using ColorFunction = const std::function<Color(const Triangle &, const Vector3D &)>;
        Mat3D GetSmallTriangle() const {
            Mat3D result;
            result << 0.05, 0.05, 0, -0.05, 0.05, 0, 0, -0.05, 0;
            return result;
        }

        Vector3D coordinates_ = World::GetOrigin();
        QuatD rotation_ = QuatD::Identity();
        Object representing_triangle_;
    };

    class CameraHolder : public Camera {
        public:
            CameraHolder(const Camera &);
            const Vector3D &GetCoordinates() const;
            const QuatD &GetAngle() const;
    
            void SetCoordinates(const Vector3D &);
            void SetAngle(const QuatD &);
    
            Vector3D GetDirection();
            Vector3D GetNormal();
            Vector3D GetDefaultDirection() const {
                return {0, 0, -1};
            }
            Vector3D GetDefaultNormal() const {
                return {0, 1, 0};
            }
    
        private:
            Vector3D coordinates_ = World::GetOrigin();
            QuatD rotation_;
        };

    class ObjectHolder : public AnyObject {
        public:
            ObjectHolder(AnyObject &&, const Vector3D &, const Eigen::AngleAxisd &);
            ObjectHolder(const AnyObject &);
            ObjectHolder(AnyObject &&) noexcept;
            const Vector3D &GetCoordinates() const;
            const QuatD &GetAngle() const;
    
            void SetCoordinates(const Vector3D &);
            void SetAngle(const QuatD &);
            void SetAngle(const Eigen::AngleAxis<double> &);
    
            Vector3D GetDirection() const;
            Vector3D GetNormal() const;
            Vector3D GetDefaultDirection() const {
                return {0, 0, -1};
            }
            Vector3D GetDefaultNormal() const {
                return {0, 1, 0};
            }
    
        private:
            Vector3D coordinates_ = World::GetOrigin();
            QuatD rotation_;
        };

private:
    std::vector<AmbientLight> ambient_lights_;
    std::vector<TLightHolder<PointLight>> point_lights_;
    std::vector<ObjectHolder> objects_;
    std::vector<CameraHolder> cameras_;
    size_t current_camera_ind_ = 0;
};
}
