#include "item_list_model.h"

namespace hakurei::ui
{
namespace model
{
item_list_model::item_list_model(QWidget* parent)
    : QAbstractListModel(reinterpret_cast<QObject*>(parent))
{
}
}

}  // namespace hakurei::ui