cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

project(MatVK)



# Build
add_subdirectory(include)
add_subdirectory(src)

option(BUILD_EXAMPLES "Build examples or not" ON)
if(BUILD_EXAMPLES)
    message(STATUS "Building examples...")
    add_subdirectory(examples)
endif()

option(BUILD_TESTS "Build tests or not" ON)
if(BUILD_TESTS)
    message(STATUS "Building tests...")
    add_subdirectory(tests)
endif()



# Install
include(GNUInstallDirs)

install(FILES cmake/MatVKConfig.cmake
    DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)
install(EXPORT MatVK
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/MatVK)
