project(graphics)
##记录头文件
set(HEADERS 
    inc/drawable_node.h 
    inc/drawable_node_proxy.h 
    inc/node.h
    inc/node_visitor.h
    inc/transform_node.h
    inc/engine.h
    inc/view.h
    inc/vertex_stream.h
    inc/instance_stream.h
    inc/render_batch.h
    inc/render_canvas.h
)

##记录源文件
set(SRCS 
    src/drawable_node.cpp 
    src/drawable_node_proxy.cpp 
    src/node.cpp
    src/node_visitor.cpp
    src/transform_node.cpp
    src/engine.cpp
    src/main.cpp
    src/view.cpp
    src/vertex_stream.cpp
    src/instance_stream.cpp
    src/render_batch.cpp
    src/render_canvas.cpp
)

##构建虚拟文件夹
source_group("inc" FILES ${HEADERS})
source_group("src" FILES ${SRCS})

##生成库
add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})

##头文件可见
target_include_directories(${PROJECT_NAME} PUBLIC inc/ ${CMAKE_SOURCE_DIR}/)

target_link_libraries(${PROJECT_NAME} PUBLIC grmath performanceTool octree grrender)

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
# 自动复制 DLL 到输出目录
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/dlls/assimp-vc140-mt.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# 构建时复制资源文件到输出目录
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${RESOURCE_DIR}"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resource"
)
