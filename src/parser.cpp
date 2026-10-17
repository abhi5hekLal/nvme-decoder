#include "nvme_decoder/parser.hpp"

#include <charconv>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace nvme_decoder {
namespace {

std::uint32_t parse_dword(std::string_view token, std::size_t index) {
    if (token.starts_with("0x") || token.starts_with("0X")) {
        token.remove_prefix(2);
    }
    if (token.empty() || token.size() > 8) {
        throw InputError("Dword " + std::to_string(index + 1) + " must contain 1 to 8 hex digits.");
    }

    std::uint32_t value = 0;
    const auto result = std::from_chars(token.data(), token.data() + token.size(), value, 16);
    if (result.ec != std::errc{} || result.ptr != token.data() + token.size()) {
        throw InputError("Dword " + std::to_string(index + 1) +
                         " is not valid hexadecimal: " + std::string(token));
    }
    return value;
}

} // namespace

Command parse_command(std::string_view input) {
    Command command{};
    std::size_t count = 0;
    std::size_t position = 0;

    while (position < input.size()) {
        while (position < input.size() &&
               (input[position] == ' ' || input[position] == '\t' || input[position] == '\n' ||
                input[position] == '\r' || input[position] == ',')) {
            ++position;
        }
        if (position == input.size())
            break;

        const auto start = position;
        while (position < input.size() && input[position] != ' ' && input[position] != '\t' &&
               input[position] != '\n' && input[position] != '\r' && input[position] != ',') {
            ++position;
        }
        if (count == command.size()) {
            throw InputError("Expected exactly 16 dwords, but found more than 16.");
        }
        command[count] = parse_dword(input.substr(start, position - start), count);
        ++count;
    }

    if (count != command.size()) {
        throw InputError("Expected exactly 16 dwords, but found " + std::to_string(count) + ".");
    }
    return command;
}

} // namespace nvme_decoder
