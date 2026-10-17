#include "decoder_controller.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QUrl>

int main(int argc, char* argv[]) {
    QGuiApplication application(argc, argv);
    QGuiApplication::setApplicationName(QStringLiteral("NVMe Decoder"));
    QQuickStyle::setStyle(QStringLiteral("Basic"));

    DecoderController decoder;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("decoder"), &decoder);
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/NVMeDecoder/Main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return 1;
    }

    return application.exec();
}
