#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_abort(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Abort";
    r.use = "Requests cancellation of a command identified by its Submission Queue and Command "
            "Identifier.";
    field(r, "Submission Queue Identifier", "CDW10[15:0]", c[10] & 0xffffu,
          std::to_string(c[10] & 0xffffu),
          "Identifies the submission queue containing the command to abort.");
    field(r, "Command Identifier", "CDW10[31:16]", c[10] >> 16, std::to_string(c[10] >> 16),
          "Identifies the command to abort within the submission queue.");
    reserved_dwords(r, c, 11, 15);
}
} // namespace nvme_decoder::detail
