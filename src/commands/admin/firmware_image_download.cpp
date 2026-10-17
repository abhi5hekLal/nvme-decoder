#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_firmware_image_download(DecodedCommand& r, const Command& c,
                                                 std::uint8_t op) {
    r.command_name = "Firmware Image Download";
    r.use = "Transfers a range of firmware image dwords into the controller for a later Firmware "
            "Commit.";
    field(r, "Number of Dwords", "CDW10[31:0]", c[10], std::to_string(c[10] + 1ull),
          "Zero-based firmware transfer length: number of dwords transferred is NUMD + 1.");
    field(r, "Offset", "CDW11[31:0]", c[11], std::to_string(c[11]) + " dwords",
          "Zero-based dword offset from the start of the firmware image for this downloaded "
          "segment.");
    reserved_dwords(r, c, 12, 15);
}
} // namespace nvme_decoder::detail
