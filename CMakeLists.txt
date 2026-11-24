cmake_minimum_required(VERSION 3.10)
project(FlappyBirdOpenGL LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------------------------- #
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Include)   
set(LIB_DIR     ${CMAKE_SOURCE_DIR}/Lib)       

# --------------------------------------------------------------------------------------------- #
add_executable(FlappyBird
    src/main.cpp
    src/ShaderClass.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp
    src/glad.c
 "Include/main.h"  )

target_include_directories(FlappyBird PRIVATE ${INCLUDE_DIR})

# --------------------------------------------------------------------------------------------- #
if(WIN32)
    target_link_directories(FlappyBird PRIVATE ${LIB_DIR})
    target_link_libraries(FlappyBird PRIVATE
        ${LIB_DIR}/glfw3.lib
        opengl32
        gdi32
        user32
        shell32
    )
else()
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    target_link_libraries(FlappyBird PRIVATE glfw OpenGL::GL)

    if(APPLE)
        target_link_libraries(FlappyBird PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework OpenGL"
        )
    else()
        target_link_libraries(FlappyBird PRIVATE dl pthread)
    endif()
endif()

# --------------------------------------------------------------------------------------------- #
add_custom_command(TARGET FlappyBird POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/Shaders
            $<TARGET_FILE_DIR:FlappyBird>/Shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/assets
            $<TARGET_FILE_DIR:FlappyBird>/assets
)
