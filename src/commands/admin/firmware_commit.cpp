#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {

DecodedCommand decode_firmware_commit(const Command& c) {
    DecodedCommand out{"Firmware Commit",
                       "Modifies the firmware image or Boot Partitions using an image previously "
                       "transferred with Firmware Image Download.",
                       {}};

    const auto cdw0 = c[0];
    field(out, "Opcode", "CDW0[7:0]", cdw0 & 0xffu, "0x10 (Firmware Commit)",
          "Identifies this as the Admin Firmware Commit command.", 2);
    const auto fused = (cdw0 >> 8) & 3u;
    if (fused != 0)
        field(out, "Fused Operation", "CDW0[9:8]", fused, std::to_string(fused),
              "Reserved for Firmware Commit; host shall set to zero.");
    field(out, "Reserved command bits", "CDW0[15:10]", (cdw0 >> 10) & 0x3fu,
          hex_value((cdw0 >> 10) & 0x3fu), "Reserved for Firmware Commit; host shall set to zero.");
    field(out, "Command Identifier", "CDW0[31:16]", cdw0 >> 16, std::to_string(cdw0 >> 16),
          "Host-assigned command tag returned in the completion entry.");

    field_text(out, "Reserved command dwords", "CDW1-CDW3", reserved_dwords_raw(c, 1, 3),
               reserved_dwords_decoded(c, 1, 3),
               "Firmware Commit is not namespace-specific; CDW1 through CDW3 are reserved and "
               "shall be zero.");

    const auto fs = c[10] & 7u;
    const std::string fs_decoded =
        fs == 0 ? "0 (controller selects a firmware slot from 1 through 7)"
                : std::to_string(fs) + " (firmware slot " + std::to_string(fs) + ")";
    field(out, "Firmware Slot (FS)", "CDW10[2:0]", fs, fs_decoded,
          "Selects the firmware slot used by the Commit Action when applicable. An encoded value "
          "of zero lets the controller choose a slot.");

    const auto ca = (c[10] >> 3) & 7u;
    const char* ca_decoded =
        ca == 0   ? "000b (replace the image in the selected slot; do not activate it)"
        : ca == 1 ? "001b (replace the image and activate it at the next Controller Level Reset)"
        : ca == 2 ? "010b (activate the existing image in the selected slot at the next Controller "
                    "Level Reset)"
        : ca == 3 ? "011b (replace the image and activate it immediately)"
        : ca == 6 ? "110b (replace the specified Boot Partition)"
        : ca == 7 ? "111b (mark the specified Boot Partition active and update BPINFO.ABPID)"
                  : "reserved";
    field(out, "Commit Action (CA)", "CDW10[5:3]", ca, ca_decoded,
          "Selects whether the downloaded or previously placed firmware image is stored, activated "
          "after reset, or activated immediately; actions 6 and 7 operate on a Boot Partition.");
    field(out, "Reserved", "CDW10[30:6]", (c[10] >> 6) & 0x1ffffffu,
          std::to_string((c[10] >> 6) & 0x1ffffffu), "Reserved; host shall set to zero.");
    field(out, "Boot Partition Identifier (BPID)", "CDW10[31]", c[10] >> 31,
          std::to_string(c[10] >> 31) + " (Boot Partition " + std::to_string(c[10] >> 31) + ")",
          "Selects Boot Partition 0 or 1 for Commit Actions 6 and 7; ignored for other Commit "
          "Actions.");

    field_text(out, "Reserved command dwords", "CDW11-CDW15", reserved_dwords_raw(c, 11, 15),
               reserved_dwords_decoded(c, 11, 15),
               "Reserved for Firmware Commit; host shall set to zero.");
    return out;
}

} // namespace nvme_decoder::detail
