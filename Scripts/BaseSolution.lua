workspace "Waffle2D"
	configurations { "Debug", "Release"}
	platforms "x64"
	systemversion "10.0.16299.0"
	staticruntime "on"
	location "../"

filter { "platforms:x64" }
	includedirs 
	{
		"../Source"
	}

filter {"configurations:Debug"}
	debugdir  "%{wks.location}"
	libdirs
	{
	}
	links
	{
	}

filter {"configurations:Release"}
	debugdir  "%{wks.location}"
	libdirs
	{
	}
	links
	{
	}


project "Waffle2D"
	kind "StaticLib"
	language "C++"
	location "../Temp/VSFiles"
	targetdir "../Build/%{cfg.platform}/%{cfg.buildcfg}"
	files
	{
		"../Source/**.h",
		"../Source/**.cpp",
		"../Depen/glad/src/**.c"
	}
	libdirs
	{
		"../Depen/OpenAL/libs/Win64"
	}
	links
	{
		"OpenGL32",
		"OpenAL32"
	}
	includedirs
	{
		"../Depen/glad/include",
		"../Depen/stb",
		"../Depen/OpenAL/include"

	}
	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"