#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_create_io_completion_queue(DecodedCommand& r, const Command& c,
                                                    std::uint8_t op) {
    r.command_name = "Create I/O Completion Queue";
    r.use = "Creates an I/O Completion Queue with the requested entry count and interrupt "
            "configuration.";
    queue_fields(r, c, true, true);
}
} // namespace nvme_decoder::detail
