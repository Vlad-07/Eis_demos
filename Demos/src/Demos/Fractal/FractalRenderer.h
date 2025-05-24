#pragma once

#include <Eis.h>


class FractalRenderer
{
	enum class FractalType : uint8_t
	{
		JULIASET = 0
	};

public:
	FractalRenderer() = default;
	~FractalRenderer() = default;

	void SetCanvasSize(uint32_t width, uint32_t height);

	void Compute(glm::vec2 constant, uint32_t maxIt);
	void Render() const;
	void OnEvent(Eis::Event& e);

private:
	static glm::vec2 ComputeNextIt(glm::vec2 current, glm::vec2 constant);
	static float ComputeIt(glm::vec2 n0, glm::vec2 constant, uint32_t maxIt);
	static glm::vec3 ColorPallette(float it, uint32_t maxIt);

private:
	Eis::Ref<Eis::Texture2D> m_Canvas;
	uint8_t* m_Buffer = nullptr;
	uint32_t m_BufSize = 0;
	float m_AspectRatio = 0.0f;
	FractalType m_Type = FractalType::JULIASET;

	glm::vec2 m_Constant{};
	uint32_t m_MaxIt = 0;

	static const uint8_t c_SamplesPerPixel = 4;
};