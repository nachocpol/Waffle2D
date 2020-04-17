workspace "Waffle2D"
	configurations { "Debug", "Release"}
	platforms "x64"
	systemversion "10.0.16299.0"
	staticruntime "on"

filter { "platforms:x64" }
	includedirs 
	{
		"Source"
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
		"Source/**.h",
		"Source/**.cpp",
		"Depen/glad/src/**.c"
	}
	links
	{
		"OpenGL32"
	}
	includedirs
	{
		"Depen/glad/include",
		"Depen/stb"
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
		"Projects/Test/**.h",
		"Projects/Test/**.cpp"
	}
	links
	{
		"Waffle2D"
	}
	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"