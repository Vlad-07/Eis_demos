#include "FractalRenderer.h"


void FractalRenderer::SetCanvasSize(uint32_t width, uint32_t height)
{
	m_Canvas = Eis::Texture2D::Create(width, height);

	m_BufSize = width * height * 4;
	m_Buffer = new uint8_t[m_BufSize]();
	m_Canvas->SetData(m_Buffer, m_BufSize);

	for (uint32_t y = 0; y < height; y++)
		for (uint32_t x = 0; x < width; x++)
			m_Buffer[y * width * 4 + x * 4 + 3] = 255; // set the alpha channel
}

void FractalRenderer::Compute(glm::vec2 constant, uint32_t maxIt)
{
	//if (constant == m_Constant && maxIt == m_MaxIt)
	//	return;

	m_Constant = constant;
	m_MaxIt = maxIt;

	const float scale = m_SceneCam.GetZoom() / (float)m_Canvas->GetHeight();
	for (uint32_t x = 0, id = 0; x < m_Canvas->GetWidth(); x += m_Canvas->GetWidth() / m_WorkGroup.GetWorkerNr() + 1, id++)
		m_WorkGroup.SetTask(id, ComputeRegion, m_Buffer, x, m_Canvas->GetWidth() / m_WorkGroup.GetWorkerNr() + 1, m_SceneCam.GetPosition(), m_Constant, m_MaxIt, scale, glm::vec2(m_Canvas->GetWidth(), m_Canvas->GetHeight()));
	
	m_WorkGroup.Dispatch();
	m_WorkGroup.JoinAll();

	m_Canvas->SetData((void*)m_Buffer, m_BufSize);
}

void FractalRenderer::Render() const
{
	Eis::Renderer2D::DrawQuad(m_SceneCam.GetPosition(), glm::vec2(2.0f * m_SceneCam.GetAspectRatio(), 2.0f) * m_SceneCam.GetZoom(), m_Canvas);
}

void FractalRenderer::OnEvent(Eis::Event& e)
{
	Eis::EventDispatcher d(e);
	d.Dispatch<Eis::WindowResizeEvent>([this](Eis::WindowResizeEvent& e) -> bool
	{
		m_Canvas.reset();
		delete[] m_Buffer;

		m_BufSize = e.GetWidth() * e.GetHeight() * 4;
		m_Canvas = Eis::Texture2D::Create(e.GetWidth(), e.GetHeight());
		m_Buffer = new uint8_t[m_BufSize]();

		for (uint32_t y = 0; y < e.GetHeight(); y++)
			for (uint32_t x = 0; x < e.GetWidth(); x++)
				m_Buffer[y * e.GetWidth() * 4 + x * 4 + 3] = 255; // set the alpha channel

		return false;
	});
}


glm::dvec2 FractalRenderer::ComputeNextIt(glm::dvec2 current, glm::dvec2 constant)
{
	// current ^ 2
	const double real = current.x * current.x - current.y * current.y;
	const double imag = 2.0 * current.x * current.y;

	return glm::dvec2(real, imag) + constant;
}

float FractalRenderer::ComputeIt(glm::dvec2 n0, glm::dvec2 constant, uint32_t maxIt)
{
	uint32_t it = 0;
	for (; glm::dot(n0, n0) < 4.0 && it < maxIt; it++)
		n0 = ComputeNextIt(n0, constant);

	const float mod = glm::sqrt(glm::dot(n0, n0));
	const float smoothIt = (float)it - glm::log2(glm::max(1.0f, glm::log2(mod)));
	return smoothIt;
}

glm::u8vec3 FractalRenderer::ColorPallette(float it, uint32_t maxIt)
{
	return glm::u8vec3(uint8_t(255.0f * it / (float)maxIt));
}

void FractalRenderer::ComputeToBuffer(uint8_t* buffer, glm::uvec2 pixelPos, glm::dvec2 pos, glm::dvec2 constant, uint32_t maxIt, double scale, glm::uvec2 size)
{
	const glm::dvec2 pixelCoords = glm::dvec2((double)pixelPos.x - size.x / 2.0, (double)pixelPos.y - size.y / 2.0) * scale + pos;

	glm::u8vec3 res/*{};
	for (uint8_t i = 0; i < c_SamplesPerPixel; i++)
		res +*/= ColorPallette(ComputeIt(pixelCoords /*+ Eis::Random::Float(0.0f, scale)*/, constant, maxIt), maxIt);
//	res /= c_SamplesPerPixel;

	buffer[pixelPos.y * size.x * 4 + pixelPos.x * 4 + 0] = res.r;
	buffer[pixelPos.y * size.x * 4 + pixelPos.x * 4 + 1] = res.g;
	buffer[pixelPos.y * size.x * 4 + pixelPos.x * 4 + 2] = res.b;
}

void FractalRenderer::ComputeRegion(uint8_t* buffer, uint32_t startCol, uint32_t colNr, glm::dvec2 pos, glm::dvec2 constant, uint32_t maxIt, double scale, glm::uvec2 size)
{
	for (uint32_t col = startCol; col < startCol + colNr && col < size.y; col++)
		for (uint32_t x = 0; x < size.x; x++)
			ComputeToBuffer(buffer, { x, col }, pos, constant, maxIt, scale, size);
}