project(MatVK)

enable_testing()

include(FetchContent)


# Googletest

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# Eigen

FetchContent_Declare(
    Eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
#set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(EIGEN_MPL2_ONLY ON)
set(EIGEN_BUILD_TESTING OFF)
set(EIGEN_BUILD_DOC OFF)
set(EIGEN_BUILD_PKGCONFIG OFF)
FetchContent_MakeAvailable(Eigen)


add_executable(tests
    "computeTest.hpp"       "computeTest.cpp"
)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../include)
target_link_libraries(tests PRIVATE 
    MatVK
    GTest::gtest_main
    Eigen3::Eigen
)


include(GoogleTest)
gtest_discover_tests(tests)
