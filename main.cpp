#include "qtquick2controlsapplicationviewer.h"
#include "ev3.h"
#include "ev3motor.h"
#include <qqml.h>

void registerQmlTypes() {
    qmlRegisterType<Ev3>("Ev3", 1, 0, "Ev3");
    qmlRegisterUncreatableType<Ev3Motor>("Ev3", 1, 0, "Ev3Motor", QStringLiteral("Access the motors from the Ev3 object."));
};

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    registerQmlTypes();

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/QtSentryController/main.qml"));
    viewer.showFullscreen();

    return app.exec();
}
