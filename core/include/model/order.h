#pragma once

#include "model_base.h"

#include <string>

namespace hakurei::core
{
namespace model
{
struct serializer;

/*
 * The order model.
 * Any modification of the structure of this model must be reflected to
 * the serializer!
 */
class order
{
    friend struct serializer;
public:
    order() = default;
    order(std::string id, std::string item_id, int price_cents, 
        datetime_t time, std::string seller_uid, std::string customer_uid)
        : _id(std::move(id)),
          _item_id(std::move(item_id)),
          _price_cents(price_cents),
          _time(std::move(time)),
          _seller_uid(std::move(seller_uid)),
          _customer_uid(std::move(customer_uid))
    {
    }

    [[nodiscard]] std::string id() const { return _id; }
    [[nodiscard]] std::string item_id() const { return _item_id; }
    [[nodiscard]] int price_cents() const { return _price_cents; }
    [[nodiscard]] datetime_t time() const { return _time; }
    [[nodiscard]] std::string seller_uid() const { return _seller_uid; }
    [[nodiscard]] std::string customer_uid() const { return _customer_uid; }

    void set_item_id(const std::string& item_id) { _item_id = item_id; }
    void set_price_cents(int price_cents) { _price_cents = price_cents; }
    void set_time(const datetime_t& time) { _time = time; }
    void set_seller_uid(const std::string& seller_uid) { _seller_uid = seller_uid; }
    void set_customer_uid(const std::string& customer_uid) { _customer_uid = customer_uid; }

private:
    std::string _id;
    std::string _item_id;
    int _price_cents;
    datetime_t _time;
    std::string _seller_uid;
    std::string _customer_uid;
};

}  // namespace model
}  // namespace hakurei::core