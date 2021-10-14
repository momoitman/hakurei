#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "internal_exceptions.h"
#include "service/services.h"

#include <fstream>
#include <filesystem>
#include <toml.hpp>
#include <spdlog/spdlog.h>

using namespace std::literals;
using namespace hakurei::core;
using namespace hakurei::core::service;

const std::string setting_filename = "./settings.toml";
const std::string test_data_basic_path = "./test_data";

class Hakurei_service_test : public ::testing::Test
{
protected:
    std::unique_ptr<service_injector> _ij;

    void make_setting() {
        // Create test settings.
        auto setting_table = toml::table{
            {{"auth", toml::table{
                          {{"admin_user_name", "test_admin"}}}},
             {"persistence", toml::table{{{"csv", toml::table{{{"base_path", test_data_basic_path}}}}}}}}};
        std::filesystem::remove(setting_filename);

        std::ofstream setting_fs;
        setting_fs.exceptions(std::ios_base::badbit | std::ios_base::failbit);
        setting_fs.open(setting_filename);
        setting_fs << setting_table;
        setting_fs.close();
    }

    void clean_data()
    {
        std::error_code ec;
        std::filesystem::remove_all(test_data_basic_path, ec);
    }

    void SetUp() override
    {
        Test::SetUp();

        make_setting();
        _ij = std::make_unique<service_injector>(get_services_component);
    }

    void TearDown() override
    {
        Test::TearDown();
        clean_data();
    }
};

TEST_F(Hakurei_service_test, auth_service_test)
{
    try
    {
        auto auth_svc = _ij->get<auth_service*>();
        auto token_admin_1 =
            auth_svc->register_user(
                "admin"sv,
                "123456"sv,
                "1891145141919"sv,
                "NJU Xianlin Camp. Nanjing, Jiangsu"sv);
        EXPECT_TRUE(auth_svc->is_user_admin(token_admin_1));
        auth_svc->logout_user(token_admin_1);
    }
    catch (hakurei_error const& err)
    {
        spdlog::critical("[EXCEPTION THROWN] {}:{}", typeid(err).name(), err.reason());
    }
}