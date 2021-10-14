#pragma once

#include "internal_exceptions.h"

#include <stdexcept>
#include <unordered_map>

namespace hakurei::core
{

template <class T, class Factory, class... CtorArgs>
class factory_registry
{
public:
    factory_registry(std::string name)
        : _name(std::move(name)){}
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
            throw factory_not_found(name, _name);
        return (it->second)(std::forward<CtorArgs>(args)...);
    }

    T construct_any(CtorArgs... args) const
    {
        auto it = _factories.begin();
        if (it == _factories.end())
            throw factory_not_found("default", _name);
        return (it->second)(std::forward<CtorArgs>(args)...);
    }

private:
    std::string _name;
    std::unordered_map<std::string, Factory> _factories;
};
}