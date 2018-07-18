import QtQuick 2.4
import Pixel 1.0
import QtQuick.Controls 1.2

Item {
    width: 1020
    height: 1020

    Flickable {
        anchors.fill: parent
        contentWidth: 1000; contentHeight: 1000
        PixelGrid {
            id: pixelGrid
            anchors { fill: parent; margins: 10 }
        }

    }
    Slider {
        anchors { bottom: parent.bottom; bottomMargin: 20;
            horizontalCenter: parent.horizontalCenter }
        minimumValue: 1
        maximumValue: 100
        value: 100
        onValueChanged: pixelGrid.scale = value / 100
    }
}
