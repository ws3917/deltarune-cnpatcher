# 初始设置
cmake_minimum_required(VERSION 3.21)
project(cn_patcher)

# C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 预加载第三方库
file(GLOB THIRD_PARTY_DIRS 3rdparty/*)
if(THIRD_PARTY_DIRS)
    list(PREPEND CMAKE_PREFIX_PATH ${THIRD_PARTY_DIRS})
endif()

# 需要使用的第三方库
find_package(SDL3 CONFIG REQUIRED)

# 主项目：包含所有C++文件
file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# 设置目标属性
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE $<CONFIG:Release>
)

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-shared)


# 构建后续操作：自动复制 DLL
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
)
