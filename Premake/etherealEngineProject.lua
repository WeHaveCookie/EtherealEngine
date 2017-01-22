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



-- //////////// ETHEREAL Engine LIB DEPENDENCIES ////////////
-- Every lib needed in order to include EtherealEngineLib should be added here
function linkEtherealEngineLibs(etherealEnginePath)

-- //////////// COOMMON SETTINGS ////////////

	filter { "platforms:Win32 or platforms:x64"}
		includedirs {
			etherealEnginePath .. "../External/SFML-2.3.2/include",
		}
		
		links{
			"opengl32.lib",
			"Xinput.lib"
		}
		
		
-- //////////// WIN32 SETTINGS ////////////

	filter { "platforms:Win32"}
		postbuildcommands {
				"xcopy /f/y/d \"..\\..\\..\\External\\SFML-2.3.2\\bin\\x32\\openal32.dll\" \"$(OutDir)\"",
			}
			
		libdirs {
			etherealEnginePath .. "../External/SFML-2.3.2/lib/x32",
			"C:/Program Files\ (x86)/Microsoft SDKs/Windows/v7.1A/Lib"
		}

	filter { "platforms:Win32", "configurations:Debug or Debug Optimized" }

		links {
			"sfml-graphics-d",
			"sfml-window-d",
			"sfml-audio-d",
			"sfml-network-d",
			"sfml-system-d",
		}
		
		postbuildcommands {
				"xcopy /f/y/d \"..\\..\\..\\External\\SFML-2.3.2\\bin\\x32\\Debug\\*.dll\" \"$(OutDir)\"",
				"xcopy /f/y/d \"..\\..\\..\\External\\DLL\\x86\\*.dll\" \"$(OutDir)\"",
			}

		filter { "platforms:Win32", "configurations:Release or Final" }


		links {
			"sfml-graphics",
			"sfml-window",
			"sfml-audio",
			"sfml-network",
			"sfml-system",
		}
		
		postbuildcommands {
				"xcopy /f/y/d \"..\\..\\..\\External\\SFML-2.3.2\\bin\\x32\\Release\\*.dll\" \"$(OutDir)\"",
			}
			
-- //////////// WIN64 SETTINGS ////////////
	filter { "platforms:x64"}
		postbuildcommands {
				"xcopy /f/y/d \"..\\..\\..\\External\\SFML-2.3.2\\bin\\x64\\openal32.dll\" \"$(OutDir)\"",
				"xcopy /f/y/d \"..\\..\\..\\External\\DLL\\x64\\*.dll\" \"$(OutDir)\"",
			}
		
		libdirs {
			etherealEnginePath .. "../External/SFML-2.3.2/lib/x64",
			"C:/Program Files\ (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64"
		}
			
	filter { "platforms:x64", "configurations:Debug or Debug Optimized" }

		links {
			"sfml-graphics-d",
			"sfml-window-d",
			"sfml-audio-d",
			"sfml-network-d",
			"sfml-system-d",
		}
		
		postbuildcommands {
				"xcopy /f/y/d \"..\\..\\..\\External\\SFML-2.3.2\\bin\\x64\\Debug\\*.dll\" \"$(OutDir)\"",
			}

		filter { "platforms:x64", "configurations:Release or Final" }


		links {
			"sfml-graphics",
			"sfml-window",
			"sfml-audio",
			"sfml-network",
			"sfml-system",
		}
		
		postbuildcommands {
				"xcopy /f/y/d \"..\\..\\..\\External\\SFML-2.3.2\\bin\\x64\\Release\\*.dll\" \"$(OutDir)\"",
			}

	filter {}
end

function includeEtherealEngine(etherealEnginePath)
	
	-- Normalize Path
	if not string.endswith(etherealEnginePath, "/") then
		etherealEnginePath = etherealEnginePath .. "/"
	end
	
-- //////////// COMMON SETTINGS ////////////
		kind "StaticLib"
		targetdir(etherealEnginePath .. "Lib/%{cfg.platform}/%{cfg.buildcfg}/")
		removelinkoptions {
			"/LARGEADDRESSAWARE",
		}
	
		defines { 
			"ETHEREALENGINE_GAME",
		}
		
		removefiles {
			etherealEnginePath .. "Project/VisualStudio2015/main.cpp"
		}

	
end

function EtherealEngineProject(name, etherealEnginePath)
		
		etherealEnginePath = etherealEnginePath or "./"
			
		filter "action:vs2015"
			location ("Project/VisualStudio2015/")
			objdir("Project/VisualStudio2015/Obj/%{cfg.buildcfg}_%{cfg.platform}_%{prj.name}")
		filter {}
		
		project(name)

			debugdir "."
			kind "ConsoleApp"
			language "C++"
			targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
			flags { "Symbols", "MultiProcessorCompile", "NoMinimalRebuild", "NoBufferSecurityCheck", "NoRuntimeChecks" }
			rtti "On"
			editandcontinue "On"
			pchheader "stdafx.h"
			pchsource(etherealEnginePath .. "Project/VisualStudio/stdafx.cpp")
			files {
				etherealEnginePath .. "Classes/**.c",
				etherealEnginePath .. "Classes/**.cpp",
				etherealEnginePath .. "Classes/**.h",
				etherealEnginePath .. "Classes/**.inl",
				etherealEnginePath .. "Project/VisualStudio2015/*.c",
				etherealEnginePath .. "Project/VisualStudio2015/*.cpp",
				etherealEnginePath .. "Project/VisualStudio2015/*.h",
			}
			vpaths {
				["*"] = { "." },
			}
			includedirs {
				etherealEnginePath .. "Project/VisualStudio2015",
				etherealEnginePath .. "Classes",
				etherealEnginePath .. "../External/ImGui",
				etherealEnginePath .. "../External/rapidjson",
				etherealEnginePath .. "../External/moodycamel",
				
				"C:/Program Files (x86)/Windows Kits/10/Include/10.0.10150.0/ucrt"
			}
			
-- //////////// COMMON SETTINGS ////////////
			filter "platforms:Win32 or platforms:x64"
				debugenvs { "_NO_DEBUG_HEAP=1" }
				disablewarnings {
					"4244" -- warning C4244: 'argument': conversion from 'time_t' to 'unsigned int', possible loss of data
				}
				flags { "Unicode" }
				
-- //////////// WIN32 SETTINGS ////////////
			filter "platforms:Win32"
				defines { "WIN32", "_CONSOLE", "_CRT_SECURE_NO_WARNINGS" }
				libdirs {
					"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10150.0/ucrt/x86",
				}

-- //////////// WIN64 SETTINGS ////////////
			filter "platforms:x64"
				defines { "WIN64", "_CONSOLE", "_CRT_SECURE_NO_WARNINGS" }
				libdirs {
					"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10150.0/ucrt/x64",
				}
		
			filter "configurations:Debug"
				defines { "ETHEREALENGINE_DEBUG", "_DEBUG" }
				runtime "Debug"

			filter "configurations:Debug Optimized"
				defines { "ETHEREALENGINE_DEBUG", "_DEBUG" }
				runtime "Debug"


			filter "configurations:Release"
				defines { "NDEBUG" }
				runtime "Release"
				optimize "Speed"
				
			filter "configurations:Final"
				defines { "ETHEREALENGINE_FINAL", "NDEBUG" }
				runtime "Release"
				optimize "Full"

			filter { "platforms:Win32", "configurations:Final" }
				linkoptions {
					"/LTCG",	-- Link Time Code Generation
				}
				
			filter {}

		-- This call provides all the libs needed by Ethereal Engine
		linkEtherealEngineLibs(etherealEnginePath)
		
end
