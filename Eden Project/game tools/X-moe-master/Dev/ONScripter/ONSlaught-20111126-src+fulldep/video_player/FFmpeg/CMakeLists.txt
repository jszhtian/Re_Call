cmake_minimum_required (VERSION 2.4.7)
project(video_player)

include_directories(../../include)
include_directories(../deps/include)
include_directories(../deps/src/openal/include/AL)
link_directories(../../deps/lib)
link_directories(../deps/lib)

set(EXECUTABLE_OUTPUT_PATH .)
set(CMAKE_BUILD_TYPE Release)
add_definitions(-D__STDC_CONSTANT_MACROS)

set(LIBS
	${LIBS}
	avformat
	avcodec
	avutil
	swscale
	SDL
	z
)

add_library(video_player SHARED
	video_player.cpp
	../common.cpp
	../font.cpp
	../../src/Thread.cpp
)

target_link_libraries(video_player ${LIBS})
