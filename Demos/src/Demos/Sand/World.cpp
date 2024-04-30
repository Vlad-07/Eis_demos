#include "World.h"

#include <Eis/Core/Log.h>
#include <Eis/Core/Core.h>
#include <Eis/Core/Random.h>


void World::Update()
{
	for (uint32_t y = 0; y < WorldSize.y; y++)
	for (uint32_t x = 0; x < WorldSize.y; x++)
	{
		const auto& elementProp = Elements[(int8_t)At(x, y)];

		if (elementProp.Movable)
		{
			if (y > 0 && elementProp.Priority < Elements[(int8_t)At(x, y - 1)].Priority)
				std::swap(m_Mat[y][x], m_Mat[y - 1][x]);
			if (Eis::Random::UInt(0, 1) && y > 0 && x > 0 && elementProp.Priority < Elements[(int8_t)At(x - 1, y - 1)].Priority) // Random hack
				std::swap(m_Mat[y][x], m_Mat[y - 1][x - 1]);
			if (y > 0 && x < WorldSize.x - 1 && elementProp.Priority < Elements[(int8_t)At(x + 1, y - 1)].Priority)
				std::swap(m_Mat[y][x], m_Mat[y - 1][x + 1]);
		}

		if (elementProp.Liquid)
		{
			if (Eis::Random::UInt(0, 1) && x > 0 && elementProp.Priority < Elements[(int8_t)At(x - 1, y)].Priority)
				std::swap(m_Mat[y][x], m_Mat[y][x - 1]);
			if (x < WorldSize.x - 1 && elementProp.Priority < Elements[(int8_t)At(x + 1, y)].Priority)
				std::swap(m_Mat[y][x], m_Mat[y][x + 1]);
		}
	}
}

void World::Clear()
{
	for (uint32_t y = 0; y < WorldSize.y; y++)
	for (uint32_t x = 0; x < WorldSize.y; x++)
		m_Mat[y][x] = Element::AIR;
}