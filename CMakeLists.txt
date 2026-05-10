cmake_minimum_required (VERSION 3.30)
project (OpenGL_learning)

set(CMAKE_CXX_STANDARD 17)

find_package (OpenGL REQUIRED)
include_directories(
    "external/glfw/include"
    "external/glad/include"
    "external/glm"
    "external"
)
set(ALL_LIBS
    ${OPENGL_LIBRARY}
    glfw
    glad
    glm
)



add_subdirectory (external)



file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets/)

add_executable(ex1 src/hw_1/main.cpp )
target_link_libraries(ex1 ${ALL_LIBS})

add_executable(ex2 src/hw_2/main.cpp)
target_link_libraries(ex2 ${ALL_LIBS})


add_executable(ex_3_1 src/hw_3/ex_3_1.cpp)
target_link_libraries(ex_3_1 ${ALL_LIBS})


add_executable(ex_3_2 src/hw_3/ex_3_2.cpp)
target_link_libraries(ex_3_2 ${ALL_LIBS})

add_executable(ex_4 src/hw_4/main.cpp)
target_link_libraries(ex_4 ${ALL_LIBS})