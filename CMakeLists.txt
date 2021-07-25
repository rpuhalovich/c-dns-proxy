cmake_minimum_required(VERSION 3.17.1)
project(c-dns-proxy C)
set(CMAKE_C_STANDARD 99)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER Predefined)

if($ENV{CLION_IDE})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,leak -g -O2")
endif()

set(phase1 phase1)
set(phase2 dns_svr)

# --- sources ------------------------------------------------------------------
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE shared ${SRC_DIR}/shared/*.c ${SRC_DIR}/shared/*.h)
set(${phase1}_sources ${shared} ${SRC_DIR}/${phase1}.c)
set(${phase2}_sources ${shared} ${SRC_DIR}/${phase2}.c)

# --- executables --------------------------------------------------------------
add_executable(${phase1} ${${phase1}_sources})
target_include_directories(${phase1} PUBLIC ${SRC_DIR}/shared)
target_link_libraries(${phase1} m)
source_group(TREE ${SRC_DIR} FILES ${${phase1}_sources})

add_executable(${phase2} ${${phase2}_sources})
target_include_directories(${phase2} PUBLIC ${SRC_DIR}/shared)
target_link_libraries(${phase2} m)
source_group(TREE ${SRC_DIR} FILES ${${phase2}_sources})
