project(grmath)
##记录头文件
set(HEADERS 
    inc/GR_math.h )

##构建虚拟文件夹
source_group("inc" FILES ${HEADERS})

##生成库
add_library(${PROJECT_NAME} INTERFACE ${HEADERS})

##头文件可见
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc/)

