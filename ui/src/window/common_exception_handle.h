#pragma once

#include "internal_exceptions.h"
#include "service/service_exceptions.h"

#include <QTranslator>
#include <QMessageBox>
#include <exception>

namespace hakurei::ui
{
// Those exception shouldn't happen in normal run.
template <class Ret, class Func>
Ret common_exception_handling(Func block)
{
    try
    {
        return block();
    }
    catch (core::persistence_error& ex)
    {
        QMessageBox::critical(
            nullptr,
            QCoreApplication::tr("存储错误"),
            QString(QCoreApplication::tr("系统存储错误：")) + QString::fromStdString(ex.reason()),
            QMessageBox::Ok);
        std::terminate();
    }
    catch (core::service::invalid_token_error& ex)
    {
        QMessageBox::critical(
            nullptr,
            QCoreApplication::tr("权限错误"),
            QString(QCoreApplication::tr("无效的访问令牌：")) + QString::fromStdString(ex.reason()),
            QMessageBox::Ok);
        std::terminate();
    }
    catch (core::service::id_gen_failed_error& ex)
    {
        QMessageBox::critical(
            nullptr,
            QCoreApplication::tr("新建失败"), 
            QString(QCoreApplication::tr("无法再生成新的 ID: ")) + QString::fromStdString(ex.reason()),
            QMessageBox::Ok);
    }
    catch (core::factory_not_found& ex)
    {
        QMessageBox::critical(
            nullptr,
            QCoreApplication::tr("配置文件错误"),
            QString(QCoreApplication::tr("配置文件中提供的 Provider 无效: ")) + QString::fromStdString(ex.reason()),
            QMessageBox::Ok);
        std::terminate();
    }
    catch (core::password_hash_error& ex)
    {
        QMessageBox::critical(
            nullptr,
            QCoreApplication::tr("密码生成失败"),
            QString(QCoreApplication::tr("密码摘要生成失败：")) + QString::fromStdString(ex.reason()),
            QMessageBox::Ok);
    }
    catch (core::service::illegal_string_error& ex)
    {
        QMessageBox::critical(
            nullptr,
            QCoreApplication::tr("非法字符串"),
            QString(QCoreApplication::tr("字符串包含非法字符：")) + QString::fromStdString(ex.reason()),
            QMessageBox::Ok);
    }
    catch (core::hakurei_error& ex)
    {
        QMessageBox::critical(
            nullptr,
            QCoreApplication::tr("未知错误"),
            QString(QCoreApplication::tr("程序发生未知 Hakurei Exception：")) + QString::fromStdString(ex.reason()),
            QMessageBox::Ok);
    }
    catch (...)
    {
        QMessageBox::critical(
            nullptr,
            QCoreApplication::tr("未知错误"),
            QString(QCoreApplication::tr("程序发生未知 C++ Exception!")),
            QMessageBox::Ok);
        std::terminate();
    }
    return static_cast<Ret>(nullptr);
}
}