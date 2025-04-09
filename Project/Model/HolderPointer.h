#pragma once

#include <memory>
#include "mesh.h"

namespace Renderer {
class AnyConstHolderPointer {
public:
    class Base;
    AnyConstHolderPointer() = default;
    AnyConstHolderPointer(const AnyConstHolderPointer &other)
        : inner_((other.inner_ ? other.inner_->Clone() : nullptr)) {
    }

    template <typename T>
    AnyConstHolderPointer(T &&object)
        : inner_(std::make_unique<Inner<std::remove_reference_t<T>>>(std::forward<T>(object))) {
    }

    AnyConstHolderPointer(AnyConstHolderPointer &&) noexcept = default;

    AnyConstHolderPointer &operator=(AnyConstHolderPointer &&) noexcept = default;

    AnyConstHolderPointer &operator=(const AnyConstHolderPointer &val) {
        return *this = AnyConstHolderPointer(val);
    }

    const Base *operator->() const {
        return inner_.get();
    }

    Base *operator->() {
        return inner_.get();
    }

    class Base {
    public:
        virtual ~Base() = default;
        using MeshConstIterator = std::vector<Mesh>::const_iterator;
        virtual const Vector3D &GetCoordinates() const = 0;
        virtual const QuatD GetAngle() const = 0;

        virtual Vector3D GetDirection() const = 0;
        virtual Vector3D GetNormal() const = 0;
        virtual Vector3D GetDefaultDirection() const = 0;
        virtual Vector3D GetDefaultNormal() const = 0;
        friend class AnyConstHolderPointer;

    private:
        virtual std::unique_ptr<Base> Clone() const = 0;
    };

private:
    template <typename T>
    class Inner : public InnerBase {
    public:
        const Vector3d &GetCoordinates() const override {
            return value_->GetCoordinates();
        }
        const Quaterniond &GetAngle() const override {
            return value_->GetAngle();
        }

        Vector3d GetDirection() const override {
            return value_->GetDirection();
        };
        Vector3d GetNormal() const override {
            return value_->GetNormal();
        };
        Vector3d GetDefaultDirection() const override {
            return value_->GetDefaultDirection();
        }
        Vector3d GetDefaultNormal() const override {
            return value_->GetDefaultNormal();
        }
        Inner(T &&value) : value_(std::move(value)) {
        }
        Inner(const T &value) : value_(value) {
        }

    private:
        std::unique_ptr<InnerBase> Clone() const override {
            return std::make_unique<Inner>(value_);
        }
        T value_;
    };

    std::unique_ptr<Base> inner_;
};
class AnyHolderPointer {
public:
    class Base;
    AnyHolderPointer() = default;

    AnyHolderPointer(const AnyHolderPointer &other)
        : inner_((other.inner_ ? other.inner_->Clone() : nullptr)) {
    }
    template <typename T>
    AnyHolderPointer(T *&&object)
        : inner_(std::make_unique<Inner<std::remove_cv_t<std::remove_reference_t<T>> *>>(
              std::forward<T *>(object))) {
    }
    AnyHolderPointer(AnyHolderPointer &&) noexcept = default;
    AnyHolderPointer &operator=(AnyHolderPointer &&) noexcept = default;

    AnyHolderPointer &operator=(const AnyHolderPointer &val) {
        return *this = AnyHolderPointer(val);
    }

    const Base *operator->() const {
        return inner_.get();
    }
    Base *operator->() {
        return inner_.get();
    }

    class Base {
    public:
        virtual ~Base() = default;
        using MeshConstIterator = std::vector<Mesh>::const_iterator;
        virtual const Vector3D &GetCoordinates() const = 0;
        virtual const QuatD &GetAngle() const = 0;

        virtual void SetCoordinates(const Vector3D &) = 0;
        virtual void SetAngle(const QuatD &) = 0;

        virtual Vector3D GetDirection() const = 0;
        virtual Vector3D GetNormal() const = 0;
        virtual Vector3D GetDefaultDirection() const = 0;
        virtual Vector3D GetDefaultNormal() const = 0;
        friend class AnyHolderPointer;

    private:
        virtual std::unique_ptr<Base> Clone() const = 0;
    };

private:
    template <typename T>
    class Inner : public Base {
    public:
        const Vector3D &GetCoordinates() const override {
            return value_->GetCoordinates();
        }
        const QuatD &GetAngle() const override {
            return value_->GetAngle();
        }
        void SetCoordinates(const Vector3D &vec) override {
            value_->SetCoordinates(vec);
        };
        void SetAngle(const QuatD &angle) override {
            value_->SetAngle(angle);
        };

        Vector3d GetDirection() const override {
            return value_->GetDirection();
        };
        Vector3d GetNormal() const override {
            return value_->GetNormal();
        };
        Vector3d GetDefaultDirection() const override {
            return value_->GetDefaultDirection();
        }
        Vector3d GetDefaultNormal() const override {
            return value_->GetDefaultNormal();
        }
        Inner(T &&value) : value_(std::move(value)) {
        }
        Inner(const T &value) : value_(value) {
        }

    private:
        std::unique_ptr<Base> Clone() const override {
            return std::make_unique<Inner>(value_);
        }
        T value_;
    };

    std::unique_ptr<Base> inner_;
};

}
