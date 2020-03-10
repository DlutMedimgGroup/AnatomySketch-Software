# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 模块名
set(MODULE_NAME Interface)

# SRC
file(GLOB MODULE_SRCS1 "${CMAKE_CURRENT_SOURCE_DIR}/FunctionWidget/*.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/FunctionWidget/*.h")
file(GLOB MODULE_SRCS2 "${CMAKE_CURRENT_SOURCE_DIR}/MainWidget/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/MainWidget/*.h")
file(GLOB MODULE_SRCS3 "${CMAKE_CURRENT_SOURCE_DIR}/OperationBar/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/OperationBar/*.h")  
file(GLOB MODULE_SRCS4 "${CMAKE_CURRENT_SOURCE_DIR}/SideBar/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/SideBar/*.h")    
file(GLOB MODULE_SUB_SRCS1 "${CMAKE_CURRENT_SOURCE_DIR}/SideBar/PaintingLandmarkWidget/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/SideBar/PaintingLandmarkWidget/*.h")         
file(GLOB MODULE_SUB_SRCS2 "${CMAKE_CURRENT_SOURCE_DIR}/SideBar/DataManagerWidget/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/SideBar/DataManagerWidget/*.h")         
file(GLOB MODULE_SUB_SUB_SRCS1 "${CMAKE_CURRENT_SOURCE_DIR}/SideBar/DataManagerWidget/ItemWidget/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/SideBar/DataManagerWidget/ItemWidget/*.h")      
file(GLOB MODULE_SUB_SUB_SRCS2 "${CMAKE_CURRENT_SOURCE_DIR}/SideBar/DataManagerWidget/RClickMenu/*.cpp"
                            "${CMAKE_CURRENT_SOURCE_DIR}/SideBar/DataManagerWidget/RClickMenu/*.h")                        
                            

foreach(source IN LISTS MODULE_SRCS1)
    source_group("\\FunctionWidget" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SRCS2)
    source_group("\\MainWidget" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SRCS3)
    source_group("\\OperationBar" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SRCS4)
    source_group("\\SideBar" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SUB_SRCS1)
    source_group("\\SideBar\\PaintingLandmarkWidget" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SUB_SRCS2)
    source_group("\\SideBar\\DataManagerWidget" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SUB_SUB_SRCS1)
    source_group("\\SideBar\\DataManagerWidget\\ItemWidget" FILES "${source}")
endforeach()

foreach(source IN LISTS MODULE_SUB_SUB_SRCS2)
    source_group("\\SideBar\\DataManagerWidget\\RClickMenu" FILES "${source}")
endforeach()

# 动态库
add_library(${MODULE_NAME} STATIC ${MODULE_SRCS1} 
                                    ${MODULE_SRCS2} 
                                    ${MODULE_SRCS3} 
                                    ${MODULE_SRCS4} 
                                    ${MODULE_SUB_SRCS1}
                                    ${MODULE_SUB_SRCS2}
                                    ${MODULE_SUB_SUB_SRCS1}
                                    ${MODULE_SUB_SUB_SRCS2})

# 链接QT
target_link_libraries(${MODULE_NAME} Qt5::Widgets)