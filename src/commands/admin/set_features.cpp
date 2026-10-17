#include "../../command_decoders.hpp"

namespace nvme_decoder::detail {
DecodedCommand decode_set_feature(const Command& command) {
    return decode_feature_command(command, false);
}
} // namespace nvme_decoder::detail
