cmake_minimum_required(VERSION 3.20)
project(renderEngine)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED) 

include_directories(
		src/Vendor/
		src/Vendor/glad/include
		src/Vendor/stb_image/include
)


add_executable(
		${PROJECT_NAME}
		src/main.cpp
		src/Vendor/glad/src/glad.c
		src/Shader.cpp
		src/Camera.cpp
		src/Vendor/stb_image/src/stb_image.cpp
)

target_link_libraries(
		${PROJECT_NAME}
		glfw
		OpenGL::GL
		${CMAKE_DL_LIBS}
)
