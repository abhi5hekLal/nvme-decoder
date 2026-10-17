#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_nvme_mi_receive(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "NVMe-MI Receive";
    r.use = "Receives an NVMe-MI Response Message into the command data buffer.";
    field(r, "NVMe-MI Specific 0", "CDW10[7:0]", c[10] & 0xffu, hex_value(c[10] & 0xffu, 2),
          "Low byte of the NVMe-MI-specific field; interpretation is defined by the selected "
          "NVMe-MI message.",
          2);
    field(r, "Reserved", "CDW10[31:8]", c[10] >> 8, std::to_string(c[10] >> 8),
          "Reserved; host shall set to zero.");
    field(r, "Number of Dwords", "CDW11[31:0]", c[11],
          std::to_string(static_cast<std::uint64_t>(c[11]) + 1u),
          "Zero-based length of the NVMe-MI message buffer: NUMD + 1 dwords.");
    field_text(r, "Message Buffer", "Data buffer", "Not present in command dwords",
               "Transferred through the Data Pointer",
               op == 0x1d ? "Contains an NVMe-MI Request Message sent to the controller."
                          : "Receives an NVMe-MI Response Message from the controller.");
    reserved_dwords(r, c, 12, 15);
}
} // namespace nvme_decoder::detail
