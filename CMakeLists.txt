cmake_minimum_required(VERSION 3.30)
project(GraphicsRelated) ##解决项目名

##设置c++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 设置全局 C 标准为 C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)   # 要求编译器必须支持 C11
##启用VS虚拟文件
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
option(GLM_BUILD_LIBRARY "Build dynamic/static library" ON)
##子项目
add_subdirectory(glm)
add_subdirectory(3dMathExample)
add_subdirectory(grmath)
add_subdirectory(performanceTool)
add_subdirectory(octree)
add_subdirectory(graphics)