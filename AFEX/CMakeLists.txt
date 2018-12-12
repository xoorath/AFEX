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
)

add_definitions(-DELPP_NO_DEFAULT_LOG_FILE)
add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)

set(CMAKE_SUPPRESS_REGENERATION true)
add_library(AFEX_Lib ${AFEX_LIB_PUBLIC_SRC} ${AFEX_LIB_PRIVATE_SRC} ${AFEX_LIB_THIRDPARTY_SRC})

target_include_directories(AFEX_Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/easyloggingpp)
target_include_directories(AFEX_Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/json/include)
target_include_directories(AFEX_Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
source_group("api"      FILES ${AFEX_LIB_PUBLIC_SRC})
source_group("internal" FILES ${AFEX_LIB_PRIVATE_SRC})
source_group("third party" FILES ${AFEX_LIB_THIRDPARTY_SRC})

add_subdirectory(ThirdParty/glfw)
include_directories(ThirdParty/glfw/include)
target_link_libraries(AFEX_Lib glfw ${GLFW_LIBRARIES})