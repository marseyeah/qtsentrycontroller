import QtQuick 2.1
import Ev3 1.0
import QtQuick.Controls 1.0

Item {
    id: controlsRoot
    property Ev3 sentry: null

    Item {
        id: drivingControl
        anchors.fill: parent
        visible: controlsRoot.state == "connected"

        function updateMotors(throttle, steering) {
            sentry.motorA.power = throttle - steering;
            sentry.motorD.power = throttle + steering;
        }

//        CustomSlider {
//            id: throttle
//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//            anchors.right: parent.right
//            anchors.margins: 30
//            orientation: Qt.Vertical
//            onValueChanged: drivingControl.updateMotors()
//        }

//        CustomSlider {
//            id: steering
//            anchors.left: parent.left
//            width: throttle.height
//            anchors.bottom: parent.bottom
//            anchors.margins: 30
//            orientation: Qt.Horizontal
//            onValueChanged: drivingControl.updateMotors()
//        }

        Rectangle {
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: 30
            width: 450
            height: 450
            color: "#50000000"
            border.width: 3
            border.color: "white"

            MouseArea {
                id: joystick
                anchors.fill: parent
                onPositionChanged: {
                    var t = -100 + ((joystick.height - mouse.y) / joystick.height) * 200;
                    var s = -100 + (mouse.x / joystick.width) * 200;

                    drivingControl.updateMotors(t, s);
                }
                onExited: drivingControl.updateMotors(0, 0);
                onReleased: drivingControl.updateMotors(0, 0);
            }

            Rectangle {
                width: 180
                height: 180
                radius: width / 2
                color: "#CCFFFFFF"
                visible: joystick.pressed
                x: Math.min(joystick.width, Math.max(joystick.mouseX, 0)) - width / 2
                y: Math.min(joystick.height, Math.max(joystick.mouseY, 0)) - height / 2
            }
        }

        Slider {
            id: arms
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 30
            height: 500
            orientation: Qt.Vertical
            minimumValue: -80
            maximumValue: 80
            value: 0
            onValueChanged: sentry.motorB.power = -arms.value
            onPressedChanged: {
                if (!arms.pressed)
                    arms.value = 0
            }
            onHoveredChanged: {
                if (!arms.hovered)
                    arms.value = 0
            }
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
