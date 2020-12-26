import QtQuick 2.11

Rectangle {
    id: fuzzyBorder

    property color toColor

    gradient: Gradient {
        GradientStop { position: 0.0; color: "transparent" }
        GradientStop { position: 1.0; color: fuzzyBorder.toColor }
    }
}
