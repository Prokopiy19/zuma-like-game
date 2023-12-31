cmake_minimum_required(VERSION 3.27)
project("trgame")

# 1. Look for a SDL2 package, 2. look for the SDL2 component and 3. fail if none can be found
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

# 1. Look for a SDL2 package, 2. Look for the SDL2maincomponent and 3. DO NOT fail when SDL2main is not available
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

find_package(SDL2_ttf REQUIRED)

find_package(SDL2_image REQUIRED)

find_package(glm REQUIRED)

set(SRCS
    src/cursor.cpp
    src/debug.cpp
    src/main.cpp
    src/window.cpp
    src/game.cpp
    src/game_rect.cpp
    src/input.cpp
    src/balls.cpp
    src/path.cpp
    src/projectile.cpp
    src/shooter.cpp
    src/timer.cpp
    src/spline.cpp
    src/render.cpp
)
add_executable(trgame ${SRCS})

set_property(TARGET trgame PROPERTY CXX_STANDARD 20)

if(MSVC)
  target_compile_options(trgame PRIVATE /W4 /WX)
else()
  target_compile_options(trgame PRIVATE -Wall -Wextra -Wpedantic -Wno-sign-compare -Wno-unused-parameter)
endif()

# SDL2::SDL2main may or may not be available. It is e.g. required by Windows GUI applications
if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(trgame PRIVATE SDL2::SDL2main)
endif()

# Link to the actual SDL2 library. SDL2::SDL2 is the shared SDL library, SDL2::SDL2-static is the static SDL libarary.
target_link_libraries(trgame PRIVATE SDL2::SDL2)

target_link_libraries(trgame PRIVATE SDL2_ttf::SDL2_ttf)

target_link_libraries(trgame PRIVATE SDL2_image::SDL2_image)

target_link_libraries(trgame PRIVATE glm)