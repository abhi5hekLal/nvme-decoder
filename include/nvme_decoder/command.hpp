#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace nvme_decoder {

using Command = std::array<std::uint32_t, 16>;
enum class CommandSet { admin, nvm };

struct DecodedCommand {
    std::string command_name;
    std::string use;
    struct Field {
        std::string name;
        std::string location;
        std::string raw_value;
        std::string decoded_value;
        std::string meaning;
    };
    std::vector<Field> fields;
};

} // namespace nvme_decoder
