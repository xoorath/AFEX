cmake_minimum_required(VERSION 2.6)
project(AFEX_Lib)

file(GLOB_RECURSE AFEX_LIB_PUBLIC_SRC
    "Include/Engine/*.h"
    "Include/Engine/*.inl"
    "Include/Engine/*.cpp"
)

file(GLOB_RECURSE AFEX_LIB_PRIVATE_SRC
    "Source/Engine/*.h"
    "Source/Engine/*.inl"
    "Source/Engine/*.cpp"
)

file(GLOB_RECURSE AFEX_LIB_THIRDPARTY_SRC
    "ThirdParty/easyloggingpp/easylogging++.cc"
    "ThirdParty/nuklear/nuklear.cpp"
)

add_definitions(-DELPP_NO_DEFAULT_LOG_FILE)
add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_subdirectory(ThirdParty/glfw)
include_directories(ThirdParty/glfw/include)
include_directories("${GLFW_SOURCE_DIR}/deps")
set(GLAD "${GLFW_SOURCE_DIR}/deps/glad/glad.h" "${GLFW_SOURCE_DIR}/deps/glad.c")


set(CMAKE_SUPPRESS_REGENERATION true)
add_library(AFEX_Lib ${AFEX_LIB_PUBLIC_SRC} ${AFEX_LIB_PRIVATE_SRC} ${AFEX_LIB_THIRDPARTY_SRC} ${GLAD})
target_sources(AFEX_Lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Misc/afex.natvis)

target_include_directories(AFEX_Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/easyloggingpp)
target_include_directories(AFEX_Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/json/include)
target_include_directories(AFEX_Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/nuklear/include)
target_include_directories(AFEX_Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(AFEX_Lib glfw ${GLFW_LIBRARIES})

source_group("api" PREFIX "api"  TREE ${CMAKE_CURRENT_SOURCE_DIR}/Include/Engine FILES ${AFEX_LIB_PUBLIC_SRC})
source_group("internal" PREFIX "internal" TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine FILES ${AFEX_LIB_PRIVATE_SRC})
source_group("third party" PREFIX "third party"  TREE ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty FILES ${AFEX_LIB_THIRDPARTY_SRC} ${GLAD})
