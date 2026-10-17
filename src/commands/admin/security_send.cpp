#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_security_send(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Security Send";
    r.use = "Sends security-protocol data to the controller.";
    field(r, "Security Protocol", "CDW10[7:0]", c[10] & 0xffu, hex_value(c[10] & 0xffu, 2),
          "Identifies the security protocol being used.");
    field(r, "Reserved", "CDW10[15:8]", (c[10] >> 8) & 0xffu, std::to_string((c[10] >> 8) & 0xffu),
          "Reserved; host shall set to zero.");
    field(r, "Security Protocol Specific", "CDW10[31:16]", c[10] >> 16, hex_value(c[10] >> 16, 4),
          "Protocol-specific value interpreted by the selected security protocol.");
    field(r, "Transfer Length", "CDW11[31:0]", c[11], std::to_string(c[11]) + " bytes",
          "Number of bytes transferred in the security data buffer.");
    reserved_dwords(r, c, 12, 15);
}
} // namespace nvme_decoder::detail
