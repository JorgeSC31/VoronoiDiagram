workspace "Voronoi"
    configurations { "Debug", "Release" }

project "Voronoi"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
    objdir "%{wks.location}/obj/%{cfg.buildcfg}"

    files {
        "src/**.cpp",
        "src/**.h"
    }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"