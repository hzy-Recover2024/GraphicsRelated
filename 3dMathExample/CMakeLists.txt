project(3dMathExample)
##记录头文件

##记录源文件
set(SRCS 
	src/main.cpp)

##构建虚拟文件夹
##source_group("inc" FILES ${HEADERS})
source_group("src" FILES ${SRCS})

##生成库
add_library(${PROJECT_NAME} STATIC ${SRCS})

##头文件可见
##target_include_directories(${PROJECT_NAME} PUBLIC inc/)

target_link_libraries(${PROJECT_NAME} PRIVATE glm)

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
