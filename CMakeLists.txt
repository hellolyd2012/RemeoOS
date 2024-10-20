# CMake 最小版本要求
cmake_minimum_required(VERSION 3.12)

# 项目名称
project(RemeoOS)

# 编译器
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_LINKER ld)

# 目录
set(IncludeDir ${CMAKE_SOURCE_DIR}/Includes)
set(KernelDir ${CMAKE_SOURCE_DIR}/Kernel)
set(BuildDir ${CMAKE_BINARY_DIR})
set(ObjectDir ${BuildDir}/Obj)

# 编译选项
set(CFLAGS "-std=c99 -nostdlib")
set(CPPFLAGS "-nostdlib -std=c++23")

# 创建目录
file(MAKE_DIRECTORY ${BuildDir})
file(MAKE_DIRECTORY ${ObjectDir})
file(MAKE_DIRECTORY ${BuildDir}/Boot)

# 编译BootLoader
add_library(Boot STATIC ${CMAKE_SOURCE_DIR}/Boot/Boot.cpp)
target_compile_options(Boot PRIVATE ${CPPFLAGS})
set_target_properties(Boot PROPERTIES OUTPUT_NAME "Boot" ARCHIVE_OUTPUT_DIRECTORY "${BuildDir}/Boot")

# 添加构建日志
add_custom_command(TARGET Boot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "@${CMAKE_CXX_COMPILER} ${CPPFLAGS} -c ${CMAKE_SOURCE_DIR}/Boot/Boot.cpp -o ${BuildDir}/Boot/Boot.o" > ${BuildDir}/Build.log
)

# 清理目标
add_custom_target(Clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${BuildDir}
    COMMENT "Cleaning build directory"
)

# 默认目标
add_custom_target(all DEPENDS Boot)