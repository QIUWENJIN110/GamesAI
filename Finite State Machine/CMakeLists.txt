# ##############################################################################
# 基础项目配置
# ##############################################################################
# 最低CMake版本，3.16为稳定基线，FATAL_ERROR：版本不足直接终止
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# 项目定义：名称、版本号、编程语言
project(
    StealthGame   # 项目名称 不能带空格 如果带空格要➕“” “Network Lab”
    VERSION 1.0.0 # 项目版本号
    LANGUAGES CXX
)

include_directories(${PROJECT_SOURCE_DIR})

# C++标准配置：标准C++20，禁用编译器扩展，保证跨平台一致性
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ##############################################################################
# 编译警告开关：用OPTION控制，编译时自由切换，商业项目常用
# ##############################################################################
# 选项：BUILD_STRICT_MODE，默认OFF，开启=警告全部当成错误
option(BUILD_STRICT_MODE "Enable strict compile warnings (warning as error)" OFF)

if(BUILD_STRICT_MODE)
    if(MSVC)
        # Windows MSVC：最高警告等级，警告=错误
        add_compile_options(/W4 /WX)
    else()
        # Mac Clang / Linux GCC：全套严格警告，警告=错误
        add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    endif()
else()
    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
endif()

# ##############################################################################
# 默认构建类型：没指定就默认Release发布版
# ##############################################################################
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ========== 新增：打印项目配置信息，配置阶段直接看参数 ==========
message(STATUS "========================================")
message(STATUS "Project Name:        ${PROJECT_NAME}")
message(STATUS "Project Version:     ${PROJECT_VERSION}")
message(STATUS "C++ Standard:        ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:          ${CMAKE_BUILD_TYPE}")
message(STATUS "Strict Warning Mode: ${BUILD_STRICT_MODE}")
message(STATUS "========================================")

# ##############################################################################
# 第三方库示例：SFML
# 如果你不用SFML，注释掉这一段，替换成别的库（fmt/spdlog等写法类似）
# ##############################################################################
# find_package：寻找系统安装的SFML库，REQUIRED找不到就直接报错终止
# COMPONENTS：指定需要的组件，不需要的组件删掉
find_package(SFML REQUIRED COMPONENTS Graphics Window System Network Audio)

# ========= AI_Framework 静态库 =========
file(GLOB_RECURSE AI_FRAMEWORK_SRCS
    "${CMAKE_SOURCE_DIR}/AI_Framework/*.cpp"
    "${CMAKE_SOURCE_DIR}/AI_Framework/*.h"
)

add_library(AI_Framework STATIC ${AI_FRAMEWORK_SRCS})
target_include_directories(AI_Framework PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(AI_Framework PRIVATE SFML::System)

# 启用预编译头
target_precompile_headers(AI_Framework PUBLIC "${CMAKE_SOURCE_DIR}/AI_Framework/pch.h")

# ##############################################################################
# 编译目标：可执行程序 
# ##############################################################################
file(GLOB_RECURSE GAME_SRCS
    "${CMAKE_SOURCE_DIR}/StealthGame/*.cpp"
    "${CMAKE_SOURCE_DIR}/StealthGame/*.h"
)
add_executable(StealthGame ${GAME_SRCS})

# 复用AI_Framework的预编译头
target_precompile_headers(StealthGame REUSE_FROM AI_Framework)

target_link_libraries(StealthGame PRIVATE
    AI_Framework
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
    SFML::Network
)

# 复制资源文件夹
file(COPY ${CMAKE_SOURCE_DIR}/StealthGame/Assets
     DESTINATION ${CMAKE_BINARY_DIR})

# ##############################################################################
# 安装规则：打包发布用，cmake --install build
# ##############################################################################
install(TARGETS StealthGame
    RUNTIME DESTINATION bin
)
