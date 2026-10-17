#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_doorbell_buffer_config(DecodedCommand& r, const Command& c,
                                                std::uint8_t op) {
    r.command_name = "Doorbell Buffer Config";
    r.use = "Configures the Shadow Doorbell and EventIdx buffers used by a virtualized controller.";
    field_text(r, "Shadow Doorbell Buffer", "PRP Entry 1 / Data Pointer[63:0]",
               "Raw pointer shown in the Data Pointer field", "64-bit PRP1 base address",
               "Points to the page-aligned host-updated shadow doorbell buffer; its entries mirror "
               "SQ tail and CQ head doorbells.");
    field_text(r, "Event Index Buffer", "PRP Entry 2 / Data Pointer[127:64]",
               "Raw pointer shown in the Data Pointer field", "64-bit PRP2 base address",
               "Points to the page-aligned controller-updated EventIdx buffer used to reduce "
               "unnecessary doorbell writes.");
    reserved_dwords(r, c, 10, 15);
}
} // namespace nvme_decoder::detail
