#pragma once

#include <glm/glm.hpp>
#include <Eis/Physics/PhysicsManager2D.h>


class Entity
{
public:
	// Box
	Entity(glm::vec2 pos, glm::vec2 size, glm::vec3 color, bool s = false) : m_Color{ color }
	{
		m_Rigidbody = Eis::CreateScope<Eis::Rigidbody2D>(pos, 0.0f, size, !s ? c_Desc : c_FloorDesc);
		Eis::PhysicsManager2D::RegisterBody(m_Rigidbody);
	}
	// Circle
	Entity(glm::vec2 pos, float radius, glm::vec3 color) : m_Color{ color }
	{
		m_Rigidbody = Eis::CreateScope<Eis::Rigidbody2D>(pos, radius, c_Desc);
		Eis::PhysicsManager2D::RegisterBody(m_Rigidbody);
	}
	Entity(Entity&& other) noexcept : m_Color{ other.m_Color } { m_Rigidbody = std::move(other.m_Rigidbody); }
	~Entity()
	{
		if (m_Rigidbody)
			Eis::PhysicsManager2D::RemoveBody(m_Rigidbody);
	}

	glm::vec2 GetPosition() const { return m_Rigidbody->GetPosition(); }
	glm::vec3 GetColor() const { return m_Color; }
	const Eis::Rigidbody2D& GetRB() const { return *m_Rigidbody; }

	void operator=(Entity&& other) noexcept
	{
		if (m_Rigidbody)
			Eis::PhysicsManager2D::RemoveBody(m_Rigidbody);
		m_Rigidbody = std::move(other.m_Rigidbody);
		m_Color = other.m_Color;
	}

private:
	glm::vec3 m_Color{};
	Eis::Scope<Eis::Rigidbody2D> m_Rigidbody;

private:
	static constexpr Eis::Rigidbody2D::Descriptor c_Desc{ false, 1.0f, 0.3f, 0.6f, 0.6f };
	static constexpr Eis::Rigidbody2D::Descriptor c_FloorDesc{ true, 1.0f, 0.3f, 0.6f, 0.6f };
};