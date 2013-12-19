#include "qtquick2controlsapplicationviewer.h"
#include "ev3.h"
#include <qqml.h>

void registerQmlTypes() {
    qmlRegisterType<Ev3>("Ev3", 1, 0, "Ev3");
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
