#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_compare(DecodedCommand& r, const Command& c, std::uint8_t op) {
    (void)op;
    r.command_name = "Compare";
    r.use =
        "Compares host-provided data with the data stored in the specified logical block range.";
    nvm_slba(r, c);
    nvm_nlb(r, c);
    nvm_verify_compare_fields(r, c, true);
}
} // namespace nvme_decoder::detail
