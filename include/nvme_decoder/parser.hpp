#pragma once

#include "nvme_decoder/command.hpp"

#include <stdexcept>
#include <string_view>

namespace nvme_decoder {

class InputError : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};

// Parses exactly 16 whitespace-separated hexadecimal dwords. A 0x prefix is optional.
Command parse_command(std::string_view input);

} // namespace nvme_decoder
