cmake_minimum_required(VERSION 2.6)
project(AFEX_Demo)

file(GLOB AFEX_DEMO_SRC
    "Source/*.h"
    "Source/*.inl"
    "Source/*.cpp"
)

add_definitions(-DELPP_NO_DEFAULT_LOG_FILE)
add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT AFEX_Demo)
add_executable(AFEX_Demo WIN32 ${AFEX_DEMO_SRC})
target_link_libraries(AFEX_Demo AFEX_Lib opengl32)
set_target_properties(AFEX_Demo PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Assets")
