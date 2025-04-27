project(grrender)
##记录头文件
set(HEADERS 
    inc/grgl_context.h 
    inc/grgl_type.h
    inc/grgl_object.h
    inc/grgl_texture.h
    inc/grgl_FBO.h
    inc/grgl_vertex_rendering.h
    inc/grgl_per_sample_test.h
    inc/grgl_vertex_pipeline.h
    inc/stb_image.h
    inc/grgl_camera.h
    inc/grgl_model.h
    inc/grgl_mesh.h
)

##记录源文件
set(SRCS 
    src/entry.cpp
    src/grgl_context.cpp
    src/grgl_object.cpp
    src/grgl_texture.cpp
    src/grgl_FBO.cpp
    src/grgl_vertex_rendering.cpp
    src/grgl_per_sample_test.cpp
    src/grgl_vertex_pipeline.cpp
    src/grgl_camera.cpp
    src/grgl_mesh.cpp 
    src/grgl_model.cpp
)

set(SHADER_SRCS
    ${CMAKE_SOURCE_DIR}/shader/multiple_lights.fs
    ${CMAKE_SOURCE_DIR}/shader/multiple_lights.vs
)

##构建虚拟文件夹
source_group("inc" FILES ${HEADERS})
source_group("src" FILES ${SRCS})
source_group("shader" FILES ${SHADER_SRCS})

##生成库
add_library(${PROJECT_NAME} STATIC ${SRCS} ${HEADERS} ${SHADER_SRCS})

find_package(OpenGL REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
##头文件可见
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/ ${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_CURRENT_SOURCE_DIR}/thirdLibs/include/)

target_link_libraries(${PROJECT_NAME} PUBLIC grmath performanceTool ${OPENGL_LIBRARIES} glm glfw glad ${ASSIMP_LIBRARY})

##编译标志
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE
        # Debug 配置
        $<$<CONFIG:Debug>:
            /W4     # 警告级别 4
            /Od     # 禁用优化
        >
        # Release 配置
        $<$<CONFIG:Release>:
            /Ox     # 完全优化
        >
        # RelWithDebInfo 配置
        $<$<CONFIG:RelWithDebInfo>:
            /Ox     # 完全优化
            /Zi     # 生成调试信息
        >
        # MinSizeRel 配置
        $<$<CONFIG:MinSizeRel>:
            /O1     # 优化代码大小
        >
    )
endif()

