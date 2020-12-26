workspace "Containers"
    startproject "Testbench"
    architecture "x64"
    warnings "extra"

    -- Set output dir
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}-%{cfg.platform}"

    -- Platform
	platforms
	{
		"x64",
    }

    -- Configurations
    configurations
    {
        "Debug",
        "Release",
        "Production",
    }

    filter "configurations:Debug"
        symbols "on"
        runtime "Debug"
        optimize "Off"
        defines
        {
            "_DEBUG",
        }
    filter "configurations:Release"
        symbols "on"
        runtime "Release"
        optimize "Full"
        defines
        {
            "NDEBUG",
        }
    filter "configurations:Production"
        symbols "off"
        runtime "Release"
        optimize "Full"
        defines
        {
            "NDEBUG",
        }
    filter {}

    -- Compiler option
	filter "action:vs*"
        defines
        {
            "COMPILER_VISUAL_STUDIO",
            "_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING",
            "_CRT_SECURE_NO_WARNINGS",
        }
    filter {}

	-- Project
    project "Testbench"
        language "C++"
        cppdialect "C++17"
        systemversion "latest"
        location "Testbench"
        kind "ConsoleApp"
		characterset "Ascii"
		
        -- Targets
		targetdir 	("Build/bin/" .. outputdir .. "/%{prj.name}")
		objdir 		("Build/bin-int/" .. outputdir .. "/%{prj.name}")	
    
        -- Files to include
		files 
		{ 
			"%{prj.name}/**.h",
			"%{prj.name}/**.hpp",
			"%{prj.name}/**.inl",
			"%{prj.name}/**.c",
            "%{prj.name}/**.cpp",
            "Containers/*.h",
        }
    project "*"