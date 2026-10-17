#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_flush(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Flush";
    r.use =
        "Requests that volatile write data for the namespace be committed to non-volatile media.";
    nvm_reserved_dwords(r, c, 10, 15, "Reserved for Flush; host shall set to zero.");
}
} // namespace nvme_decoder::detail
