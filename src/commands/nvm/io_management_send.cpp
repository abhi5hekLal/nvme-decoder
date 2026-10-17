#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_io_management_send(DecodedCommand& r, const Command& c, std::uint8_t op) {
    (void)op;
    r.command_name = "I/O Management Send";
    const auto mo = c[10] & 0xffu;
    r.use = "Sends I/O-management information selected by the Management Operation field.";
    field(r, "Management Operation (MO)", "CDW10[7:0]", mo, hex_value(mo, 2),
          "Selects the I/O-management send operation. The operation and its payload format are "
          "defined by the applicable Base and I/O Command Set specifications.",
          2);
    field(r, "Reserved", "CDW10[15:8]", (c[10] >> 8) & 0xffu, std::to_string((c[10] >> 8) & 0xffu),
          "Reserved; host shall set to zero.");
    field(r, "Management Operation Specific (MOS)", "CDW10[31:16]", c[10] >> 16,
          hex_value(c[10] >> 16, 4),
          "Meaning is specific to MO. If the selected operation does not define MOS, this field is "
          "reserved.",
          4);
    field_text(r, "Management Request Data", "Data Pointer",
               "Address shown in common pointer fields", "Host-to-controller data buffer",
               "Contains the payload for the selected MO, if that operation requires a data "
               "buffer. The buffer bytes are not part of the 16 command dwords.");
    nvm_reserved_dwords(r, c, 11, 15, "Reserved for I/O Management Send; host shall set to zero.");
}
} // namespace nvme_decoder::detail
