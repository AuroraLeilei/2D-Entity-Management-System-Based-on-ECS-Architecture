cmake_minimum_required(VERSION 3.10)
project(ECS-2D-Entity-Manager)

set(CMAKE_CXX_STANDARD 14)  # 已改好C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include)

# 自动匹配src/ecs下的所有cpp（包括新建的Component.cpp）
file(GLOB ECS_SRC ${PROJECT_SOURCE_DIR}/src/ecs/*.cpp)
file(GLOB RENDERER_SRC ${PROJECT_SOURCE_DIR}/src/renderer/*.cpp)
file(GLOB ECS_SRC ${PROJECT_SOURCE_DIR}/src/ecs/*.cpp)
set(SOURCE_FILES 
    ${PROJECT_SOURCE_DIR}/src/main.cpp 
    ${ECS_SRC} 
    ${RENDERER_SRC}
)

add_executable(ecs-entity-manager ${SOURCE_FILES})

# 关键：链接SDL2main（解决SDL_main未定义）
target_link_libraries(ecs-entity-manager ${SDL2_LIBRARIES})