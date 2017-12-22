import QtQuick 2.4
import QtQuick.Controls 1.3

import com.wapice.models 1.0

ApplicationWindow {
    id: window
    visible: true
    minimumWidth: 700
    minimumHeight: 400
    width: 1024
    height: 768
    title: qsTr("Bus data visualizer")

    Component.onCompleted: busModel.refresh()

    Text { id: header; text: "Registered busses"; font.bold: true; y: 20 }
    Rectangle {
        color: "white"
        width: Math.max(400, parent.width/2)
        anchors { top: header.bottom; bottom: parent.bottom; left: parent.left; margins: 5 }
        ListView {
            clip: true
            anchors.fill: parent
            model: BusListModel { id: busModel }
            spacing: 3
            delegate: BusDelegate {
                height: 230
                width: parent.width
                busId: bus.name
                speed: bus.speed
                fuelConsumption: bus.fuelConsumption
            }
        }
    }
}
