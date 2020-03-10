# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 模块名
set(MODULE_NAME Models)

# SRC
file(GLOB MODULE_SRCS1 "${CMAKE_CURRENT_SOURCE_DIR}/Addins/*.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/Addins/*.h")
file(GLOB MODULE_SUB_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Addins/IO/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/Addins/IO/*.h")   
file(GLOB MODULE_SRCS2 "${CMAKE_CURRENT_SOURCE_DIR}/GraphCut/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/GraphCut/*.h")
file(GLOB MODULE_SRCS3 "${CMAKE_CURRENT_SOURCE_DIR}/PolyDeformation/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/PolyDeformation/*.h")  
file(GLOB MODULE_SRCS4 "${CMAKE_CURRENT_SOURCE_DIR}/Segmentation/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/Segmentation/*.h")                          
                            

foreach(source IN LISTS MODULE_SRCS1)
    source_group("\\Addins" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SUB_SRCS)
    source_group("\\Addins\\IO" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SRCS2)
    source_group("\\GraphCut" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SRCS3)
    source_group("\\PolyDeformation" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SRCS4)
    source_group("\\Segmentation" FILES "${source}")
endforeach()

# 动态库
add_library(${MODULE_NAME} STATIC ${MODULE_SRCS1}
                                    ${MODULE_SUB_SRCS}
                                    ${MODULE_SRCS2}
                                    ${MODULE_SRCS3}
                                    ${MODULE_SRCS4})

# 链接QT
target_link_libraries(${MODULE_NAME} Qt5::Widgets)
