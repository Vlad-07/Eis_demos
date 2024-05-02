#include "World.h"

#include <Eis/Core/Log.h>
#include <Eis/Core/Core.h>
#include <Eis/Core/Random.h>


void World::Update()
{
	for (uint32_t y = 0; y < m_Mat.size(); y++)
	for (uint32_t x = 0; x < m_Mat[0].size(); x++)
	{
		const auto& elementProp = g_Elements[At(x, y)];

		if (elementProp.Movable)
		{
			if (y > 0 && elementProp.Priority < g_Elements[At(x, y - 1)].Priority)
				std::swap(m_Mat[y][x], m_Mat[y - 1][x]);
			if (Eis::Random::UInt(0, 1) && y > 0 && x > 0 && elementProp.Priority < g_Elements[At(x - 1, y - 1)].Priority) // Random hack
				std::swap(m_Mat[y][x], m_Mat[y - 1][x - 1]);
			if (y > 0 && x < m_Mat[0].size() - 1 && elementProp.Priority < g_Elements[At(x + 1, y - 1)].Priority)
				std::swap(m_Mat[y][x], m_Mat[y - 1][x + 1]);
		}

		if (elementProp.Liquid)
		{
			if (Eis::Random::UInt(0, 1) && x > 0 && elementProp.Priority < g_Elements[At(x - 1, y)].Priority)
				std::swap(m_Mat[y][x], m_Mat[y][x - 1]);
			if (x < m_Mat[0].size() - 1 && elementProp.Priority < g_Elements[At(x + 1, y)].Priority)
				std::swap(m_Mat[y][x], m_Mat[y][x + 1]);
		}
	}
}

void World::Clear()
{
	for (uint32_t y = 0; y < m_Mat.size(); y++)
	for (uint32_t x = 0; x < m_Mat[0].size(); x++)
		m_Mat[y][x] = ElementId::AIR;
}