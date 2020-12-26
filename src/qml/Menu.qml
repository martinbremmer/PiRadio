import QtQuick 2.11

Column {
    id: menu
    width: 50
    spacing: 10
    visible: false
    property bool toggle
    onToggleChanged: if (toggle) { visible = !visible }

    property bool pressedList: false
    property bool pressedExit: false

    Rectangle {
        id: menuExit
        color: "#000000"
        width: menu.width
        height: width
        Text {
            font.pixelSize: menuExit.height - 10
            color: "#7F7F7F"
            text: "X"
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }
        }

        MouseArea {
            anchors.fill: parent
            onPressed:  menu.pressedExit = true
            onReleased: menu.pressedExit = false
        }
    }
    Rectangle {
        color: "#000000"
        width: menu.width
        height: width
        Bars {
            width: parent.width - 10
            height: parent.height - 10
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }
        }
        MouseArea {
            anchors.fill: parent
            onPressed:  menu.pressedList = true
            onReleased: menu.pressedList = false
        }
    }
}
