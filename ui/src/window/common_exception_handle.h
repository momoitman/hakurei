#pragma once

#include "internal_exceptions.h"
#include "service/service_exceptions.h"

#include <DMessageBox>
#include <exception>

namespace hakurei::ui
{
inline void critial_message_box(const char* msg_lit, const char* title_lit)
{
    DMessageBox::critical(
        nullptr,
        QCoreApplication::tr(title_lit),
        QString(QCoreApplication::tr(msg_lit)),
        DMessageBox::Ok);
}

inline void critial_message_box(QString const& msg, const char* title_lit)
{
    DMessageBox::critical(
        nullptr,
        QCoreApplication::tr(title_lit),
        QString(msg),
        DMessageBox::Ok);
}

inline QString gen_message_from_exception(const char* base_msg_lit, core::hakurei_error const& ex)
{
    return QCoreApplication::tr(base_msg_lit) + QString::fromStdString(ex.reason());
}

// Those exception shouldn't happen in normal run.
template <class Ret = void, class Func>
Ret common_exception_handling(Func block)
{
    try
    {
        return block();
    }
    catch (core::persistence_error& ex)
    {
        critial_message_box(gen_message_from_exception("系统存储错误：", ex), "存储错误");
        std::terminate();
    }
    catch (core::service::invalid_token_error& ex)
    {
        critial_message_box(gen_message_from_exception("无效的访问令牌：", ex), "权限错误");
        std::terminate();
    }
    catch (core::service::id_gen_failed_error& ex)
    {
        critial_message_box(gen_message_from_exception("无法再生成新的 ID: ", ex), "新建失败");
    }
    catch (core::factory_not_found& ex)
    {
        critial_message_box(gen_message_from_exception("配置文件中提供的 Provider 无效: ", ex), "配置文件错误");
        std::terminate();
    }
    catch (core::password_hash_error& ex)
    {
        critial_message_box(gen_message_from_exception("密码摘要生成失败：", ex), "密码生成失败");
    }
    catch (core::service::illegal_string_error& ex)
    {
        critial_message_box(gen_message_from_exception("字符串包含非法字符：", ex), "非法字符串");
    }
    catch (core::hakurei_error& ex)
    {
        critial_message_box(gen_message_from_exception("程序发生未知 Hakurei Exception：", ex), "未知错误");
    }
    catch (...)
    {
        critial_message_box("程序发生未知 C++ Exception!", "存储错误");
        std::terminate();
    }
    return static_cast<Ret>(nullptr);
}
}