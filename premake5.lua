workspace "Direct3D"
    architecture "x86"
    configurations { "Debug", "Release" }
    startproject "Direct3D"

outputdir = "%{cfg.system:gsub(\"^%l\", string.upper)}%{cfg.architecture}/%{cfg.buildcfg}"

project "Direct3D"
    location "Direct3D"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("Intermediates/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.hlsl",
        "%{prj.name}/Source/**.h",
        "%{prj.name}/ThirdParty/**.h",
        "%{prj.name}/Source/**.cpp",
        "%{prj.name}/ThirdParty/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/ThirdParty"
    }

    pchheader "stdafx.h"
    pchsource "%{prj.name}/Source/stdafx.cpp"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    -- disable precompiled headers for some file(s)
    filter "files:**/ThirdParty/**.cpp"
        flags { "NoPCH" }

    filter "files:**.hlsl"
        shadermodel "5.0"
        shaderobjectfileoutput "Shaders/%{file.basename}.cso"
        
    filter "files:**.vertex.hlsl"
        shadertype "Vertex"

    filter "files:**.pixel.hlsl"
        shadertype "Pixel"