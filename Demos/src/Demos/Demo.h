#pragma once


class Demo
{
public:
	Demo() = delete;
	Demo(const std::string& name) : m_Name(name) {}
	virtual ~Demo() = default;

	virtual void Attach() {}
	virtual void Detach() {}

	virtual void Update(Eis::TimeStep) = 0;
	virtual void Render() {}
	virtual void ImGuiRender() {}

	virtual void OnEvent(Eis::Event& e) = 0;

	const std::string& GetName() const { return m_Name; }

protected:
	std::string m_Name;
};