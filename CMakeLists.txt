#1.工程信息
cmake_minimum_required(VERSION 3.10.0)
project(game-01 VERSION 0.1.0 LANGUAGES C CXX)

#2.设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#3.设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

#4.设置输出文件规范名称
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME}-1)


#5.add compiler options | error and warning
if(MSVC)
    add_compile_options(/W4)
    # 设置源文件编码为UTF-8
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#6.链接库目录
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)


#7.添加执行文件 
add_executable(${TARGET} 
            "src/main.cpp"
            "src/game_structure.cpp"
            "src/scene_main.cpp"
            "src/scene_main.h"
            "src/objects.h"
            "resources.rc"
            # 01-demo.cpp

            )


#8.链接库
target_link_libraries(${TARGET}
                    SDL2_image::SDL2_image
                    SDL2_ttf::SDL2_ttf
                    SDL2_mixer::SDL2_mixer
                    SDL2::SDL2main #before the SDL2::SDL2 ！
                    SDL2::SDL2) 

#9.不生成控制台窗口（仅Windows有效）
target_link_options(game-01-Windows-1 PRIVATE /SUBSYSTEM:WINDOWS)