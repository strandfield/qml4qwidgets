
file(GLOB UI_FILES "*.ui")
file(GLOB QML_FILES "*.qml")

add_custom_target(examplefiles SOURCES ${UI_FILES} ${QML_FILES})
