#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Element.h"


class World
{
public:
	World(glm::ivec2 size) : m_Mat(size.y, std::vector<ElementId>(size.x, ElementId::AIR)) {}
	~World() = default;

	void Update();
	void Clear();


	void SetElement(glm::ivec2 pos, ElementId e) { m_Mat[pos.y][pos.x] = e; }
	void SetElement(uint32_t x, uint32_t y, ElementId e) { m_Mat[y][x] = e; }

	ElementId At(uint32_t x, uint32_t y) const { return m_Mat[y][x]; }
	ElementId At(glm::ivec2 pos) const { return m_Mat[pos.y][pos.x]; }

private:
	std::vector<std::vector<ElementId>> m_Mat;
};