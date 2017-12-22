import QtQuick 2.4
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.1

import com.wapice.models 1.0

Rectangle {
    id: delegate
    color: "black"

    property string busId: ""
    property real speed: 0
    property real fuelConsumption: 0

    ColumnLayout {
        anchors { fill: parent; bottomMargin: 10 }
        Text { text: "Bus id: " + busId; font.bold: true; color: "white" }
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20
            CircularGauge {
                Layout.fillWidth: true
                Layout.fillHeight: true
                value: speed
                maximumValue: 200
                minimumValue: 0

                Text {
                    anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom }
                    text: "km/h"
                    color: "white"
                }

                Behavior on value {
                    NumberAnimation {
                        duration: 1000
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 10

                Gauge {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    minimumValue: 0
                    maximumValue: 40
                    value: fuelConsumption

                    Behavior on value {
                        NumberAnimation {
                            duration: 1000
                        }
                    }
                }
                Text {
                    text: "Fuel l/h";
                    color: "white";
                    anchors { horizontalCenter: parent.horizontalCenter }
                }
            }
        }
    }
}
