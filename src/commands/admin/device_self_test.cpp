#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_device_self_test(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Device Self-test";
    r.use = "Starts, reports, or aborts a device self-test according to the Self-test Code.";
    const auto stc = c[10] & 0xfu;
    const std::string stc_name = stc == 1    ? "1 (short self-test)"
                                 : stc == 2  ? "2 (extended self-test)"
                                 : stc == 3  ? "3 (Host-Initiated Refresh)"
                                 : stc == 14 ? "14 (vendor-specific)"
                                 : stc == 15 ? "15 (abort self-test)"
                                             : "reserved";
    field(r, "Self-test Code", "CDW10[3:0]", stc, stc_name, "Selects the device self-test action.");
    field(r, "Reserved", "CDW10[31:4]", c[10] >> 4, std::to_string(c[10] >> 4),
          "Reserved; host shall set to zero.");
    reserved_dwords(r, c, 11, 14);
    if (stc == 0xe)
        field(r, "Device Self-test Parameter", "CDW15[31:0]", c[15], hex_value(c[15], 8),
              "Vendor-specific parameter when STC is Eh.", 8);
    else
        field(r, "Reserved", "CDW15[31:0]", c[15], hex_value(c[15], 8),
              "Reserved unless the vendor-specific self-test code is selected.", 8);
}
} // namespace nvme_decoder::detail
