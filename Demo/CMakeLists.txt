cmake_minimum_required(VERSION 2.6)
project(AFEX_Demo)

file(GLOB AFEX_DEMO_SRC
    "Source/*.h"
    "Source/*.inl"
    "Source/*.cpp"
)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AFEX_Demo)
add_executable(AFEX_Demo WIN32 ${AFEX_DEMO_SRC})
target_link_libraries(AFEX_Demo AFEX_Lib opengl32)