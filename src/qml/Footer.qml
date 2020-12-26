import QtQuick 2.11

Item {
    id: footer

    property real borderSpan
    property real pixelSize
    property string icon
    property string text

    property bool pressed: false

    FuzzyBorder {
        id: footerBorder
        width: footer.width
        height: footer.borderSpan
        toColor: "#000000"
        opacity: 0.9
        anchors.bottom: footerRect.top
    }

    Rectangle {
        id: footerRect
        width: footer.width
        height: footer.height - footerBorder.height
        color: "#000000"
        opacity: 0.9
        anchors.bottom: parent.bottom

        FuzzyImage {
            id: footerIcon
            width: height
            height: footerRect.height - 2
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 10
            }
            image: footer.icon
            outsideColor: footerRect.color
            borderSpan: 5
        }

        Text {
            id: footerText
            width: parent.width - 100
            height: parent.height
            color: "#FFFFFF"
            text: footer.text
            fontSizeMode: Text.Fit
            minimumPixelSize: 10
            font.pixelSize: footer.pixelSize
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Bars {
            id: footerMenu
            width: 30
            height: footerRect.height - 2
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 10
            }
        }

        MouseArea {
            anchors.fill: parent
            onPressed:  footer.pressed = true
            onReleased: footer.pressed = false
        }
    }
}
