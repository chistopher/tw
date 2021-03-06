
project(CPT)

find_package(girgs REQUIRED)
find_package(OpenMP REQUIRED)

# CMake version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set (CMAKE_CXX_STANDARD 17)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wshadow -Wfloat-equal -Wconversion)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
                -fno-sanitize-recover=all
        )
        add_link_options(
                -fsanitize=address,undefined
                -fno-omit-frame-pointer)
        # add_compile_definitions(_GLIBCXX_DEBUG) # gcc only
    endif()
endif()

set(HEADER 
    src/setstuff.h
    src/graphstuff.h)

set(SOURCES
    src/graphstuff.cpp)

add_executable(runner ${HEADER} ${SOURCES} src/main.cpp)
target_link_libraries(runner girgs::girgs)
target_link_libraries(runner OpenMP::OpenMP_CXX)
