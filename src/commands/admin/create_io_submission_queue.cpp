#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_create_io_submission_queue(DecodedCommand& r, const Command& c,
                                                    std::uint8_t op) {
    r.command_name = "Create I/O Submission Queue";
    r.use =
        "Creates an I/O Submission Queue and associates it with the specified Completion Queue.";
    queue_fields(r, c, true, false);
}
} // namespace nvme_decoder::detail
