#include "model/serialization.h"

#include <gtest/gtest.h>

#include "model/item.h"
#include "model/order.h"
#include "model/user.h"
#include "persistence/table.h"

#include <chrono>
#include <date/date.h>

using namespace hakurei::core::model;
using namespace hakurei::core::persistence;

TEST(Hakurei_model_test, user_model)
{
    user u(
        "U00001", "Minato Aqua", "$$$$$$",
        "14917277", "Tokyo Bay", 150000);
    EXPECT_EQ(u.id(), "U00001");
    EXPECT_EQ(u.name(), "Minato Aqua");
    EXPECT_EQ(u.password_obfus(), "$$$$$$");
    EXPECT_EQ(u.contact(), "14917277");
    EXPECT_EQ(u.address(), "Tokyo Bay");
    EXPECT_EQ(u.balance_cents(), 150000);

    u.set_name("Kagura Mea");
    u.set_password_obfus("$$$");
    u.set_contact("12235923");
    u.set_address("Mount Tengu");
    u.set_balance_cents(5000000);

    EXPECT_EQ(u.id(), "U00001");
    EXPECT_EQ(u.name(), "Kagura Mea");
    EXPECT_EQ(u.password_obfus(), "$$$");
    EXPECT_EQ(u.contact(), "12235923");
    EXPECT_EQ(u.address(), "Mount Tengu");
    EXPECT_EQ(u.balance_cents(), 5000000);
    
    auto u2 = u.safe_copy();
    EXPECT_EQ(u2.id(), "U00001");
    EXPECT_EQ(u2.name(), "Kagura Mea");
    EXPECT_EQ(u2.password_obfus(), std::optional<std::string>());
    EXPECT_EQ(u2.contact(), "12235923");
    EXPECT_EQ(u2.address(), "Mount Tengu");
    EXPECT_EQ(u2.balance_cents(), 5000000);

    u.set_address("Ichikara corp");
    u.set_password_obfus("______");
    EXPECT_EQ(u2.address(), "Mount Tengu"); // Change in parent object must not affact copied
    EXPECT_EQ(u2.password_obfus(), std::optional<std::string>());

    u2.set_address("774 Inc.");
    EXPECT_EQ(u.address(), "Ichikara corp"); // Change in copied object must not affact parent

    u.safe_copy_to(u2);
    EXPECT_EQ(u2.address(), "Ichikara corp");
    EXPECT_EQ(u2.password_obfus(), std::optional<std::string>());
}

TEST(Hakurei_model_test, user_serialization)
{
    table::row_t row;
    user u(
        "U00001", "Minato Aqua", "$$$$$$",
        "14917277", "Tokyo Bay", 150000);
    serializer::serialize(u, row);

    EXPECT_EQ(row[0], table::cell_t("U00001"));
    EXPECT_EQ(row[1], table::cell_t("Minato Aqua"));
    EXPECT_EQ(row[2], table::cell_t("$$$$$$"));
    EXPECT_EQ(row[3], table::cell_t("14917277"));
    EXPECT_EQ(row[4], table::cell_t("Tokyo Bay"));
    EXPECT_EQ(row[5], table::cell_t(150000));

    user u2;
    serializer::deserialize(row, u2);
    EXPECT_EQ(u2.id(), "U00001");
    EXPECT_EQ(u2.name(), "Minato Aqua");
    EXPECT_EQ(u2.password_obfus(), "$$$$$$");
    EXPECT_EQ(u2.contact(), "14917277");
    EXPECT_EQ(u2.address(), "Tokyo Bay");
    EXPECT_EQ(u2.balance_cents(), 150000);

    u2.set_name("Mononobe Alice");
    EXPECT_EQ(u.name(), "Minato Aqua");

    u.set_name("Nana7mi");
    EXPECT_EQ(u2.name(), "Mononobe Alice");
}

TEST(Hakurei_model_test, item_model)
{
    using namespace std::chrono;
    using namespace date;
    item i(
        "M00001", "Takehana Note Figma 1:1", 10000000,
        "Takehana-mama's 1:1 figma!!", "U00001",
        sys_days(2021_y / October / 1) + 12h, item_status::on_stock); // 2021-10-1 10:00 UTC
    EXPECT_EQ(i.id(), "M00001");
    EXPECT_EQ(i.name(), "Takehana Note Figma 1:1");
    EXPECT_EQ(i.price_cents(), 10000000);
    EXPECT_EQ(i.description(), "Takehana-mama's 1:1 figma!!");
    EXPECT_EQ(duration_cast<seconds>(i.on_stock_time().time_since_epoch()).count(), 1633089600);  // 2021-10-1 10:00 UTC
    EXPECT_EQ(i.status(), item_status::on_stock);
}

TEST(Hakurei_model_test, item_serialization)
{
    table::row_t row;
    using namespace std::chrono;
    using namespace date;
    item i(
        "M00001", "Takehana Note Figma 1:1", 10000000,
        "Takehana-mama's 1:1 figma!!", "U00001",
        sys_days(2021_y / October / 1) + 12h, item_status::on_stock);  // 2021-10-1 10:00 UTC
    serializer::serialize(i, row);
    EXPECT_EQ(row[0], table::cell_t("M00001"));
    EXPECT_EQ(row[1], table::cell_t("Takehana Note Figma 1:1"));
    EXPECT_EQ(row[2], table::cell_t(10000000));
    EXPECT_EQ(row[3], table::cell_t("Takehana-mama's 1:1 figma!!"));
    EXPECT_EQ(row[4], table::cell_t("U00001"));
    EXPECT_EQ(row[5], table::cell_t(static_cast<std::int64_t>(1633089600000)));  // 2021-10-1 10:00 UTC
    EXPECT_EQ(row[6], table::cell_t(1));

    item i2;
    serializer::deserialize(row, i2);
    EXPECT_EQ(i2.id(), "M00001");
    EXPECT_EQ(i2.name(), "Takehana Note Figma 1:1");
    EXPECT_EQ(i2.price_cents(), 10000000);
    EXPECT_EQ(i2.description(), "Takehana-mama's 1:1 figma!!");
    EXPECT_EQ(duration_cast<seconds>(i2.on_stock_time().time_since_epoch()).count(), 1633089600);  // 2021-10-1 10:00 UTC
    EXPECT_EQ(i2.status(), item_status::on_stock);

    i2.set_name("Inuyama Tamaki Pillow");
    EXPECT_EQ(i.name(), "Takehana Note Figma 1:1");

    i.set_name("Mr.Quin Mark Cup");
    EXPECT_EQ(i2.name(), "Inuyama Tamaki Pillow");
}

TEST(Hakurei_model_test, order_model)
{
    using namespace std::chrono;
    using namespace date;
    order o(
        "T00001", "M00001", 10000000,
        sys_days(2021_y / October / 1) + 12h, // 2021-10-1 10:00 UTC
        "U00001", "U00002");
    EXPECT_EQ(o.id(), "T00001");
    EXPECT_EQ(o.item_id(), "M00001");
    EXPECT_EQ(o.price_cents(), 10000000);
    EXPECT_EQ(duration_cast<seconds>(o.time().time_since_epoch()).count(), 1633089600);  // 2021-10-1 10:00 UTC
    EXPECT_EQ(o.seller_uid(), "U00001");
    EXPECT_EQ(o.customer_uid(), "U00002");
}

TEST(Hakurei_model_test, order_serialization)
{
    table::row_t row;
    using namespace std::chrono;
    using namespace date;
    order o(
        "T00001", "M00001", 10000000,
        sys_days(2021_y / October / 1) + 12h,  // 2021-10-1 10:00 UTC
        "U00001", "U00002");

    serializer::serialize(o, row);
    EXPECT_EQ(row[0], table::cell_t("T00001"));
    EXPECT_EQ(row[1], table::cell_t("M00001"));
    EXPECT_EQ(row[2], table::cell_t(10000000));
    EXPECT_EQ(row[3], table::cell_t(static_cast<std::int64_t>(1633089600000)));  // 2021-10-1 10:00 UTC
    EXPECT_EQ(row[4], table::cell_t("U00001"));
    EXPECT_EQ(row[5], table::cell_t("U00002"));  // 2021-10-1 10:00 UTC

    order o2;
    serializer::deserialize(row, o2);

    EXPECT_EQ(o2.id(), "T00001");
    EXPECT_EQ(o2.item_id(), "M00001");
    EXPECT_EQ(o2.price_cents(), 10000000);
    EXPECT_EQ(duration_cast<seconds>(o2.time().time_since_epoch()).count(), 1633089600);  // 2021-10-1 10:00 UTC
    EXPECT_EQ(o2.seller_uid(), "U00001");
    EXPECT_EQ(o2.customer_uid(), "U00002");
}