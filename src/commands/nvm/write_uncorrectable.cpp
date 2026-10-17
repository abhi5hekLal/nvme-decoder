#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_write_uncorrectable(DecodedCommand& r, const Command& c, std::uint8_t op) {
    (void)op;
    r.command_name = "Write Uncorrectable";
    r.use = "Marks the specified logical block range as containing unrecoverable data.";
    nvm_slba(r, c);
    nvm_nlb(r, c);
    field(r, "Reserved", "CDW12[31:16]", c[12] >> 16, std::to_string(c[12] >> 16),
          "Reserved; host shall set to zero.");
    nvm_reserved_dwords(r, c, 13, 15, "Reserved for Write Uncorrectable; host shall set to zero.");
}
} // namespace nvme_decoder::detail
