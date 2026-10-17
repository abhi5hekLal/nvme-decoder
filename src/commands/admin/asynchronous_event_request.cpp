#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_asynchronous_event_request(DecodedCommand& r, const Command& c,
                                                    std::uint8_t op) {
    r.command_name = "Asynchronous Event Request";
    r.use = "Submits a request that the controller completes when an enabled asynchronous event is "
            "ready.";
    // Asynchronous Event Request
    reserved_dwords(r, c, 10, 15);
    r.use =
        "Posts an asynchronous event request. The controller completes it when an enabled event is "
        "ready; event type and information are returned in the completion entry, not this command.";
}
} // namespace nvme_decoder::detail
