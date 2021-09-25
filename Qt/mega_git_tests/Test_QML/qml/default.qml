import QtQuick 2.0

Rectangle
{
     id: canvas
     width: 265
     height: 314
     color: "blue"

     Image {
         id: logo
         source: "../pic/tux.png"
     }

     Text {
         id: message
         color: "black"
         text: "Hello World!"
         anchors.centerIn: canvas
     }    
}
