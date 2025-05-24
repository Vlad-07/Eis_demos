#include "FractalRenderer.h"


void FractalRenderer::SetCanvasSize(uint32_t width, uint32_t height)
{
	m_Canvas = Eis::Texture2D::Create(width, height);

	m_AspectRatio = (float)width / (float)height;
	m_BufSize = width * height * 4;
	m_Buffer = new uint8_t[m_BufSize]();
	m_Canvas->SetData(m_Buffer, m_BufSize);
}

void FractalRenderer::Compute(glm::vec2 constant, uint32_t maxIt)
{
	if (constant == m_Constant && maxIt == m_MaxIt)
		return;

	m_Constant = constant;
	m_MaxIt = maxIt;

	const float scale = 2.1f / (float)m_Canvas->GetHeight();
	for (uint32_t y = 0; y < m_Canvas->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_Canvas->GetWidth(); x++)
		{
			glm::vec2 pixelCoords = glm::vec2((float)x - m_Canvas->GetWidth() / 2.0f, (float)y - m_Canvas->GetHeight() / 2.0f) * scale;

			glm::vec3 res{};
			for (uint8_t i = 0; i < c_SamplesPerPixel; i++)
				res += ColorPallette(ComputeIt(pixelCoords + Eis::Random::Float(0.0f, scale), m_Constant, m_MaxIt), m_MaxIt);
			res /= c_SamplesPerPixel;

			try
			{
				m_Buffer[y * m_Canvas->GetWidth() * 4 + x * 4 + 0] = (uint8_t)res.r;
				m_Buffer[y * m_Canvas->GetWidth() * 4 + x * 4 + 1] = (uint8_t)res.g;
				m_Buffer[y * m_Canvas->GetWidth() * 4 + x * 4 + 2] = (uint8_t)res.b;
				m_Buffer[y * m_Canvas->GetWidth() * 4 + x * 4 + 3] = 255;
			}
			catch (std::exception)
			{
				EIS_ERROR("Buffer write fault!");
			}
		}
	}

	m_Canvas->SetData((void*)m_Buffer, m_BufSize);
}

void FractalRenderer::Render() const
{
	Eis::Renderer2D::DrawQuad(glm::vec2(0.0f), glm::vec2(4.0f * m_AspectRatio, 4.0f), m_Canvas); // 4.0f is 2 * the cam zoom (locked at 2.0f)
}

void FractalRenderer::OnEvent(Eis::Event& e)
{
	Eis::EventDispatcher d(e);
	d.Dispatch<Eis::WindowResizeEvent>([this](Eis::WindowResizeEvent& e) -> bool
	{
		m_Canvas.reset();
		delete[] m_Buffer;

		m_Canvas = Eis::Texture2D::Create(e.GetWidth(), e.GetHeight());
		m_Buffer = new uint8_t[m_BufSize]();

		m_BufSize = e.GetWidth() * e.GetHeight() * 4;
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();

		return false;
	});
}


glm::vec2 FractalRenderer::ComputeNextIt(glm::vec2 current, glm::vec2 constant)
{
	// current ^ 2
	const float real = current.x * current.x - current.y * current.y;
	const float imag = 2.0f * current.x * current.y;

	return glm::vec2(real, imag) + constant;
}

float FractalRenderer::ComputeIt(glm::vec2 n0, glm::vec2 constant, uint32_t maxIt)
{
	int it = 0;
	glm::vec2 n = n0;
	while (glm::dot(n, n) < 4.0f && it < maxIt)
	{
		n = ComputeNextIt(n, constant);
		it++;
	}

	const float mod = glm::sqrt(glm::dot(n, n));
	const float smoothIt = (float)it - glm::log2(glm::max(1.0f, glm::log2(mod)));
	return smoothIt;
}

glm::vec3 FractalRenderer::ColorPallette(float it, uint32_t maxIt)
{
	return glm::vec3(255.0f * it / (float)maxIt);
}