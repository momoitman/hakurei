#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "internal_exceptions.h"
#include "service/services.h"
#include "service/service_exceptions.h"

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
        clean_data();
        _ij = std::make_unique<service_injector>(get_services_component);
    }

    void TearDown() override
    {
        Test::TearDown();
        
    }
};

TEST_F(Hakurei_service_test, services_test)
{
    try
    {
        auto auth_svc = _ij->get<auth_service*>();
        auto token_admin_1 =
            auth_svc->register_user(
                "test_admin"sv,
                "123456abcd"sv,
                "1891145141919"sv,
                "NJU Xianlin Camp. Nanjing, Jiangsu"sv);
        EXPECT_TRUE(auth_svc->is_user_admin(token_admin_1));
        EXPECT_THROW(auth_svc->set_user_info(token_admin_1, "admin"sv, std::nullopt, std::nullopt, std::nullopt), access_denied_error);
        auth_svc->set_user_info(token_admin_1, std::nullopt, std::nullopt, "abcdefg"sv, std::nullopt);
        EXPECT_EQ(auth_svc->get_user_info(token_admin_1).contact(), "abcdefg");
        auth_svc->logout_user(token_admin_1);
        EXPECT_THROW(auth_svc->get_user_info(token_admin_1), invalid_token_error);

        auto token_normal_1 =
            auth_svc->register_user(
                "test_normal"sv,
                "abcd123456"sv,
                "18925252121"sv,
                "NJU Gulou Camp. Nanjing, Jiangsu"sv);
        auto token_normal_2 =
            auth_svc->register_user(
                "test_normal_2"sv,
                "abcd123456"sv,
                "18925252121"sv,
                "NJU Gulou Camp. Nanjing, Jiangsu"sv);

        EXPECT_THROW(auth_svc->login_user("admin"sv, "abcdef"sv), invalid_credential_error);
        EXPECT_THROW(auth_svc->login_user("test_admin"sv, "abcdef"sv), invalid_credential_error);
        auto token_admin_2 = auth_svc->login_user("test_admin", "123456abcd");
        std::vector<model::user> users_temp;
        EXPECT_THROW(auth_svc->get_all_users(token_normal_1, users_temp), access_denied_error);
        auth_svc->get_all_users(token_admin_2, users_temp);
        EXPECT_EQ(users_temp.size(), 3);

        EXPECT_EQ(auth_svc->get_user_name("U00001").value(), "test_admin");
        EXPECT_EQ(auth_svc->get_user_info("U00001").value().name(), "test_admin");

        EXPECT_THROW(auth_svc->deposit_money(token_admin_2, -100), invalid_argument_error);
        auth_svc->deposit_money(token_admin_2, 1000);
        EXPECT_EQ(auth_svc->get_user_balance_cents(token_admin_2), 1000);

        auto item_svc = _ij->get<item_service*>();
        item_svc->add_item(token_normal_1, "Item1", 150, "Descrip");
        item_svc->set_item(token_normal_1, "M00001", "Item1_1", 100, "Descrip2");
        EXPECT_EQ(item_svc->get_item("M00001").value().description(), "Descrip2");
        EXPECT_THROW(item_svc->set_item(token_admin_2, "M00001", std::nullopt, std::nullopt, std::nullopt), access_denied_error);

        std::vector<model::item> items_temp;
        item_svc->get_my_items(token_normal_1, items_temp);
        EXPECT_EQ(items_temp.size(), 1);
        EXPECT_EQ(items_temp[0].status(), model::item_status::on_stock);

        item_svc->remove_item(token_normal_1, "M00001");
        item_svc->get_my_items(token_normal_1, items_temp);
        EXPECT_EQ(items_temp[0].status(), model::item_status::deleted);

        item_svc->add_item(token_normal_1, "Item2", 150, "Descrip");
        item_svc->get_all_items(token_admin_2, items_temp);
        EXPECT_EQ(items_temp.size(), 2);

        item_svc->search_item(token_normal_1, "Item"sv, items_temp);
        EXPECT_EQ(items_temp.size(), 1);
        EXPECT_EQ(items_temp[0].name(), "Item2");

        auth_svc->remove_user(token_admin_2, "U00002");
        item_svc->get_all_items(token_admin_2, items_temp);
        EXPECT_EQ(items_temp[0].status(), model::item_status::deleted);
        EXPECT_EQ(items_temp[1].status(), model::item_status::deleted);

        auth_svc->deposit_money(token_normal_2, 1500);
        auto item_id1 = item_svc->add_item(token_normal_2, "Item3", 150, "Descrip");
        auto item_id2 = item_svc->add_item(token_normal_2, "Item4", 15000, "Descrip");
        auto item_id3 = item_svc->add_item(token_normal_2, "Item5", 150, "Descrip");

        auto order_svc = _ij->get<order_service*>();
        order_svc->place_order(token_admin_2, item_id1);
        order_svc->place_order(token_normal_2, item_id3);
        EXPECT_EQ(auth_svc->get_user_info(token_admin_2).balance_cents(), 850); // 1000 - 150
        EXPECT_THROW(order_svc->place_order(token_admin_2, item_id2), insufficient_balance_error);

        std::vector<model::order> orders_temp;
        EXPECT_THROW(order_svc->get_all_orders(token_normal_2, orders_temp), access_denied_error);
        order_svc->get_all_orders(token_admin_2, orders_temp);
        EXPECT_EQ(orders_temp.size(), 2);

        order_svc->get_my_bought_orders(token_admin_2, orders_temp);
        EXPECT_EQ(orders_temp.size(), 1);
        EXPECT_EQ(orders_temp[0].item_id(), item_id1);

        order_svc->get_my_sold_orders(token_normal_2, orders_temp);
        EXPECT_EQ(orders_temp[0].item_id(), item_id1);
    }
    catch (hakurei_error const& err)
    {
        spdlog::critical("[EXCEPTION THROWN] {}:{}", typeid(err).name(), err.reason());
        ASSERT_TRUE(false);
    }
}