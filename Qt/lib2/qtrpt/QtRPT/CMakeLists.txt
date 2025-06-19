cmake_minimum_required(VERSION 3.16)

project(QtRPT LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/../CommonFiles")
include_directories("${PROJECT_SOURCE_DIR}/../QtRptDemo")
include_directories("${PROJECT_SOURCE_DIR}/../3rdparty/zint-master/backend")
include_directories("${PROJECT_SOURCE_DIR}/../3rdparty/zint-master/backend_qt")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Xml PrintSupport Sql Qml Charts Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Xml PrintSupport Sql Qml Charts Widgets)

message(STATUS "QT_MAJOR_VERSION=${QT_MAJOR_VERSION}")

#===============QtRPT========================
add_library(QtRPT_objs OBJECT
    #$<TARGET_OBJECTS:commonfiles_objs>
    $<TARGET_OBJECTS:XYZ_COMMON>
    qtrptnamespace.h
    qtrpt.cpp qtrpt.h
    RptBandObject.cpp RptBandObject.h
    RptCrossTabObject.cpp RptCrossTabObject.h
    RptDsAbstract.cpp RptDsAbstract.h
    RptFieldObject.cpp RptFieldObject.h
    RptPageObject.cpp RptPageObject.h
    RptScriptEngine.cpp RptScriptEngine.h
    RptSql.cpp RptSql.h
    RptDsInline.cpp RptDsInline.h
    RptDsPlugin.cpp RptDsPlugin.h
)

add_library(QtRPT SHARED
    #$<TARGET_OBJECTS:commonfiles_objs>
    $<TARGET_OBJECTS:XYZ_COMMON>
    $<TARGET_OBJECTS:QtRPT_objs>
)

target_link_libraries(QtRPT PRIVATE
    XYZ_COMMON
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Widgets
    QZint
)

target_link_libraries(QtRPT_objs PRIVATE
    XYZ_COMMON
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Xml
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Widgets
    QZint
)

target_compile_definitions(QtRPT PRIVATE QtRPT_LIBRARY)
