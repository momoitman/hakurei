#pragma once

#include "internal_exceptions.h"
#include "service/service_exceptions.h"

#include <QTranslator>
#include <QMessageBox>

namespace hakurei::ui
{
template <class Func, class Ret>
Ret common_exception_handling(Func block)
{
    try
    {
        return block();
    }
    catch (core::persistence_error& ex)
    {
        
    }
    catch (core::service::invalid_token_error& ex)
    {
        
    }
    catch (core::service::id_gen_failed_error& ex)
    {
        QMessageBox::critical(
            nullptr,
            QCoreApplication::tr("新建失败"), 
            QCoreApplication::QString(tr("无法再生成新的 ID: ")) + QString::fromStdString(ex.reason()),
            QMessageBox::Ok);
    }
    catch (...)
    {
        
    }
}
}