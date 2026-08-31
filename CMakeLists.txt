cmake_minimum_required(VERSION 3.20)

project(2048 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL3_DIR "D:/SDL/SDL3-3.4.14/x86_64-w64-mingw32/lib/cmake/SDL3")
set(SDL3_ttf_DIR "D:/SDL/SDL3_ttf-3.2.2/x86_64-w64-mingw32/lib/cmake/SDL3_ttf")

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

add_executable(2048
    cli/main.cc
    common/nums.cc
)

target_include_directories(2048 PRIVATE common)

add_executable(board_test
    gui/sdl_board_test.cc
)

target_link_libraries(board_test PRIVATE
    SDL3::SDL3
)

# 构建完成后自动拷贝 SDL3 运行时 DLL 到 exe 所在目录
# 这样清空 build 重新构建后不会丢失 DLL
set(SDL3_DLL "D:/SDL/SDL3-3.4.14/x86_64-w64-mingw32/bin/SDL3.dll")
set(SDL3_TTF_DLL "D:/SDL/SDL3_ttf-3.2.2/x86_64-w64-mingw32/bin/SDL3_ttf.dll")

function(copy_sdl_dlls target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL3_DLL}" "${SDL3_TTF_DLL}"
            $<TARGET_FILE_DIR:${target}>
    )
endfunction()

copy_sdl_dlls(board_test)

# sdl_window_test.cc 仅保留作为 SDL3 API 参考，不参与构建
