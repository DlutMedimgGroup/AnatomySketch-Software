# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 添加子目录
add_subdirectory(CoreFunctionModel)
add_subdirectory(DataManager)
add_subdirectory(InteractorStyle)
add_subdirectory(Interface)
add_subdirectory(Models)
add_subdirectory(Thread)
add_subdirectory(Transmission)
add_subdirectory(Translation)
add_subdirectory(Visualization)
add_subdirectory(vtk)
