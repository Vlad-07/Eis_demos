#pragma once

#include <array>
#include <glm/glm.hpp>

// TODO: better element system
// property inheriting(?) system, avoiding code duplication in update func
// element priority (eg. sand replaces water)
// static color for optimized rendering

enum class Element : uint8_t
{
	AIR = 0, SAND, WATER, STONE
};

struct ElementProps
{
public:
	constexpr ElementProps(bool movable = false, bool liquid = false, uint8_t priority = 0, glm::vec3 color = glm::vec3(0))
		: Movable(movable), Liquid(liquid), Priority(priority), Color(color) {}

	bool Movable;
	bool Liquid;
	uint8_t Priority;
	glm::vec3 Color;
};

constexpr std::array<const ElementProps, 4> Elements
												= { ElementProps(false, false, 255, glm::vec3(0.2f)),             // Air
													ElementProps(true,  false, 1,   glm::vec3(1.0f, 1.0f, 0.0f)), // Sand
													ElementProps(true,  true,  2,   glm::vec3(0.0f, 0.0f, 1.0f)), // Water
													ElementProps(false, false, 0,   glm::vec3(0.6f)) };           // Stone