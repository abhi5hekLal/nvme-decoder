#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_cancel(DecodedCommand& r, const Command& c, std::uint8_t op) {
    (void)op;
    r.command_name = "Cancel";
    r.use = "Requests that the controller abort one specified command, or multiple commands, on "
            "this I/O Submission Queue.";
    field(r, "Submission Queue Identifier (SQID)", "CDW10[15:0]", c[10] & 0xffffu,
          std::to_string(c[10] & 0xffffu),
          "Identifies the Submission Queue containing the command(s) to cancel. It shall match the "
          "queue to which this Cancel command is submitted.");
    field(r, "Command Identifier (CID)", "CDW10[31:16]", c[10] >> 16, hex_value(c[10] >> 16, 4),
          "Identifies the target command for Single Command Cancel. Multiple Command Cancel uses "
          "FFFFh to select all eligible commands.",
          4);
    const auto acode = c[11] & 3u;
    field(r, "Action Code (ACODE)", "CDW11[1:0]", acode,
          acode == 0   ? "0 (Single Command Cancel)"
          : acode == 1 ? "1 (Multiple Command Cancel)"
                       : "reserved",
          "00b requests cancellation of the specified CID. 01b requests cancellation of multiple "
          "commands and requires CID=FFFFh. Other values are reserved.");
    field(r, "Reserved", "CDW11[31:2]", c[11] >> 2, std::to_string(c[11] >> 2),
          "Reserved; host shall set to zero.");
    field_text(r, "Cancel Completion Result", "Completion DW0", "Not present in command",
               "Returned by the controller",
               "Completion DW0 reports whether commands were eligible for deferred abort and "
               "whether any command was aborted.");
    nvm_reserved_dwords(r, c, 12, 15, "Reserved for Cancel; host shall set to zero.");
}
} // namespace nvme_decoder::detail
