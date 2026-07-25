cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_COMPILER "C:/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")
project(mik64_blog_builder)
if(NOT TARGET mik64_blog_builder)
    add_subdirectory(lib/mik64_logger)
    add_library(mik64_blog_builder STATIC
        "builder/cpp/builder.cpp"
    )
    add_library(mik64::blog-builder ALIAS mik64_blog_builder)
    target_include_directories(mik64_blog_builder PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/builder/h>)
    target_compile_features(mik64_blog_builder PRIVATE cxx_std_17)

    target_link_libraries(mik64_blog_builder PRIVATE mik64::logger)
endif()
add_executable(mik64_blog_builder_test builder/cpp/main.cpp)
target_link_libraries(mik64_blog_builder_test PRIVATE mik64::blog-builder mik64::logger)