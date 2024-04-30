#pragma once

#include <array>
#include <glm/glm.hpp>

#include "Element.h"


static constexpr glm::ivec2 WorldSize(100);

class World
{
public:
	World() { Clear(); }
	~World() = default;

	void Update();
	void Clear();


	void SetElement(glm::ivec2 pos, Element e) { m_Mat[pos.y][pos.x] = e; }
	void SetElement(uint32_t x, uint32_t y, Element e) { m_Mat[y][x] = e; }

	Element At(uint32_t x, uint32_t y) const { return m_Mat[y][x]; }
	Element At(glm::ivec2 pos) const { return m_Mat[pos.y][pos.x]; }

	glm::vec2 GetSize() const { return WorldSize; }

private:
	std::array<std::array<Element, WorldSize.x>, WorldSize.y> m_Mat;
};