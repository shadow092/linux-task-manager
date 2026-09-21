#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Load Main.qml from module URI "MyApp"
    engine.loadFromModule("MyApp", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}