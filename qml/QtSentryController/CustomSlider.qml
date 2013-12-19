import QtQuick 2.1
import QtQuick.Controls 1.0

Slider {
    id: slider
    minimumValue: -100
    maximumValue: 100
    value: 0

    MouseArea {
        anchors.centerIn: parent
        width: (slider.orientation === Qt.Horizontal ? (slider.width / 5) : slider.width)
        height: (slider.orientation === Qt.Horizontal ? slider.height : (slider.height / 5))
        onClicked: slider.value = 0
        enabled: slider.value !== 0
    }
}
