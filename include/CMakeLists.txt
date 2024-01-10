project(MatVK)

find_package(Vulkan REQUIRED)
include_directories(PUBLIC ${Vulkan_INCLUDE_DIRS})

# Install
install(FILES "MatVK/matvk.hpp"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MatVK)
