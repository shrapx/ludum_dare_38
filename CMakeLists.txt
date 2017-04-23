cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(TARGET ludumdare)
project(${TARGET})

# includes
#set(SFML_ROOT "C:/Program Files (x86)/SFML")
find_package(SFML COMPONENTS audio graphics window system)
include_directories(${SFML_INCLUDE_DIR})
#include_directories("E:/dev/usr/include")

# libraries
set(LIBS
	${SFML_LIBRARIES}
	${SFML_DEPENDENCIES}
)

# source files
set(SRC
	src/sfml_math_extra.hpp
	src/timer.hpp
	src/input.hpp
	src/persist.hpp
	
	src/asset.hpp
	src/scene.hpp
	
	src/main.cpp
)

add_executable(${TARGET} ${SRC})
target_link_libraries(${TARGET} ${LIBS})
