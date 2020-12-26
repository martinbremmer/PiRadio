import QtQuick 2.11

Item {
    id: header

    property real borderSpan
    property real pixelSize
    property string text

    Rectangle {
        id: headerRect
        width: header.width
        height: header.height - headerBorder.height
        color: "#000000"
        opacity: 0.9
        anchors.top: parent.top

        Text {
            id: headerText
            width: parent.width
            height: parent.height
            color: "#FFFFFF"
            text: header.text
            fontSizeMode: Text.Fit
            minimumPixelSize: 10
            font.pixelSize: header.pixelSize
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    FuzzyBorder {
        id: headerBorder
        width: header.width
        height: header.borderSpan
        toColor: "#000000"
        opacity: 0.9
        rotation: 180
        anchors.top: headerRect.bottom
    }
}
