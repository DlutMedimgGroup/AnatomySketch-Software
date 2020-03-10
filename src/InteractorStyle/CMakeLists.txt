# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 模块名
set(MODULE_NAME InteractorStyle)

# SRC
file(GLOB MODULE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB MODULE_SUB_SRCS1 "${CMAKE_CURRENT_SOURCE_DIR}/2D/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/2D/*.h")
file(GLOB MODULE_SUB_SRCS2 "${CMAKE_CURRENT_SOURCE_DIR}/3D/*.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/3D/*.h")

foreach(source IN LISTS MODULE_SRCS)
    source_group("\\" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SUB_SRCS1)
    source_group("\\2D" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SUB_SRCS2)
    source_group("\\3D" FILES "${source}")
endforeach()

# 动态库
add_library(${MODULE_NAME} STATIC ${MODULE_SRCS} ${MODULE_SUB_SRCS1} ${MODULE_SUB_SRCS2})

# 链接QT
target_link_libraries(${MODULE_NAME} Qt5::Widgets)
