#pragma once

#include <Eigen/Dense>
#include <memory>
#include "mesh.h"
namespace Renderer {

class AnyObject {
public:
    class Base;
    AnyObject() = default;

    AnyObject(const AnyObject &other) : inner_((other.inner_ ? other.inner_->Clone() : nullptr)) {
    }

    template <typename T>
    AnyObject(T &&object)
        : inner_(std::make_unique<Inner<std::remove_reference_t<T>>>(std::forward<T>(object))) {
    }
    AnyObject(AnyObject &&) noexcept = default;
    AnyObject &operator=(AnyObject &&) noexcept = default;

    AnyObject &operator=(const AnyObject &val) {
        return *this = AnyObject(val);
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
        virtual Iterable<MeshConstIterator> GetMeshes() const = 0;
        friend class AnyObject;

    private:
        virtual std::unique_ptr<Base> Clone() const = 0;
    };

private:
    template <typename T>
    class Inner : public Base {
    public:
        /*Inner(const T &value) : value_(value) {
        }*/
        Inner(T &&value) : value_(std::move(value)) {
        }
        Inner(const T &value) : value_(value) {
        }
        Iterable<MeshConstIterator> GetMeshes() const override {
            return value_.GetMeshes();
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
