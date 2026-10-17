#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_virtualization_management(DecodedCommand& r, const Command& c,
                                                   std::uint8_t op) {
    r.command_name = "Virtualization Management";
    r.use = "Allocates, assigns, or changes the state of controller resources.";
    field(r, "Controller Identifier", "CDW10[31:16]", c[10] >> 16, std::to_string(c[10] >> 16),
          "Controller whose resources are being modified.");
    field(r, "Reserved", "CDW10[15:11]", (c[10] >> 11) & 0x1fu,
          std::to_string((c[10] >> 11) & 0x1fu), "Reserved; host shall set to zero.");
    const auto resource = (c[10] >> 8) & 7u;
    field(r, "Resource Type", "CDW10[10:8]", resource,
          resource == 0   ? "0 (Virtual Queues)"
          : resource == 1 ? "1 (Virtual Interrupts)"
                          : "reserved",
          "Selects the type of controller resource to modify.");
    field(r, "Reserved", "CDW10[7:4]", (c[10] >> 4) & 0xfu, std::to_string((c[10] >> 4) & 0xfu),
          "Reserved; host shall set to zero.");
    const auto act = c[10] & 0xfu;
    const std::string action = act == 1   ? "1 (Primary Controller Flexible Allocation)"
                               : act == 7 ? "7 (Secondary Controller Offline)"
                               : act == 8 ? "8 (Secondary Controller Assign)"
                               : act == 9 ? "9 (Secondary Controller Online)"
                                          : "reserved";
    field(r, "Action", "CDW10[3:0]", act, action,
          "Selects the virtualization management operation.");
    field(r, "Number of Controller Resources", "CDW11[15:0]", c[11] & 0xffffu,
          std::to_string(c[11] & 0xffffu),
          "Number of flexible controller resources to allocate or assign for applicable actions.");
    field(r, "Reserved", "CDW11[31:16]", c[11] >> 16, std::to_string(c[11] >> 16),
          "Reserved; host shall set to zero.");
    reserved_dwords(r, c, 12, 15);
}
} // namespace nvme_decoder::detail
