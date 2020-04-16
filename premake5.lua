workspace "Waffle2D"
	configurations { "Debug", "Release"}
	platforms "x64"
	systemversion "10.0.16299.0"
	staticruntime "on"

filter { "platforms:x64" }
	includedirs 
	{
		"Source/Waffle"
	}

filter {"configurations:Debug"}
	libdirs
	{
	}
	links
	{
	}

filter {"configurations:Release"}
	libdirs
	{
	}
	links
	{
	}


project "Waffle2D"
	kind "StaticLib"
	language "C++"
	location "Temp/VSFiles"
	targetdir "Build/%{cfg.platform}/%{cfg.buildcfg}"
	files
	{
		"Source/Waffle/**.h",
		"Source/Waffle/**.cpp",
		"Depen/glad/src/**.c"
	}
	links
	{
		"OpenGL32"
	}
	includedirs
	{
		"Depen/glad/include"
	}
	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "Test"
	kind "ConsoleApp"
	language "C++"
	location "Temp/VSFiles"
	targetdir "Build/%{cfg.platform}/%{cfg.buildcfg}"
	files
	{
		"Source/Test/**.h",
		"Source/Test/**.cpp"
	}
	links
	{
		"Waffle2D"
	}
	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"