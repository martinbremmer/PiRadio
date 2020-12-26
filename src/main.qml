import QtQuick 2.11
import QtQuick.Window 2.11

import PiRadio.CoverArt      1.0
import PiRadio.AudioPlayer   1.0
import PiRadio.RadioStation  1.0

import "qml/"

Window {
    id: root
    visible: true
    width: 480
    height: 320
    title: qsTr("PiRadio")

    /* Fullscreen on the 3.5 inch touchscreen. */
    visibility: Screen.width === 480 ? Window.FullScreen : Window.Windowed

    color: "#7F7F7F"

    property RadioStation currentRadio: radioStationSelector.current

    AudioPlayer {
        id: audioPlayer
        stream: currentRadio.stream
    }

    CoverArt {
        id: coverArt
        artist: audioPlayer.artist
        title: audioPlayer.title
    }

    FuzzyImage {
        id: cover
        width: height
        height: parent.height
        visible: !menu.listVisible
        image: coverArt.image === "" ? currentRadio.icon : coverArt.image
        outsideColor: root.color
        borderSpan: 30
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    RadioStationSelector {
        id: radioStationSelector
        x: 10
        y: header.height - (header.borderSpan / 2)
        width: 350
        height: parent.height - (y*2)
        visible: menu.listVisible
    }

    Header {
        id: header
        width: root.width
        height: 90
        borderSpan: 50
        pixelSize: 25
        anchors.top: parent.top
        text: audioPlayer.artist + " - " + audioPlayer.title
    }

    Footer {
        id: footer
        width: root.width
        height: 90
        borderSpan: 50
        pixelSize: 25
        anchors.bottom: parent.bottom
        text: currentRadio.name
        icon: currentRadio.icon
    }

    Menu {
        id: menu
        width: 60
        anchors {
            right: parent.right
            rightMargin: 10
            verticalCenter: parent.verticalCenter
        }

        visible: false
        toggle: footer.pressed

        onPressedExitChanged: if (pressedExit) Qt.quit()
        onPressedListChanged: if (pressedList) listVisible = !listVisible
        onVisibleChanged:     if (!visible)    listVisible = false

        property bool listVisible: false
    }
}
