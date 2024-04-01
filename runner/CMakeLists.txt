
find_package(Qt5 COMPONENTS Xml UiTools REQUIRED)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(quark ${HDR_FILES} ${SRC_FILES} "quark.qrc")
target_link_libraries(quark qml4qwidgets)
target_link_libraries(quark Qt5::Xml Qt5::UiTools)

if (WIN32)
  set_target_properties(quark PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=${Qt5_DIR}/../../../bin;%PATH%")
endif()
