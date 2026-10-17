#pragma once

#include "nvme_decoder/command.hpp"

namespace nvme_decoder {

DecodedCommand decode(const Command& command, CommandSet command_set);

} // namespace nvme_decoder
