import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.1
import QtQuick.Controls.Styles 1.4

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    signal qmlSignal(string msg)

    MessageDialog {
        id: messageDialog
        title: "MessageDialog"
        text: "Info"
    }

    Button {
        id: item
        x: 100
        y: 100
        width: 200
        text: "YES"

        MouseArea {
            anchors.fill: parent
            onClicked: qmlSignal("Hello from QML")
        }

        /*
        onClicked: {
            messageDialog.setText("YES")
            messageDialog.visible = true
        }
        style: ButtonStyle {
            background: Rectangle {
                color: "blue"
            }
            Text {
                //id: name
                //text: qsTr("text")
                color: "yellow"
            }
        }
        */
    }

    Grid {
        columns: 2

        Button {
            id: button_0
            text: "0"
            onClicked: {
                messageDialog.setText("0")
                messageDialog.visible = true
            }
        }
        Button {
            id: button_1
            text: "1"
            onClicked: {
                messageDialog.setText("1")
                messageDialog.visible = true
            }
        }
        Button {
            id: button_2
            text: "2"
            onClicked: {
                messageDialog.setText("2")
                messageDialog.visible = true
            }
        }
        Button {
            id: button_3
            text: "3"
            onClicked: {
                messageDialog.setText("3")
                messageDialog.visible = true
            }
        }
    }
}
