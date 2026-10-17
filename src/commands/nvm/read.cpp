#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_read(DecodedCommand& r, const Command& c, std::uint8_t op) {
    (void)op;
    r.command_name = "Read";
    r.use = "Reads the specified logical blocks and associated metadata from the namespace.";
    nvm_slba(r, c);
    nvm_nlb(r, c);
    nvm_rw_fields(r, c, false);
}
} // namespace nvme_decoder::detail
