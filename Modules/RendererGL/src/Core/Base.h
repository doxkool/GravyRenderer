#pragma once

#include "Core/Defines.h"

#include <iostream>
#include <array>
#include <queue>
#include <set>
#include <algorithm>
#include <bitset>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_map>
#include <map>
#include <variant>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>

constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

enum ColorSpace
{
	RGB,
	SRGB
};

struct Transform_t
{
	glm::vec3 Position  = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation  = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale     = {1.0f, 1.0f, 1.0f};
};