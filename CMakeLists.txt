cmake_minimum_required (VERSION 3.8)
project(FBML VERSION 0.1.0.0) # MAJOR.MINOR.PATCH.TWEAK

SET(VERSION_TYPE "IDB") # "(I)n (D)evelopment (B)uild"
#SET(VERSION_TYPE "RCB") # "(R)elease (C)andidate (B)uild"
#SET(VERSION_TYPE "PVB") # "(P)re(V)iew (B)uild"

SET(GAME_STRING "FBMC")
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d")

include_directories("lib")
include_directories("src")
include_directories("include")

file(GLOB_RECURSE SRC
    "src/*"
)

add_executable (${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ws2_32 kernel32)

#paths for build outputs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/debug/build_files)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/debug/)
