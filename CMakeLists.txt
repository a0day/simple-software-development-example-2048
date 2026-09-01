cmake_minimum_required(VERSION 3.20)

project(2048 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL3_DIR "D:/SDL/SDL3-3.4.14/x86_64-w64-mingw32/lib/cmake/SDL3")
set(SDL3_ttf_DIR "D:/SDL/SDL3_ttf-3.2.2/x86_64-w64-mingw32/lib/cmake/SDL3_ttf")

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

add_executable(2048_cli
    cli/main.cc
    common/nums.cc
)

target_include_directories(2048_cli PRIVATE common)

add_executable(2048_gui
    gui/main.cc
    gui/board.cc
    common/nums.cc
)

target_include_directories(2048_gui PRIVATE common)

target_link_libraries(2048_gui PRIVATE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)

# 构建完成后自动拷贝 SDL3 运行时 DLL 到 exe 所在目录 根据自己下载的 SDL3 地址修改路径
# 这样清空 build 重新构建后不会丢失 DLL
set(SDL3_DLL "D:/SDL/SDL3-3.4.14/x86_64-w64-mingw32/bin/SDL3.dll")
set(SDL3_TTF_DLL "D:/SDL/SDL3_ttf-3.2.2/x86_64-w64-mingw32/bin/SDL3_ttf.dll")

# 把运行期资源（DLL、字体等）拷贝到目标 exe 所在目录
# 用法: copy_assets(<目标名> <源文件>...)
function(copy_assets target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ARGN}
            $<TARGET_FILE_DIR:${target}>
    )
endfunction()

copy_assets(2048_gui
    "${SDL3_DLL}"
    "${SDL3_TTF_DLL}"
    "${CMAKE_SOURCE_DIR}/resources/arial.ttf" #字体文件
)
