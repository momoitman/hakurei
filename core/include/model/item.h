#pragma once

#include "model_base.h"

#include <string>
#include <cstdint>
#include <chrono>
#include <utility>

namespace hakurei::core
{
namespace model
{
struct serializer;

/*
 * The item status enum.
 * Any modification of the values of this enum must be reflected to
 * the serializer!
 */
enum class item_status : std::uint8_t
{
    on_stock = 1,
    sold = 2,
    deleted = 3
};

/*
 * The item model.
 * Any modification of the structure of this model must be reflected to
 * the serializer!
 */
class item
{
    friend struct serializer;
public:
    item() = default;
    item(std::string id, std::string name, int price_cents, std::string description, 
        std::string seller_uid, const datetime_t& on_stock_time, item_status status)
        : _id(std::move(id)),
          _name(std::move(name)),
          _price_cents(price_cents),
          _description(std::move(description)),
          _seller_uid(std::move(seller_uid)),
          _on_stock_time(on_stock_time),
          _status(status)
    {
    }

    [[nodiscard]] std::string id() const { return _id; }
    [[nodiscard]] std::string name() const { return _name; }
    [[nodiscard]] int price_cents() const { return _price_cents; }
    [[nodiscard]] std::string description() const { return _description; }
    [[nodiscard]] std::string seller_uid() const { return _seller_uid; }
    [[nodiscard]] datetime_t on_stock_time() const { return _on_stock_time; }
    [[nodiscard]] item_status status() const { return _status; }

    void set_name(const std::string& name) { _name = name; }
    void set_price_cents(int price_cents) { _price_cents = price_cents; }
    void set_description(const std::string& description) { _description = description; }
    void set_seller_uid(const std::string& seller_uid) { _seller_uid = seller_uid; }
    void set_on_stock_time(const datetime_t& on_stock_time) { _on_stock_time = on_stock_time; }
    void set_status(item_status status) { _status = status; }

private:
    std::string _id;
    std::string _name;
    int _price_cents;
    std::string _description;
    std::string _seller_uid;
    datetime_t _on_stock_time;
    item_status _status;
};

}
}  // namespace hakurei::core