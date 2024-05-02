#pragma once

#include <array>
#include <glm/glm.hpp>


enum ElementId : uint8_t
{
	AIR = 0, SAND, WATER, STONE
};

struct ElementProps
{
public:
	constexpr ElementProps(const char* name, bool movable = false, bool liquid = false, uint8_t priority = 0, glm::vec3 color = glm::vec3(0))
		: Name(name), Movable(movable), Liquid(liquid), Priority(priority), Color(color) {}

	bool operator==(const ElementProps& other) const
	{ return strcmp(Name, other.Name) == 0; }


	const char* const Name;
	bool Movable;
	bool Liquid;
	uint8_t Priority;
	glm::vec3 Color;
};

constexpr std::array<const ElementProps, 4> g_Elements
												= { ElementProps("Air",   false, false, 255, glm::vec3(0.2f)),             // Air
													ElementProps("Sand",  true,  false, 1,   glm::vec3(1.0f, 1.0f, 0.0f)), // Sand
													ElementProps("Water", true,  true,  2,   glm::vec3(0.0f, 0.0f, 1.0f)), // Water
													ElementProps("Stone", false, false, 0,   glm::vec3(0.6f)) };           // Stone