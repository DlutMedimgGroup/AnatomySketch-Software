# 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 模块名
set(MODULE_NAME Translation)

# SRC
file(GLOB_RECURSE PROJECT_SRCS "${PROJECT_SOURCE_DIR}/*.cpp"
                                "${PROJECT_SOURCE_DIR}/*.h")


set_source_files_properties(${PROJECT_SOURCE_DIR}/src/Translation/zh.ts PROPERTIES OUTPUT_LOCATION ${PROJECT_SOURCE_DIR}/src/Translation)
QT5_create_translation(QM_FILES ${PROJECT_SRCS} ${PROJECT_SOURCE_DIR}/src/Translation/zh.ts)

ADD_CUSTOM_TARGET (${MODULE_NAME} ALL DEPENDS ${QM_FILES})