# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 设置工程名称
project(AnatomySketch)

# 设置工程包含当前目录
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# VTK
find_package(VTK REQUIRED)
if(VTK_FOUND)
  include(${VTK_USE_FILE})
else(VTK_FOUND)
  message(FATAL_ERROR "VTK not found. Please set VTK_DIR.")
endif(VTK_FOUND)

# ITK
find_package(ITK)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
else(ITK_FOUND)
  message(FATAL_ERROR "ITK not found. Please set ITK_DIR.")
endif(ITK_FOUND)

# QT
find_package(Qt5Widgets)
find_package(Qt5LinguistTools)
find_package(Qt5WebEngineWidgets)

# 包含目录
include_directories(
  ${VTK_DIR}
  ${PYTHON_INCLUDE_DIRS}
  ${PYTHON_NUMPY_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/src/Interface/MainWidget
  ${PROJECT_SOURCE_DIR}/src/Interface/SideBar/PaintingLandmarkWidget
  ${PROJECT_SOURCE_DIR}/src/Interface/SideBar/DataManagerWidget
  ${PROJECT_SOURCE_DIR}/src/Interface/SideBar/DataManagerWidget/ItemWidget
  ${PROJECT_SOURCE_DIR}/src/Interface/SideBar/DataManagerWidget/RClickMenu
  ${PROJECT_SOURCE_DIR}/src/Interface/SideBar/
  ${PROJECT_SOURCE_DIR}/src/Interface/OperationBar
  ${PROJECT_SOURCE_DIR}/src/Interface/FunctionWidget
  ${PROJECT_SOURCE_DIR}/src/Transmission
  ${PROJECT_SOURCE_DIR}/src/Translation
  ${PROJECT_SOURCE_DIR}/src/DataManager
  ${PROJECT_SOURCE_DIR}/src/DataManager/DataType
  ${PROJECT_SOURCE_DIR}/src/DataManager/DataType/ROIType
  ${PROJECT_SOURCE_DIR}/src/Visualization
  ${PROJECT_SOURCE_DIR}/src/Visualization/Pipeline
  ${PROJECT_SOURCE_DIR}/src/CoreFunctionModel
  ${PROJECT_SOURCE_DIR}/src/Thread
  ${PROJECT_SOURCE_DIR}/src/InteractorStyle
  ${PROJECT_SOURCE_DIR}/src/InteractorStyle/2D
  ${PROJECT_SOURCE_DIR}/src/InteractorStyle/3D
  ${PROJECT_SOURCE_DIR}/src/Models/GraphCut
  ${PROJECT_SOURCE_DIR}/src/Models/PolyDeformation
  ${PROJECT_SOURCE_DIR}/src/Models/Segmentation
  ${PROJECT_SOURCE_DIR}/src/Models/Addins
  ${PROJECT_SOURCE_DIR}/src/Models/Addins/IO
  ${PROJECT_SOURCE_DIR}/src/vtk
  ${CMAKE_CURRENT_BINARY_DIR})

# SRC
file(GLOB
     PROJECT_SRCS
     ${PROJECT_SOURCE_DIR}/src
     ${PROJECT_SOURCE_DIR}/src/*.cpp
     ${PROJECT_SOURCE_DIR}/src/*.h
     ${PROJECT_SOURCE_DIR}/Resources/myapp.rc)

# QRC
set(PROJECT_QRCS AnatomySketch.qrc)

# UI
set(PROJECT_UIS AnatomySketch.ui src/ASHelpWidget.ui
                src/Interface/FunctionWidget/AboutWidget.ui)

# 库文件目录
link_directories(${PROJECT_SOURCE_DIR}/lib)

foreach(source IN LISTS PROJECT_SRCS)
    source_group("\\" FILES "${source}")
endforeach()

# 添加可执行文件
add_executable(AnatomySketch ${PROJECT_SRCS} ${PROJECT_UIS} ${PROJECT_QRCS})

# 链接库
target_link_libraries(AnatomySketch
                      ${VTK_LIBRARIES}
                      ${ITK_LIBRARIES}
                      ${PYTHON_LIBRARIES}
                      Thread
                      CoreFunctionModel
                      DataManager
                      InteractorStyle
                      Interface 
                      Models
                      Transmission
                      Visualization
                      vtk
                      Qt5::Widgets
                      Qt5::WebEngineWidgets)

# 屏蔽Release运行时的控制台窗口
set_target_properties(${PROJECT_NAME}
                      PROPERTIES LINK_FLAGS_RELEASE
                                 "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

# 启用多核编译
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MP")

# 添加子目录
add_subdirectory(src)
