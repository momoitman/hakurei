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
    "src/window/main_window.cpp"
    "src/window/main_toolbar.cpp"

    "res/resources.qrc"
)
target_include_directories(hakurei_ui PRIVATE "../vendor")

target_include_directories(hakurei_ui PRIVATE ${DTKWIDGET_INCLUDES})
target_link_libraries(hakurei_ui Qt5::Core Qt5::Widgets ${DtkWidget_LIBRARIES})
target_link_libraries(hakurei_ui hakurei_core)