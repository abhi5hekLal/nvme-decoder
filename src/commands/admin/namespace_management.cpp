#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_namespace_management(DecodedCommand& r, const Command& c,
                                              std::uint8_t op) {
    (void)op;
    r.command_name = "Namespace Management";
    r.use = "Creates or deletes a namespace according to the Namespace Management Select field.";
    const auto select = c[10] & 3u;
    field(r, "Namespace Management Select", "CDW10[1:0]", select,
          select == 0   ? "0 (Create Namespace)"
          : select == 1 ? "1 (Delete Namespace)"
                        : "reserved",
          "0b selects namespace creation and uses the Namespace Data structure. 1b selects "
          "namespace deletion and does not transfer Namespace Data. 2b and 3b are reserved.");
    field(r, "Reserved", "CDW10[31:2]", c[10] >> 2, std::to_string(c[10] >> 2),
          "Reserved; host shall set to zero.");
    if (select == 0)
        field_text(
            r, "Namespace Data", "Data buffer", "Not encoded in command dwords",
            "Transferred through the Data Pointer",
            "Contains the Namespace Management data structure used to create the namespace.");
    reserved_dwords(r, c, 11, 15);
}
} // namespace nvme_decoder::detail
