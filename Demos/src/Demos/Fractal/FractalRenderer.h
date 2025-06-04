#pragma once

#include <Eis.h>


class FractalRenderer
{
	enum class FractalType : uint8_t
	{
		JULIASET = 0
	};

public:
	FractalRenderer(const Eis::OrthoCameraController& cam) : m_SceneCam(cam), m_WorkGroup(35) {}
	~FractalRenderer() = default;

	void SetCanvasSize(uint32_t width, uint32_t height);

	void Compute(glm::vec2 constant, uint32_t maxIt);
	void Render() const;
	void OnEvent(Eis::Event& e);

private:
	static glm::dvec2 ComputeNextIt(glm::dvec2 current, glm::dvec2 constant);
	static float ComputeIt(glm::dvec2 n0, glm::dvec2 constant, uint32_t maxIt);
	static glm::u8vec3 ColorPallette(float it, uint32_t maxIt);
	static void ComputeToBuffer(uint8_t* buffer, glm::uvec2 pixelPos, glm::dvec2 pos, glm::dvec2 constant, uint32_t maxIt, double scale, glm::uvec2 size);
	static void ComputeRegion(uint8_t* buffer, uint32_t startCol, uint32_t colNr, glm::dvec2 pos, glm::dvec2 constant, uint32_t maxIt, double scale, glm::uvec2 size);

private:
	Eis::WorkGroup m_WorkGroup;

	Eis::Ref<Eis::Texture2D> m_Canvas;
	uint8_t* m_Buffer = nullptr;
	uint32_t m_BufSize = 0;

	FractalType m_Type = FractalType::JULIASET;

	glm::dvec2 m_Constant{};
	uint32_t m_MaxIt = 0;

	const Eis::OrthoCameraController& m_SceneCam;

	static const uint8_t c_SamplesPerPixel = 2;
};