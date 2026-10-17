#include "nvme_decoder/decoder.hpp"
#include "command_decoders.hpp"
namespace nvme_decoder {
DecodedCommand decode(const Command& command, CommandSet command_set) {
    return command_set == CommandSet::admin ? detail::decode_admin(command)
                                            : detail::decode_nvm(command);
}
} // namespace nvme_decoder
