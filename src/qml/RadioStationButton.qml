import QtQuick 2.11
import PiRadio.RadioStation 1.0

Rectangle {
    id: radioInfo

    color: highlight ? "light grey" : "transparent"
    border.color: "black"
    border.width: 2
    radius: 4

    signal selected

    property RadioStation radioStation
    property bool highlight: false

    Image {
        id: radioIcon
        width: height
        height: parent.height - 4
        source: radioStation.icon
        fillMode: Image.PreserveAspectFit
        anchors {
            left: radioInfo.left
            leftMargin: 2
            verticalCenter: radioInfo.verticalCenter
        }
    }
    Text {
        id: radioName
        width: parent.width - radioIcon.width - 4
        fontSizeMode: Text.Fit
        minimumPixelSize: 10
        font.pixelSize: 25
        font.bold: true
        text: radioStation.name
        anchors {
            left: radioIcon.right
            leftMargin: 10
            verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            radioInfo.selected()
        }
    }
}
