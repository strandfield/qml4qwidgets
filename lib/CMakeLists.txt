
find_package(Qt5 COMPONENTS Core Gui Widgets Qml REQUIRED)

file(GLOB_RECURSE LIB_HDR_FILES "qml4qwidgets/*.h")
file(GLOB_RECURSE LIB_SRC_FILES "qml4qwidgets/*.cpp")

add_library(qml4qwidgets SHARED ${LIB_HDR_FILES} ${LIB_SRC_FILES} "qml4qwidgets.qrc")
target_compile_definitions(qml4qwidgets PRIVATE QML4QWIDGETS_BUILD_LIBRARY)
target_include_directories(qml4qwidgets PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(qml4qwidgets Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Qml)

foreach(_source IN ITEMS ${HDR_FILES} ${SRC_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
