
find_package(Qt5 COMPONENTS Core Gui Widgets Qml Xml UiTools REQUIRED)

file(GLOB LIB_HDR_FILES "*.h")
file(GLOB LIB_SRC_FILES "*.cpp")

add_library(qml4qwidgets SHARED ${LIB_HDR_FILES} ${LIB_SRC_FILES} "qml4qwidgets.qrc")
target_compile_definitions(qml4qwidgets PRIVATE QML4QWIDGETS_BUILD_LIBRARY)
target_include_directories(qml4qwidgets PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(qml4qwidgets Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Qml Qt5::Xml Qt5::UiTools)
