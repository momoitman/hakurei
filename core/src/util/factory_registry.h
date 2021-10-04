#pragma once

#include <stdexcept>
#include <unordered_map>

namespace hakurei::core
{
class factory_not_found : std::runtime_error
{
public:
    explicit factory_not_found(const char* string)
        : runtime_error(string) {}
    explicit factory_not_found(const std::string& basic_string)
        : runtime_error(basic_string) {}
};

template <class T, class Factory, class... CtorArgs>
class factory_registry
{
public:
    factory_registry() = default;
    void register_factory(std::string_view name, Factory factory)
    {
        _factories.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(name),
            std::forward_as_tuple(factory)
        );
    }
    T construct(std::string const& name, CtorArgs... args) const
    {
        if (name.empty())
            return construct_any(args...);
        auto it = _factories.find(name);
        if (it == _factories.end())
            throw factory_not_found(name);
        return (it->second)(std::forward<CtorArgs>(args)...);
    }

    T construct_any(CtorArgs... args) const
    {
        auto it = _factories.begin();
        if (it == _factories.end())
            throw factory_not_found("No any factory registered");
        return (it->second)(std::forward<CtorArgs>(args)...);
    }

private:
    std::unordered_map<std::string, Factory> _factories;
};
}