#pragma once


class Demo
{
public:
	Demo(const std::string& name) : m_Name(name) {}
	virtual ~Demo() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}

	virtual void Update(Eis::TimeStep) {}
	virtual void ImGuiRender() {}
	virtual void OnEvent(Eis::Event& e) {}

	const std::string& GetName() const { return m_Name; }

protected:
	std::string m_Name;
};