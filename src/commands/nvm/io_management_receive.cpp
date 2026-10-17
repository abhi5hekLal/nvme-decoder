#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_io_management_receive(DecodedCommand& r, const Command& c, std::uint8_t op) {
    (void)op;
    r.command_name = "I/O Management Receive";
    const auto mo = c[10] & 0xffu;
    r.use = mo == 1
                ? "Receives Reclaim Unit Handle Status information for the specified namespace."
                : "Receives I/O-management information selected by the Management Operation field.";
    field(r, "Management Operation (MO)", "CDW10[7:0]", mo,
          mo == 0      ? "0x00 (no action)"
          : mo == 1    ? "0x01 (Reclaim Unit Handle Status)"
          : mo == 0xff ? "0xFF (vendor-specific)"
                       : hex_value(mo, 2),
          "Selects the I/O-management receive operation. Other values are reserved unless defined "
          "by an applicable specification or vendor extension.",
          2);
    field(r, "Reserved", "CDW10[15:8]", (c[10] >> 8) & 0xffu, std::to_string((c[10] >> 8) & 0xffu),
          "Reserved; host shall set to zero.");
    field(r, "Management Operation Specific (MOS)", "CDW10[31:16]", c[10] >> 16,
          hex_value(c[10] >> 16, 4),
          "Meaning is specific to MO. If the selected operation does not define MOS, this field is "
          "reserved.",
          4);
    field(r, "Number of Dwords (NUMD)", "CDW11[31:0]", c[11],
          std::to_string(static_cast<std::uint64_t>(c[11]) + 1u),
          "Zero-based transfer length: requested data-buffer length is NUMD + 1 dwords. The "
          "returned structure may be shorter than this request.");
    field_text(r, "Management Result Data", "Data Pointer",
               "Address shown in common pointer fields", "Controller-to-host data buffer",
               "Contains the data structure selected by MO. Decoding its entries requires the "
               "transferred bytes and the applicable I/O Command Set specification.");
    nvm_reserved_dwords(r, c, 12, 15,
                        "Reserved for I/O Management Receive; host shall set to zero.");
}
} // namespace nvme_decoder::detail
