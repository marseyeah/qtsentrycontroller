import QtQuick 2.1
import QtQuick.Controls 1.0
import Ev3 1.0

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Component.onCompleted: sentry.searchAndConnect()

    Ev3 {
        id: sentry
        onErrorChanged: print(error)
    }

    VideoFeed {
        id: video
        anchors.fill: parent
    }

    SentryControls {
        id: controls
        anchors.fill: video
        sentry: sentry
    }
}
