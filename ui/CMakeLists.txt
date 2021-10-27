cmake_minimum_required (VERSION 3.16)
cmake_policy(SET CMP0074 NEW)
set(CMAKE_CXX_STANDARD 17)

project("hakurei_ui")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(DtkCore REQUIRED)
find_package(DtkWidget REQUIRED)
find_package(DtkGui REQUIRED)

add_executable(hakurei_ui 
    "src/ui_main.cpp"

    "src/window/login_window.cpp"
    "src/window/register_window.cpp"
    "src/window/main_window.cpp"

    "src/page/customer_page.cpp"
    "src/page/seller_page.cpp"
    "src/page/admin_page.cpp"
    "src/page/item_pages.cpp"

    "src/widget/main_toolbar.cpp"
    "src/widget/info_bar_widget.cpp"

    "src/model/item_list_model.cpp"
    "src/model/item_list_delegate.cpp"
    "src/model/order_table_model.cpp"

    "src/model/cell_delegates.cpp"

    "res/resources.qrc"
)
target_include_directories(hakurei_ui PRIVATE "../vendor" "src")

target_include_directories(hakurei_ui PRIVATE ${DTKWIDGET_INCLUDES})
target_link_libraries(hakurei_ui Qt5::Core Qt5::Widgets ${DtkWidget_LIBRARIES})
target_link_libraries(hakurei_ui hakurei_core)