#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_format_nvm(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Format NVM";
    r.use = "Formats the namespace using the selected LBA format, metadata settings, protection "
            "information, and secure erase action.";
    field(r, "LBA Format", "CDW10[3:0]", c[10] & 0xfu, std::to_string(c[10] & 0xfu),
          "Selects the LBA format to apply to the namespace.");
    field(
        r, "Metadata Settings", "CDW10[4]", (c[10] >> 4) & 1u, std::to_string((c[10] >> 4) & 1u),
        "Selects metadata placement (extended LBA or separate metadata) when metadata is present.");
    field(r, "Protection Information", "CDW10[7:5]", (c[10] >> 5) & 7u,
          std::to_string((c[10] >> 5) & 7u),
          "Selects the protection information format; zero disables protection information.");
    field(r, "Protection Information Location", "CDW10[8]", (c[10] >> 8) & 1u,
          std::to_string((c[10] >> 8) & 1u),
          "Selects protection information location relative to metadata when PI is enabled.");
    field(r, "Secure Erase Settings", "CDW10[11:9]", (c[10] >> 9) & 7u,
          std::to_string((c[10] >> 9) & 7u),
          "Selects no secure erase, user data erase, or cryptographic erase.");
    field(r, "Reserved", "CDW10[31:12]", c[10] >> 12, std::to_string(c[10] >> 12),
          "Reserved; host shall set to zero.");
    reserved_dwords(r, c, 11, 15);
}
} // namespace nvme_decoder::detail
