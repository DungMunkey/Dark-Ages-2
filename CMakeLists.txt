# Dark Ages II: Engel - CMake build.
#
# SDL3 + SDL3_mixer, statically linked, source under src/. Mirrors the Dark-Ages (DA1) sibling project's approach.
#
# Dependencies are vendored under third_party/ (not committed - see .gitignore):
#   third_party/sdl3   - copied from the Dark-Ages repo's own third_party/sdl3 (built by its tools/get-deps.ps1;
#                         same SDL3 + SDL3_mixer static libs, proven there to play .ogg files via the MIX_ API)
#
# Usage:
#   cmake -S . -B build -G "Visual Studio 17 2022" -A x64
#   cmake --build build --config Release

cmake_minimum_required(VERSION 3.16)
project(DarkAges2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DA2_SDL3_DIR "${CMAKE_SOURCE_DIR}/third_party/sdl3" CACHE PATH "SDL3 + SDL3_mixer static build root")

file(GLOB DA2_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(DarkAges2 ${DA2_SOURCES})
target_include_directories(DarkAges2 PRIVATE
  "${CMAKE_SOURCE_DIR}/src"
  "${DA2_SDL3_DIR}/include"
)

if(MSVC)
  target_compile_definitions(DarkAges2 PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
set_target_properties(DarkAges2 PROPERTIES DEBUG_POSTFIX "_d")   # DarkAges2_d.exe for Debug, so it doesn't clobber Release

# The executable goes next to the game's assets (Data/, Gfx/, music/ live in the repo root), whatever the
# generator or configuration, since the code loads them via relative paths.
set_target_properties(DarkAges2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
foreach(cfg DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
  set_target_properties(DarkAges2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${cfg} "${CMAKE_SOURCE_DIR}")
endforeach()

target_link_directories(DarkAges2 PRIVATE "${DA2_SDL3_DIR}/lib/$<CONFIG>")
target_link_libraries(DarkAges2 PRIVATE
  SDL3_mixer-static SDL3-static
  user32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 setupapi shell32
)
