#include "World.h"

#include <Eis/Core/Log.h>
#include <Eis/Core/Core.h>
#include <Eis/Core/Random.h>


void World::Update()
{
	for (uint32_t y = 0; y < WorldSize.y; y++)
	for (uint32_t x = 0; x < WorldSize.y; x++)
	{
		switch (m_Mat[y][x])
		{
		case Element::NONE:
		case Element::STONE:
			break;

		case Element::SAND:
		{
			if (y == 0) break;

			if (m_Mat[y - 1][x] == Element::NONE)
			{
				m_Mat[y - 1][x] = Element::SAND;
				m_Mat[y][x] = Element::NONE;
				break;
			}

			if (x > 0)
			if (m_Mat[y - 1][x - 1] == Element::NONE)
			{
				m_Mat[y - 1][x - 1] = Element::SAND;
				m_Mat[y][x] = Element::NONE;
				break;
			}

			if (x < WorldSize.x - 1)
			if (m_Mat[y - 1][x + 1] == Element::NONE)
			{
				m_Mat[y - 1][x + 1] = Element::SAND;
				m_Mat[y][x] = Element::NONE;
				break;
			}

			break;
		}

		case Element::WATER:
		{
			if (y == 0) break;

			if (m_Mat[y - 1][x] == Element::NONE)
			{
				m_Mat[y - 1][x] = Element::WATER;
				m_Mat[y][x] = Element::NONE;
				break;
			}

			if (x > 0)
			if (m_Mat[y - 1][x - 1] == Element::NONE)
			{
				m_Mat[y - 1][x - 1] = Element::WATER;
				m_Mat[y][x] = Element::NONE;
				break;
			}

			if (x < WorldSize.x - 1)
			if (m_Mat[y - 1][x + 1] == Element::NONE)
			{
				m_Mat[y - 1][x + 1] = Element::WATER;
				m_Mat[y][x] = Element::NONE;
				break;
			}

			if (x > 0 && Eis::Random::UInt(0, 1))
			if (m_Mat[y][x - 1] == Element::NONE)
			{
				m_Mat[y][x - 1] = Element::WATER;
				m_Mat[y][x] = Element::NONE;
				break;
			}

			if (x < WorldSize.x - 1)
			if (m_Mat[y][x + 1] == Element::NONE)
			{
				m_Mat[y][x + 1] = Element::WATER;
				m_Mat[y][x] = Element::NONE;
				break;
			}

			break;
		}

		default:
			EIS_ASSERT(false, "Invalid element found!");
			break;
		}
	}
}

void World::Clear()
{
	for (uint32_t y = 0; y < WorldSize.y; y++)
	for (uint32_t x = 0; x < WorldSize.y; x++)
		m_Mat[y][x] = Element::NONE;
}