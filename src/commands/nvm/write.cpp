#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_write(DecodedCommand& r, const Command& c, std::uint8_t op) {
    (void)op;
    r.command_name = "Write";
    r.use = "Writes the specified logical blocks and associated metadata to the namespace.";
    nvm_slba(r, c);
    nvm_nlb(r, c);
    nvm_rw_fields(r, c, true);
}
} // namespace nvme_decoder::detail
