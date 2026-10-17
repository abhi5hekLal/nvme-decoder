#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_namespace_attachment(DecodedCommand& r, const Command& c,
                                              std::uint8_t op) {
    (void)op;
    const auto select = c[10] & 3u;
    r.command_name = "Namespace Attachment";
    r.use =
        "Attaches or detaches the namespace to the controllers in the supplied Controller List.";
    field(r, "Namespace Attachment Select", "CDW10[1:0]", select,
          select == 0   ? "0 (Attach Namespace)"
          : select == 1 ? "1 (Detach Namespace)"
                        : "reserved",
          "0b selects attach and 1b selects detach. 2b and 3b are reserved.");
    field(r, "Reserved", "CDW10[31:2]", c[10] >> 2, std::to_string(c[10] >> 2),
          "Reserved; host shall set to zero.");
    if (select <= 1)
        field_text(
            r, "Controller List", "Data buffer", "Not encoded in command dwords",
            "Transferred through the Data Pointer",
            "Contains the controller identifiers to attach to or detach from the namespace.");
    reserved_dwords(r, c, 11, 15);
}
} // namespace nvme_decoder::detail
