cmake_minimum_required(VERSION 3.12)
project(comp_graph_01 C)

set(CMAKE_C_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLU REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

#add_executable(comp_graph_01 dinoshade.c)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} #[[${SRC_LIST}]] lab1.c)

target_link_libraries(
        ${PROJECT_NAME}
        m
        ${OPENGL_LIBRARY}
        ${OPENGL_glu_LIBRARY}
        ${GLUT_LIBRARIES} )
