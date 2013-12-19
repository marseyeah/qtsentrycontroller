import QtQuick 2.1
import Ev3 1.0

Item {
    id: controlsRoot
    property Ev3 sentry: null

    Item {
        anchors.fill: parent
        visible: controlsRoot.state == "connected"

        CustomSlider {
            id: throttle
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: 30
            orientation: Qt.Vertical
        }

        CustomSlider {
            id: steering
            anchors.left: parent.left
            width: throttle.height
            anchors.bottom: parent.bottom
            anchors.margins: 30
            orientation: Qt.Horizontal
        }
    }

    Rectangle {
        id: stateLight
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 30
        width: 40
        height: 40
        radius: width / 2
        border.width: 2
        border.color: "#333333"
    }

    Text {
        id: stateText
        anchors.left: stateLight.right
        anchors.leftMargin: 20
        anchors.verticalCenter: stateLight.verticalCenter
        font.pointSize: 14
        color: "white"
        style: Text.Outline
        styleColor: "black"
    }

    states: [
        State {
            name: "connected"
            when: sentry.state === Ev3.Connected
            PropertyChanges {
                target: stateLight
                color: "#96e074"
            }
            PropertyChanges {
                target: stateText
                text: "Connected"
            }
        },
        State {
            name: "disconnected"
            when: sentry.state === Ev3.Disconnected || sentry.state === Ev3.Unknown
            PropertyChanges {
                target: stateLight
                color: "#dd2020"
            }
            PropertyChanges {
                target: stateText
                text: "Disconnected"
            }
        },
        State {
            name: "transient"
            when: sentry.state === Ev3.Connecting || sentry.state === Ev3.Disconnecting || sentry.state === Ev3.Searching
            PropertyChanges {
                target: stateLight
                color: "#eab748"
            }
            PropertyChanges {
                target: stateText
                text: sentry.state === Ev3.Connecting ? "Connecting" : (sentry.state === Ev3.Searching ? "Searching" : "Disconnecting")
            }
        }
    ]
}
