#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_verify(DecodedCommand& r, const Command& c, std::uint8_t op) {
    (void)op;
    r.command_name = "Verify";
    r.use = "Verifies the specified logical block range without transferring the data to the host.";
    nvm_slba(r, c);
    nvm_nlb(r, c);
    nvm_verify_compare_fields(r, c, false);
}
} // namespace nvme_decoder::detail
