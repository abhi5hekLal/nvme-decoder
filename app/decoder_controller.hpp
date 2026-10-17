#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>

class DecoderController final : public QObject {
    Q_OBJECT

  public:
    using QObject::QObject;

    Q_INVOKABLE QVariantMap decodeCommand(const QString& input, const QString& command_set) const;
};
