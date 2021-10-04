cmake_minimum_required (VERSION 3.16)
cmake_policy(SET CMP0074 NEW)
set(CMAKE_CXX_STANDARD 17)

project("hakurei_core")

add_library(hakurei_core 
    "src/core_main.cpp"

    "src/util/setting.cpp"

    "src/persistence/abstract_persistence.cpp"
    "src/persistence/csv_persistence.cpp"

    "src/model/user.cpp"
    "src/model/order.cpp"
    "src/model/item.cpp"
    "src/model/serialization.cpp"
    "src/model/repository.cpp"

    "src/service/auth_service.cpp"
)
target_include_directories(hakurei_core PRIVATE "include" "../vendor" "src")
target_link_libraries(hakurei_core stdc++fs)
target_link_libraries(hakurei_core spdlog::spdlog GTest::GTest fmt::fmt date::date tomlplusplus::tomlplusplus)

enable_testing()
add_executable(hakurei_core_test
    "test/persistence/table_test.cpp"
    "test/persistence/memory_persistence_test.cpp"
    "test/persistence/csv_persistence_test.cpp"
    "test/model/serialization_test.cpp"
)
target_include_directories(hakurei_core_test PRIVATE "include" "../vendor" "src" "test")
target_link_libraries(hakurei_core_test hakurei_core)
include(GoogleTest)
gtest_discover_tests(hakurei_core_test)