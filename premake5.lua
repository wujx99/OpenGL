workspace "OpenGL"
	architecture "x64"
	
	startproject "OpenGL"
	configurations
	{
		"Debug",
		"Release",
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "OpenGL/vendor/GLFW/include"
IncludeDir["Glad"] = "OpenGL/vendor/Glad/include"
IncludeDir["ImGui"] = "OpenGL/vendor/imgui"
IncludeDir["glm"] = "OpenGL/vendor/glm"


group "Dependencies"
	include "OpenGL/vendor/GLFW"
	include "OpenGL/vendor/Glad"
	include "OpenGL/vendor/imgui"

group ""

project "OpenGL"
	location "OpenGL"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
	}
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

	filter "system:windows"
		
		systemversion "latest"

		defines
		{
			"GLFW_INCLUDE_NONE",
		}

		


	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"



