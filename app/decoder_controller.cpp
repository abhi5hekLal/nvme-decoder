#include "decoder_controller.hpp"

#include "nvme_decoder/decoder.hpp"
#include "nvme_decoder/parser.hpp"

#include <QVariantList>

#include <exception>

QVariantMap DecoderController::decodeCommand(const QString& input, const QString& command_set) const {
    QVariantMap result;

    try {
        const auto command = nvme_decoder::parse_command(input.toStdString());
        const auto set = command_set.compare(QStringLiteral("nvm"), Qt::CaseInsensitive) == 0
                             ? nvme_decoder::CommandSet::nvm
                             : nvme_decoder::CommandSet::admin;
        const auto decoded = nvme_decoder::decode(command, set);

        QVariantList fields;
        fields.reserve(static_cast<qsizetype>(decoded.fields.size()));
        for (const auto& source : decoded.fields) {
            QVariantMap field;
            field.insert(QStringLiteral("name"), QString::fromStdString(source.name));
            field.insert(QStringLiteral("location"), QString::fromStdString(source.location));
            field.insert(QStringLiteral("value"), QString::fromStdString(source.decoded_value));
            field.insert(QStringLiteral("meaning"), QString::fromStdString(source.meaning));
            fields.append(field);
        }

        result.insert(QStringLiteral("ok"), true);
        result.insert(QStringLiteral("commandName"), QString::fromStdString(decoded.command_name));
        result.insert(QStringLiteral("use"), QString::fromStdString(decoded.use));
        result.insert(QStringLiteral("fields"), fields);
    } catch (const nvme_decoder::InputError& error) {
        result.insert(QStringLiteral("ok"), false);
        result.insert(QStringLiteral("error"), QString::fromUtf8(error.what()));
    } catch (const std::exception& error) {
        result.insert(QStringLiteral("ok"), false);
        result.insert(QStringLiteral("error"), QString::fromUtf8(error.what()));
    }

    return result;
}
