#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_delete_io_submission_queue(DecodedCommand& r, const Command& c,
                                                    std::uint8_t op) {
    r.command_name = "Delete I/O Submission Queue";
    r.use =
        "Deletes the specified I/O Submission Queue after commands in it complete or are aborted.";
    queue_fields(r, c, false, false);
}
} // namespace nvme_decoder::detail
