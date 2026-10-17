#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_delete_io_completion_queue(DecodedCommand& r, const Command& c,
                                                    std::uint8_t op) {
    r.command_name = "Delete I/O Completion Queue";
    r.use = "Deletes the specified I/O Completion Queue after associated Submission Queues have "
            "been deleted.";
    queue_fields(r, c, false, true);
}
} // namespace nvme_decoder::detail
