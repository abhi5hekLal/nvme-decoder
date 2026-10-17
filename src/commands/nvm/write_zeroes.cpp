#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_write_zeroes(DecodedCommand& r, const Command& c, std::uint8_t op) {
    (void)op;
    r.command_name = "Write Zeroes";
    r.use = "Writes zeroes to the specified logical block range.";
    nvm_slba(r, c);
    nvm_nlb(r, c);
    const auto tag_upper = (static_cast<std::uint64_t>(c[3] & 0xffffu) << 32) | c[2];
    field(r, "Logical Block Tags Upper", "CDW2:CDW3[47:0]", tag_upper, hex_value(tag_upper, 12),
          "Upper portion of the variable-sized logical block storage tag and initial logical block "
          "reference tag. Ignored if the namespace is not formatted for end-to-end protection "
          "information.",
          12);
    field(r, "Reserved", "CDW2:CDW3[63:48]", c[3] >> 16, std::to_string(c[3] >> 16),
          "Reserved; host shall set to zero.");
    field(r, "Limited Retry", "CDW12[31]", c[12] >> 31, std::to_string(c[12] >> 31),
          "Requests limited retry effort during error recovery.");
    field(r, "Force Unit Access", "CDW12[30]", (c[12] >> 30) & 1u,
          std::to_string((c[12] >> 30) & 1u),
          "When set, associated data and metadata are committed to non-volatile media before "
          "completion.");
    const auto prinfo = (c[12] >> 26) & 0xfu;
    field(r, "Protection Information (PRINFO)", "CDW12[29:26]", prinfo, hex_value(prinfo, 1),
          "PRACT is bit 29; PRCHK is bits 28:26. Valid use depends on namespace "
          "protection-information format; PRCHK is reserved for this command.");
    field(r, "Protection Information Action (PRACT)", "CDW12[29]", (c[12] >> 29) & 1u,
          std::to_string((c[12] >> 29) & 1u),
          "Controls generation/removal of protection information when supported by the namespace "
          "format.");
    field(r, "Protection Information Check (PRCHK)", "CDW12[28:26]", (c[12] >> 26) & 7u,
          hex_value((c[12] >> 26) & 7u, 1), "PRCHK shall be cleared to 000b for Write Zeroes.");
    field(r, "Deallocate (DEAC)", "CDW12[25]", (c[12] >> 25) & 1u,
          std::to_string((c[12] >> 25) & 1u),
          "Requests deallocation of the specified logical blocks when set.");
    field(r, "Storage Tag Check (STC)", "CDW12[24]", (c[12] >> 24) & 1u,
          std::to_string((c[12] >> 24) & 1u), "Reserved; host shall set to zero.");
    field(r, "Namespace Zeroes (NSZ)", "CDW12[23]", (c[12] >> 23) & 1u,
          std::to_string((c[12] >> 23) & 1u),
          "When supported and DEAC is set, requests deallocation of all logical blocks in the "
          "namespace and requires SLBA and NLB to be zero. Controller support is reported in "
          "Identify Controller ONCS.NSZS.");
    field(r, "Directive Type (DTYPE)", "CDW12[22:20]", (c[12] >> 20) & 7u,
          hex_value((c[12] >> 20) & 7u, 1),
          "Selects the directive type associated with DSPEC in CDW13[31:16].");
    const auto cetype = (c[12] >> 16) & 0xfu;
    field(r, "Command Extension Type (CETYPE)", "CDW12[19:16]", cetype, hex_value(cetype, 1),
          "Selects command-extension interpretation for CDW13[15:0]; zero means that portion of "
          "CDW13 is reserved.");
    field(r, "Directive Specific (DSPEC)", "CDW13[31:16]", c[13] >> 16, hex_value(c[13] >> 16, 4),
          "Directive-specific value associated with DTYPE.", 4);
    if (cetype == 0)
        field(r, "Reserved", "CDW13[15:0]", c[13] & 0xffffu, std::to_string(c[13] & 0xffffu),
              "Reserved when CETYPE is zero; host shall set to zero.");
    else
        field(r, "Command Extension Value (CEV)", "CDW13[15:0]", c[13] & 0xffffu,
              hex_value(c[13] & 0xffffu, 4),
              "Command-extension value interpreted according to CETYPE.", 4);
    field(r, "Initial Logical Block Reference Tag", "CDW14[31:0]", c[14], hex_value(c[14], 8),
          "Initial reference tag used when generating protection information, as defined by the "
          "namespace format.",
          8);
    field(r, "Logical Block Application Tag", "CDW15[15:0]", c[15] & 0xffffu,
          hex_value(c[15] & 0xffffu, 4),
          "Application tag used when generating protection information.", 4);
    field(r, "Logical Block Application Tag Mask", "CDW15[31:16]", c[15] >> 16,
          hex_value(c[15] >> 16, 4),
          "Mask applied to the application tag during protection-information generation.", 4);
}
} // namespace nvme_decoder::detail
