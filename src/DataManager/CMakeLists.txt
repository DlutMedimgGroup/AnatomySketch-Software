# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 模块名
set(MODULE_NAME DataManager)

# SRC
file(GLOB MODULE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB MODULE_SUB_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/DataType/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/DataType/*.h")
file(GLOB MODULE_SUB_SUB_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/DataType/ROIType/*.cpp"
                                "${CMAKE_CURRENT_SOURCE_DIR}/DataType/ROIType/*.h")

                                                                   
foreach(source IN LISTS MODULE_SRCS)
    source_group("\\" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SUB_SRCS)
    source_group("\\DataType" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SUB_SUB_SRCS)
    source_group("\\DataType\\ROIType" FILES "${source}")
endforeach()
          


# 动态库
add_library(${MODULE_NAME} STATIC ${MODULE_SRCS} ${MODULE_SUB_SRCS} ${MODULE_SUB_SUB_SRCS})

# 链接QT
target_link_libraries(${MODULE_NAME} Qt5::Widgets)
