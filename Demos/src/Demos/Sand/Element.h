#pragma once

#include <array>
#include <glm/glm.hpp>


struct ElementParams
{
public:
	enum ID : uint8_t
	{
		AIR = 0, SAND = 1, WATER = 2, STONE = 3
	};

public:
	ElementParams() : Id(ID::AIR), Temp(0.0f) {}
	ElementParams(ID id) : Id(id), Temp(0.0f) {}
	~ElementParams() = default;

	void Reset() { Id = ID::AIR; Temp = 0.0f; }
	operator uint8_t() const { return Id; }
	bool operator==(const ID& other) const { return Id == other; }
	bool operator!=(const ID& other) const { return Id != other; }
	bool operator==(const ElementParams& other) const { return Id == other.Id; }
	bool operator!=(const ElementParams& other) const { return Id != other.Id; }

	ID Id;
	float Temp;
};

struct ElementProps
{
public:
	constexpr ElementProps(const char* name, bool movable, bool liquid, uint8_t priority, glm::vec4 color)
		: Name(name), Movable(movable), Liquid(liquid), Priority(priority), Color(color) {}

	bool operator==(const ElementProps& other) const
	{ return strcmp(Name, other.Name) == 0; }


	const char* const Name;
	bool Movable;
	bool Liquid;
	uint8_t Priority;
	glm::vec4 Color;
};

constexpr std::array<const ElementProps, 4> g_Elements
												= { ElementProps("Air",   false, false, 255, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f)), // Air
													ElementProps("Sand",  true,  false, 1,   glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)), // Sand
													ElementProps("Water", true,  true,  2,   glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)), // Water
													ElementProps("Stone", false, false, 0,   glm::vec4(0.6f, 0.6f, 0.6f, 1.0f))  // Stone
												};