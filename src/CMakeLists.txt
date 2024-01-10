project(MatVK)


# Build
add_library(MatVK)
target_sources(MatVK PRIVATE matvk.cpp)

target_include_directories(MatVK PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

find_package(Vulkan REQUIRED)
target_include_directories(MatVK PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(MatVK PUBLIC ${Vulkan_LIBRARIES})


# Install
install(TARGETS MatVK EXPORT MatVK
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/MatVK)
