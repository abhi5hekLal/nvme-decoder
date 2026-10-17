#include "../../command_decoders.hpp"

namespace nvme_decoder::detail {
DecodedCommand decode_get_feature(const Command& command) {
    return decode_feature_command(command, true);
}
} // namespace nvme_decoder::detail
