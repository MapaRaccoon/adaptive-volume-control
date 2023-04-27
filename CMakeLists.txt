cmake_minimum_required(VERSION 3.20)
project(adaptive-volume-control VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_package(portaudio REQUIRED)
find_package(portaudiocpp REQUIRED)
find_package(ALSA REQUIRED)
find_package(Boost REQUIRED)

file ( GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" )
add_executable(adaptive-volume-control ${SOURCES})

target_include_directories(adaptive-volume-control PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/include
    ${PORTAUDIO_INCLUDE_DIRS}
    ${PORTAUDIO_CPP_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(adaptive-volume-control PRIVATE 
    ${PORTAUDIO_LIBRARIES}
    ${PORTAUDIO_CPP_LIBRARIES}
    ALSA::ALSA
)

install( TARGETS adaptive-volume-control DESTINATION bin )
