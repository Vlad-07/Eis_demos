workspace "Demos"
	startproject "Demos"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Dependencies"
include "Eis/vendor/GLFW"
include "Eis/vendor/Glad"
include "Eis/vendor/imgui"
include "Eis/vendor/implot"
group ""

include "Eis"


project "Demos"
	location "Demos" 
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"Eis/src",
		"Eis/vendor/spdlog/include",
		"Eis/vendor/imgui",
		"Eis/vendor/implot",
		"Eis/vendor/glm",
		"Eis/vendor/GameNetworkingSockets/include"
	}

	links
	{
		"Eis"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"IMGUI_USER_CONFIG=\"Eis/ImGui/ImGuiConfig.h\""
	}

	filter "toolset:msc*"
		buildoptions { "/utf-8" }
		multiprocessorcompile "on"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "EIS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EIS_RELEASE"
		runtime "Release"
		optimize "on"
		linktimeoptimization "on"