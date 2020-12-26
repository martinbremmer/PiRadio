import QtQuick 2.11


Item {
    id: imageMenu

    Rectangle {
        id: imageMenuTop
        width: imageMenu.width
        height: 4
        color: "#7F7F7F"
        anchors.bottom: imageMenuMiddle.top
        anchors.bottomMargin: 6
    }

    Rectangle {
        id: imageMenuMiddle
        y: (imageMenu.height / 2) - (height / 2)
        width: imageMenu.width
        height: 4
        color: "#7F7F7F"
    }

    Rectangle {
        id: imageMenuBottom
        width: imageMenu.width
        height: 4
        color: "#7F7F7F"
        anchors.top: imageMenuMiddle.bottom
        anchors.topMargin: 6
    }
}
