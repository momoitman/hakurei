cmake_minimum_required (VERSION 3.16)
cmake_policy(SET CMP0074 NEW)
set(CMAKE_CXX_STANDARD 17)

project("hakurei_core")

add_library(hakurei_core 
    "src/core_main.cpp"

    "src/util/setting.cpp"
    "src/util/argon2_password_hasher.cpp"
    "src/util/string_sanitizer.cpp"

    "src/persistence/abstract_search_engine.cpp"
    "src/persistence/abstract_persistence.cpp"
    "src/persistence/csv_persistence.cpp"
    "src/persistence/basic_search_engine.cpp"
    
    "src/model/serialization.cpp"
    "src/model/repository.cpp"

    "src/service/id_generator.cpp"
    "src/service/auth_service.cpp"
    "src/service/item_service.cpp"
    "src/service/order_service.cpp"
    "src/service/services.cpp"
)
target_include_directories(hakurei_core PRIVATE "include" "../vendor" "src")
target_link_libraries(hakurei_core stdc++fs)
target_link_libraries(hakurei_core 
    spdlog::spdlog GTest::GTest fmt::fmt date::date
    tomlplusplus::tomlplusplus fruit::fruit argon2::argon2
)

enable_testing()
add_executable(hakurei_core_test
    "test/util/argon2_password_hasher_test.cpp"
    "test/util/string_sanitizer_test.cpp"

    "test/persistence/table_test.cpp"
    "test/persistence/memory_persistence_test.cpp"
    "test/persistence/csv_persistence_test.cpp"

    "test/model/serialization_test.cpp"

    "test/service/id_generator_test.cpp"
    "test/service/service_intg_test.cpp"
)
target_include_directories(hakurei_core_test PRIVATE "include" "../vendor" "src" "test")
target_link_libraries(hakurei_core_test hakurei_core)
include(GoogleTest)
gtest_discover_tests(hakurei_core_test)