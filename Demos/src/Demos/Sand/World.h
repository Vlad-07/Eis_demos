#pragma once

#include <array>
#include <glm/glm.hpp>


static constexpr glm::ivec2 WorldSize(100);

enum class Element : uint8_t
{
	NONE = 0, SAND, WATER, STONE
};

class World
{
public:
	World() { Clear(); }
	~World() = default;

	void Update();
	void Clear();


	void SetElement(glm::ivec2 pos, Element e) { m_Mat[pos.y][pos.x] = e; }

	const auto& GetElements() const { return m_Mat; }
	glm::vec2 GetSize() const { return WorldSize; }

private:
	std::array<std::array<Element, WorldSize.x>, WorldSize.y> m_Mat;
};