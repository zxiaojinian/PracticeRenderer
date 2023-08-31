workspace "PracticeRenderer"
    architecture "x86_64"
    startproject "PracticeRenderer"

    configurations 
    { 
        "Debug", 
        "Release", 
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "PracticeRenderer/Dependencies/GLFW"
    include "PracticeRenderer/Dependencies/Glad"
    include "PracticeRenderer/Dependencies/imgui"
group ""

project "PracticeRenderer"
    location "PracticeRenderer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "%{prj.name}/Source/pch.cpp"

    files 
    { 
        "%{prj.name}/Source/**.h", 
        "%{prj.name}/Source/**.cpp",
        "%{prj.name}/Source/**.inl",
        "%{prj.name}/Dependencies/stb_image/**.h",
        "%{prj.name}/Dependencies/stb_image/**.cpp",
        "%{prj.name}/Dependencies/glm/glm/**.hpp",
        "%{prj.name}/Dependencies/glm/glm/**.inl",
    }

    defines
    {
        --"_CRT_SECURE_NO_WARNINGS"
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "%{prj.name}/Source",
        "%{prj.name}/Dependencies/GLFW/include",
        "%{prj.name}/Dependencies/Glad/include",
        "%{prj.name}/Dependencies/imgui",
        "%{prj.name}/Dependencies/glm",
        "%{prj.name}/Dependencies/stb_image",
        "%{prj.name}/Dependencies/spdlog/include",
        "%{prj.name}/Dependencies/assimp/include"
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

    filter "configurations:Debug"
        defines "PR_DEBUG"
        runtime "Debug"
        symbols "on"

        links 
        { 
            --"%{wks.location}/%{prj.name}/Dependencies/assimp/bin/Debug/assimp-vc143-mtd.lib"
            "%{wks.location}/%{prj.name}/Dependencies/assimp/bin/Debug/assimp.lib"
        }

        postbuildcommands 
        {
            --'{COPY} "%{wks.location}/%{prj.name}/Dependencies/assimp/bin/Debug/assimp-vc143-mtd.dll" "%{cfg.targetdir}"'
            '{COPY} "%{wks.location}/%{prj.name}/Dependencies/assimp/bin/Debug/assimp-vc140-mt.dll" "%{cfg.targetdir}"'
        }

    filter "configurations:Release"
        defines "PR_RELEASE"
        runtime "Release"
        optimize "on"

        links 
        { 
            --"%{wks.location}/%{prj.name}/Dependencies/assimp/bin/Release/assimp-vc143-mt.lib"
            "%{wks.location}/%{prj.name}/Dependencies/assimp/bin/Debug/assimp.lib"
        }

        postbuildcommands 
        {
            --'{COPY} "%{wks.location}/%{prj.name}/Dependencies/assimp/bin/Release/assimp-vc143-mt.dll" "%{cfg.targetdir}"'
            '{COPY} "%{wks.location}/%{prj.name}/Dependencies/assimp/bin/Debug/assimp-vc140-mt.dll" "%{cfg.targetdir}"'
        }