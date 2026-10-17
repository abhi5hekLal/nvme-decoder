#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_copy(DecodedCommand& r, const Command& c, std::uint8_t op) {
    (void)op;
    r.command_name = "Copy";
    r.use = "Copies one or more source logical block ranges to a destination range.";
    const auto destination_lba = (static_cast<std::uint64_t>(c[11]) << 32) | c[10];
    field(r, "Starting Destination LBA (SDLBA)", "CDW10:CDW11[63:0]", destination_lba,
          std::to_string(destination_lba),
          "64-bit address of the first logical block in the destination range.", 16);
    const auto tag_upper = (static_cast<std::uint64_t>(c[3] & 0xffffu) << 32) | c[2];
    field(r, "Logical Block Tags Upper", "CDW2:CDW3[47:0]", tag_upper, hex_value(tag_upper, 12),
          "Upper portion of the variable-sized logical block storage tag and initial logical block "
          "reference tag for the write portion of Copy. Ignored if the destination namespace is "
          "not formatted for end-to-end protection information.",
          12);
    field(r, "Reserved", "CDW2:CDW3[63:48]", c[3] >> 16, std::to_string(c[3] >> 16),
          "Reserved; host shall set to zero.");
    field(r, "Number of Source Ranges", "CDW12[7:0]", c[12] & 0xffu,
          std::to_string((c[12] & 0xffu) + 1u),
          "Zero-based number of source range descriptors in the data buffer.");
    field(r, "Descriptor Format", "CDW12[11:8]", (c[12] >> 8) & 0xfu,
          std::to_string((c[12] >> 8) & 0xfu),
          "Selects the Source Range Entry format; valid descriptor formats depend on controller "
          "support and host behavior settings.");
    field(r, "Protection Information Read", "CDW12[15:12]", (c[12] >> 12) & 0xfu,
          hex_value((c[12] >> 12) & 0xfu, 1),
          "PRINFOR: protection-information actions/checks for the read portion of the copy.");
    field(r, "Command Extension Type", "CDW12[19:16]", (c[12] >> 16) & 0xfu,
          std::to_string((c[12] >> 16) & 0xfu),
          "Selects command extension interpretation for the copy command.");
    field(r, "Directive Type", "CDW12[23:20]", (c[12] >> 20) & 0xfu,
          std::to_string((c[12] >> 20) & 0xfu),
          "Selects the directive type associated with the directive-specific field.");
    field(r, "Storage Tag Check Read", "CDW12[25]", (c[12] >> 25) & 1u,
          std::to_string((c[12] >> 25) & 1u),
          "Requests storage-tag checking for the read portion of the copy.");
    field(r, "Protection Information Write", "CDW12[29:26]", (c[12] >> 26) & 0xfu,
          hex_value((c[12] >> 26) & 0xfu, 1),
          "PRINFOW: protection-information actions/checks for the write portion of the copy.");
    field(r, "Force Unit Access", "CDW12[30]", (c[12] >> 30) & 1u,
          std::to_string((c[12] >> 30) & 1u),
          "When set, copied data and metadata are committed to non-volatile media before command "
          "completion.");
    field(r, "Limited Retry", "CDW12[31]", c[12] >> 31, std::to_string(c[12] >> 31),
          "Requests limited retry effort for the write portion of the copy.");
    field(r, "Directive Specific", "CDW13[31:16]", c[13] >> 16, std::to_string(c[13] >> 16),
          "Directive-specific value associated with DTYPE.");
    field(r, "Command Extension Value", "CDW13[15:0]", c[13] & 0xffffu,
          hex_value(c[13] & 0xffffu, 4),
          "Command-extension value for the write portion; interpreted according to CETYPE.", 4);
    field_text(r, "Source Range Entries", "Data buffer", "Not present in command dwords",
               "Transferred through the Data Pointer",
               "The data buffer contains NR + 1 source range descriptors in the selected "
               "descriptor format.");
    field(r, "Logical Block Tags Lower", "CDW14[31:0]", c[14], hex_value(c[14], 8),
          "Lower portion of the storage/reference tag for the write portion of the copy.", 8);
    field(r, "Logical Block Application Tag", "CDW15[15:0]", c[15] & 0xffffu,
          hex_value(c[15] & 0xffffu, 4), "Application tag for the write portion of the copy.", 4);
    field(r, "Logical Block Application Tag Mask", "CDW15[31:16]", c[15] >> 16,
          hex_value(c[15] >> 16, 4), "Application-tag mask for the write portion of the copy.", 4);
}
} // namespace nvme_decoder::detail
