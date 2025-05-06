#include "World.h"

#include <Eis/Core/Log.h>
#include <Eis/Core/Core.h>
#include <Eis/Core/Random.h>


void World::Update()
{
	EIS_PROFILE_FUNCTION();

	for (uint32_t y = 0; y < m_Mat.size(); y++)
	for (uint32_t x = 0; x < m_Mat[0].size(); x++)
	{
		const auto& elementProp = g_Elements[At(x, y)];

		if (elementProp.Movable && y > 0)
		{
			if (elementProp.Priority < g_Elements[At(x, y - 1)].Priority)
			{ std::swap(m_Mat[y][x], m_Mat[y - 1][x]); continue; }


			bool swap = Eis::Random::Bool();
			if (swap) goto _Second1;

		_First1:
			swap = false;
			if (x > 0 && elementProp.Priority < g_Elements[At(x - 1, y - 1)].Priority)
			{ std::swap(m_Mat[y][x], m_Mat[y - 1][x - 1]); continue; }
		_Second1:
			if (x < m_Mat[0].size() - 1 && elementProp.Priority < g_Elements[At(x + 1, y - 1)].Priority)
			{ std::swap(m_Mat[y][x], m_Mat[y - 1][x + 1]); continue; }
			if (swap) goto _First1;
		}

		if (elementProp.Liquid)
		{
			bool swap = Eis::Random::Bool();
			if (swap) goto _Second2;

		_First2:
			swap = false;
			if (x > 0 && elementProp.Priority < g_Elements[At(x - 1, y)].Priority)
			{ std::swap(m_Mat[y][x], m_Mat[y][x - 1]); continue; }
		_Second2:
			if (x < m_Mat[0].size() - 1 && elementProp.Priority < g_Elements[At(x + 1, y)].Priority)
			{ std::swap(m_Mat[y][x], m_Mat[y][x + 1]); continue; }
			if (swap) goto _First2;
		}
	}
}

void World::Clear()
{
	EIS_PROFILE_FUNCTION();

	for (uint32_t y = 0; y < m_Mat.size(); y++)
	for (uint32_t x = 0; x < m_Mat[0].size(); x++)
		m_Mat[y][x].Reset();
}