#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_keep_alive(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Keep Alive";
    r.use = "Restarts the controller keep-alive timer when keep-alive is enabled.";
    // Keep Alive
    reserved_dwords(r, c, 10, 15);
    r.use = "Restarts the controller keep-alive timer when that capability is enabled. All "
            "command-specific dwords are reserved.";
}
} // namespace nvme_decoder::detail
