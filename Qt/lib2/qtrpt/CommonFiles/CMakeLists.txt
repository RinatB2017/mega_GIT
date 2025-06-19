set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/zint-master/backend")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/zint-master/backend_qt")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Xml PrintSupport Sql Qml Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Xml PrintSupport Sql Qml Widgets)

add_library(commonfiles_objs OBJECT
    CommonClasses.cpp CommonClasses.h
    Barcode.cpp Barcode.h
    numbertoarabicword.cpp numbertoarabicword.h
)
set_property(TARGET commonfiles_objs
             PROPERTY POSITION_INDEPENDENT_CODE True)

add_library(commonfiles
    $<TARGET_OBJECTS:commonfiles_objs>
)

target_link_libraries(commonfiles_objs PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Widgets
)

