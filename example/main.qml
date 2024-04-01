import QtQml 2.15

import Qml4QWidgets 1.0

WidgetController {

    property var formComponent: Qt.createComponent("helloworld.qml")

    Connections {
        target: createButton
        function onClicked() {
            let w = qml4qwidgets.createWidget("helloworld");
            qml4qwidgets.installControllerOnWidget(w, formComponent);
            w.show();
        }
    }
}
