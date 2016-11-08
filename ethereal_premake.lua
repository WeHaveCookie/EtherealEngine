-- Here is an example of the most common used configuration functions
-- For more in-depth use of premake, refer to the online documentation:
-- https://github.com/premake/premake-core/wiki

-- Add additional defines:
	-- defines { "DEFINE1", "DEFINE2" }

-- Remove existing defines:
	-- undefines { "DEFINE1", "DEFINE2" }

-- Add files to the project:
	-- files { "foo/**.cpp", "foo/**.h", "bar/file.lua" }

-- Remove files to the project:
	-- removefiles { "foo/**.h", "bar/file.lua" }

-- Add include directories:
	-- includedirs { "foo/", "bar/" }

-- Remove include directories:
	-- removeincludedirs { "foo/", "bar/" }

-- Link libraries:
	-- links { "libname", "lib2" }

-- Remove libraries:
	-- removelinks { "libname", "lib2" }

-- Add libraries directory
	-- libdirs { "foo/", "bar/" }

-- Remove libraries directory
	-- removelibdirs { "foo/", "bar/" }


local etherealEnginePath = "./"
include(etherealEnginePath .. "Premake/etherealEngineProject")

solution "EtherealEngine"
	platforms { "Win32", "x64" }
	configurations { "Debug", "Debug Optimized", "Release", "Final" }

	startproject "EtherealEngineApp"

	
	filter "action:vs2015"
		location ("Project/VisualStudio2015/")
		objdir("Project/VisualStudio2015/Obj/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}")
	filter {}

	includeEtherealEngine(etherealEnginePath)
	-- projects
	EtherealEngineProject("EtherealEngineApp", etherealEnginePath)

		targetname ( "EtherealEngine" )


		-- //////////// COMMON SETTINGS ////////////

		defines {
			"ETHEREAL_ENGINE"
		}


