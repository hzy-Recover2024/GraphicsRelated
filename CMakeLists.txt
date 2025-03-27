cmake_minimum_required(VERSION 3.30)
project(GraphicsRelated) ##解决项目名

##设置c++标准
set(CMAKE_CXX_STANDARD "11")
set(CMAKE_CXX_STANDARD_REQUIRED "True")

##启用VS虚拟文件
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

##子项目
add_subdirectory(octree)