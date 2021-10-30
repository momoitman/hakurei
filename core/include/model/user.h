#pragma once

#include "model_base.h"

#include <string>
#include <optional>
#include <utility>

namespace hakurei::core
{
namespace model
{
struct serializer;

/*
 * The user model.
 * Any modification of the structure of this model must be reflected to
 * the serializer!
 */
class user
{
    friend struct serializer;
public:
    user() = default;

    user(std::string id, std::string name, std::string password_obfus,
        std::string contact, std::string address, int balance_cents)
        : _id(std::move(id)),
          _name(std::move(name)),
          _password_obfus(std::move(password_obfus)),
          _contact(std::move(contact)),
          _address(std::move(address)),
          _balance_cents(balance_cents)
    {
    }

    user(std::string id, std::string name,
         std::string contact, std::string address, int balance_cents)
        : _id(std::move(id)),
          _name(std::move(name)),
          _contact(std::move(contact)),
          _address(std::move(address)),
          _balance_cents(balance_cents)
    {
        // Password not present
    }

    [[nodiscard]] std::string const& id() const { return _id; } 
    [[nodiscard]] std::string const& name() const { return _name; }
    [[nodiscard]] std::optional<std::string> password_obfus() const { return _password_obfus; }
    [[nodiscard]] std::string const& contact() const { return _contact; }
    [[nodiscard]] std::string const& address() const { return _address; }
    [[nodiscard]] int balance_cents() const { return _balance_cents; }

    void set_name(const std::string& name) { _name = name; }
    void set_password_obfus(const std::optional<std::string>& password_obfus) { _password_obfus = password_obfus; }
    void set_contact(const std::string& contact) { _contact = contact; }
    void set_address(const std::string& address) { _address = address; }
    void set_balance_cents(int balance_cents) { _balance_cents = balance_cents; }

    [[nodiscard]] user safe_copy() const
    {
        return user(_id, _name, _contact, _address, _balance_cents);
    }

    void safe_copy_to(user& dest) const
    {
        dest._id = _id;
        dest._name = _name;
        dest._password_obfus.reset();
        dest._contact = _contact;
        dest._address = _address;
        dest._balance_cents = _balance_cents;
    }
private:
    std::string _id;
    std::string _name;
    std::optional<std::string> _password_obfus;
    std::string _contact;
    std::string _address;
    int _balance_cents;
};

}  // namespace model
}  // namespace hakurei::core