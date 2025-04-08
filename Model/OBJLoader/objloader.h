#pragma once
#include "object.h"
#include "world.h"

namespace kernel {
using Object = Renderer::Object;
void ReadAllFromDirectory(std::string path, Renderer::World *world);
Object ReadObject(const std::string &path);
}
