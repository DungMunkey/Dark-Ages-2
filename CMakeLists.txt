# Dark Ages II: Engel - Phase 1 build.
#
# Goal of this CMakeLists.txt: get the codebase compiling and linking again, exactly as it sits (SDL2 + SFML audio,
# flat file layout), with no restructuring or porting yet. That comes later once this baseline is proven.
#
# Dependencies are vendored under third_party/ (not committed - see .gitignore):
#   third_party/SDL2-2.0.12   - copied from the Dark-Ages (DA1) repo's own third_party (same version, already in use there)
#   third_party/SFML-2.6.1    - official SFML 2.6.1 Windows VC17 64-bit package (matches the sf::Music/sf::Time-era API
#                                the code already uses - SFML 3 renamed several of these calls)
#
# Usage:
#   cmake -S . -B build -G "Visual Studio 17 2022" -A x64
#   cmake --build build --config Debug

cmake_minimum_required(VERSION 3.16)
project(DarkAges2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DA2_SDL2_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL2-2.0.12" CACHE PATH "SDL2 dev package root")
set(DA2_SFML_DIR "${CMAKE_SOURCE_DIR}/third_party/SFML-2.6.1" CACHE PATH "SFML dev package root")

file(GLOB DA2_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/*.cpp")
add_executable(DarkAges2 ${DA2_SOURCES})
target_include_directories(DarkAges2 PRIVATE
  "${CMAKE_SOURCE_DIR}"
  "${DA2_SDL2_DIR}/include"
  "${DA2_SFML_DIR}/include"
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

target_link_directories(DarkAges2 PRIVATE "${DA2_SDL2_DIR}/lib/x64" "${DA2_SFML_DIR}/lib")
target_link_libraries(DarkAges2 PRIVATE
  SDL2 SDL2main
  $<$<CONFIG:Debug>:sfml-audio-s-d> $<$<NOT:$<CONFIG:Debug>>:sfml-audio-s>
  $<$<CONFIG:Debug>:sfml-system-s-d> $<$<NOT:$<CONFIG:Debug>>:sfml-system-s>
  flac ogg vorbis vorbisenc vorbisfile openal32 winmm
)
# Statically link SFML itself (its official Windows package does not export data members like
# sf::Time::Zero from the DLL import libs - only the static libs contain them). OpenAL remains a
# runtime DLL dependency even in SFML's static build.
target_compile_definitions(DarkAges2 PRIVATE SFML_STATIC)

# Copy the runtime DLLs next to the executable after each build.
add_custom_command(TARGET DarkAges2 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DA2_SDL2_DIR}/lib/x64/SDL2.dll" "${CMAKE_SOURCE_DIR}/"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${DA2_SFML_DIR}/bin/openal32.dll" "${CMAKE_SOURCE_DIR}/"
)
