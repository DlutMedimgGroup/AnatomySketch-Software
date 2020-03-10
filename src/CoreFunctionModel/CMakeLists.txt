# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 模块名
set(MODULE_NAME CoreFunctionModel)

# SRC
file(GLOB_RECURSE MODULE_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp"
                                                                   "*.h")

                                                                   
foreach(source IN LISTS MODULE_SRCS)
    source_group("\\" FILES "${source}")
endforeach()
# 动态库
add_library(${MODULE_NAME} STATIC ${MODULE_SRCS})

# 链接QT
target_link_libraries(${MODULE_NAME} Qt5::Widgets)
