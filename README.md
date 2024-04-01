
A proof-of-concept that demonstrates how QML can be used to control 
the behavior of widgets in a QtWidgets application.

Source code for the library is in `lib`.

The main entry point for the API is the C++ class `Qml4QWidgetsController`
defined in `qml4qwidgets.h`.

The `runner` directory contains the source code for `quark`, 
a small program that uses the API for controlling (with QML) forms 
created with QtDesigner.
The example files in `example` can be used with quark.

```
quark example/helloworld.ui
```
