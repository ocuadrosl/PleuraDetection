cmake_minimum_required(VERSION 3.5)

project(PleuraDetection LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(VTK REQUIRED)
find_package(ITK REQUIRED)
find_package(dlib REQUIRED)

include(${ITK_USE_FILE})
include(${VTK_USE_FILE})

file(GLOB core
    "core/*.h"
   "core/*.cpp"
)


file(GLOB util
    "util/*.h"
   "util/*.cpp"
)


add_executable(${PROJECT_NAME}
    "main.cpp"
    ${core}
    ${util}

    )

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${ITK_LIBRARIES})
target_link_libraries(${PROJECT_NAME} dlib::dlib)

