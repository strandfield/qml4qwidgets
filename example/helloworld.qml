import QtQml 2.15

import Qml4QWidgets 1.0

WidgetController {
    Connections {
        target: okButton
        function onClicked() {
            let text = "Hello";
            if (theLineEdit.text.length > 0) {
                text += " " + theLineEdit.text + "!";
            } else {
                text += " World!"
            }

            MessageBox.information(controlledWidget, "QML MessageBox", text);
        }
    }
}
