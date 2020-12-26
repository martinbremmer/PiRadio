import QtQuick 2.11

Item {
    id: fuzzyRoot

    property color outsideColor
    property real borderSpan
    property alias image: fuzzyImage.source

    Image {
        id: fuzzyImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
    }

//    Rectangle {
//        color: "red"
//        x: -1
//        y: -1
//        width: fuzzyRoot.width + 2
//        height: fuzzyRoot.height + 2
//    }

    FuzzyBorder {
        id: fuzzyTop
        rotation: 180
        x: 0
        y: 0
        width: fuzzyRoot.width
        height: fuzzyRoot.borderSpan
        toColor: fuzzyRoot.outsideColor
    }
    FuzzyBorder {
        id: fuzzyRight
        rotation: 270
        x: (fuzzyRoot.width / 2) - (fuzzyRoot.borderSpan / 2)
        y: (fuzzyRoot.height / 2) - (fuzzyRoot.borderSpan / 2)
        width: fuzzyRoot.height
        height: fuzzyRoot.borderSpan
        toColor: fuzzyRoot.outsideColor
    }
    FuzzyBorder {
        id: fuzzyBottom
        rotation: 0
        x: 0
        y: fuzzyRoot.height - fuzzyRoot.borderSpan
        width: fuzzyRoot.width
        height: fuzzyRoot.borderSpan
        toColor: fuzzyRoot.outsideColor
    }
    FuzzyBorder {
        id: fuzzyLeft
        rotation: 90
        x: (fuzzyRoot.width / -2) + (fuzzyRoot.borderSpan / 2)
        y: (fuzzyRoot.height / 2) - (fuzzyRoot.borderSpan / 2)
        width: fuzzyRoot.height
        height: fuzzyRoot.borderSpan
        toColor: fuzzyRoot.outsideColor
    }
}
