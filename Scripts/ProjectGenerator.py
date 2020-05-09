import subprocess;
import os;
import shutil;

CurDir = os.getcwd();
print("Current dir is: " + CurDir);

ProjName = "Test";
ProjPath = CurDir + "\\Projects\\" + ProjName;
if os.path.exists(ProjPath) == False:
    print("Project doesn't exist");
    print("\t" + ProjPath);
    exit(-1);

# Check premake exe
PremakeExe = CurDir + "\\Depen\\premake\\premake5.exe"
if os.path.exists(PremakeExe):
    print("Using premake at: " + PremakeExe);
else:
    print("Failed to find the premake exe: " + PremakeExe);    
    exit(-1);

# Generate the premake config
BaseLuaPath = CurDir + "\Scripts\BaseSolution.lua";
ProjLuaPath = ""
if os.path.exists(BaseLuaPath):
    ProjLuaPath = CurDir + "\Temp\PremakeConfig.lua";
    os.makedirs(os.path.dirname(ProjLuaPath), exist_ok=True);    
    shutil.copyfile(BaseLuaPath, ProjLuaPath);
else:
    print("Couldn't find BaseSolution.lua");


"""
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
"""

ConfigFile = open(ProjLuaPath,"a")

ConfigFile.write("\n\n");
ConfigFile.write("project \"{0}\"\n".format(ProjName));
ConfigFile.write("\tkind \"ConsoleApp\"\n");
ConfigFile.write("\tlanguage \"C++\"\n");
ConfigFile.write("\tlocation \"../Temp/VSFiles\"\n");
ConfigFile.write("\ttargetdir \"../Build/%{cfg.platform}/%{cfg.buildcfg}\"\n");
ConfigFile.write("\tfiles\n");

ConfigFile.write("\t{\n");
ConfigFile.write(("\t\t\"../Projects/{0}/**.h\",\n").format(ProjName));
ConfigFile.write(("\t\t\"../Projects/{0}/**.cpp\"\n").format(ProjName));
ConfigFile.write("\t}\n");

ConfigFile.write("\tlinks\n");
ConfigFile.write("\t{\n");
ConfigFile.write("\t\t\"Waffle2D\"\n");
ConfigFile.write("\t}\n");

ConfigFile.write("\tfilter \"configurations:Debug\"\n");
ConfigFile.write("\t\tsymbols \"On\"\n");

ConfigFile.write("\tfilter \"configurations:Release\"\n");
ConfigFile.write("\t\toptimize \"On\"\n");

ConfigFile.close();

PremakeCmdLine = PremakeExe + " vs2017 --file=\"" + ProjLuaPath + "\"";
#PremakeCmdLine = PremakeExe + " --help";
print(PremakeCmdLine);
os.system(PremakeCmdLine);



