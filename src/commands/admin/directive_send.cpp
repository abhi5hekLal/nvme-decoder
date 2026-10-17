#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_directive_send(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Directive Send";
    r.use = "Sends directive-specific data to the controller.";
    field(r, "Number of Dwords", "CDW10[31:0]", c[10],
          std::to_string(static_cast<std::uint64_t>(c[10]) + 1u),
          "Zero-based data transfer length: NUMD + 1 dwords.");
    field(r, "Directive Operation", "CDW11[7:0]", c[11] & 0xffu, hex_value(c[11] & 0xffu, 2),
          "Selects the directive-specific operation; interpretation depends on Directive Type.");
    field(r, "Directive Type", "CDW11[15:8]", (c[11] >> 8) & 0xffu,
          hex_value((c[11] >> 8) & 0xffu, 2),
          "Selects the directive family and the interpretation of directive-specific fields.");
    field(r, "Directive Specific", "CDW11[31:16]", c[11] >> 16, hex_value(c[11] >> 16, 4),
          "Directive-type-specific parameter.", 4);
    field(r, "Directive-specific command dwords", "CDW12:CDW13[63:0]",
          (static_cast<std::uint64_t>(c[13]) << 32) | c[12],
          hex_value((static_cast<std::uint64_t>(c[13]) << 32) | c[12], 16),
          "Interpretation depends on Directive Type and Directive Operation.", 16);
    reserved_dwords(r, c, 14, 15);
}
} // namespace nvme_decoder::detail
