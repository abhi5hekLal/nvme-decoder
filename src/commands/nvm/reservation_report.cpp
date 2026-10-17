#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_reservation_report(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Reservation Report";
    r.use = "Returns the namespace reservation status and registered controller information.";
    field(r, "Number of Dwords", "CDW10[31:0]", c[10],
          std::to_string(static_cast<std::uint64_t>(c[10]) + 1u),
          "Zero-based number of dwords of the Reservation Status data structure to return; a "
          "shorter request returns a prefix, and an overlong request returns only the structure.");
    field(r, "Extended Data Structure", "CDW11[0]", c[11] & 1u, std::to_string(c[11] & 1u),
          "Selects the extended Reservation Status data structure when set.");
    field(r, "Dispersed Namespace Reservation Support", "CDW11[1]", (c[11] >> 1) & 1u,
          std::to_string((c[11] >> 1) & 1u),
          "Indicates host support for reservations on dispersed namespaces when set.");
    field(r, "Reserved", "CDW11[31:2]", c[11] >> 2, std::to_string(c[11] >> 2),
          "Reserved; host shall set to zero.");
    nvm_reserved_dwords(r, c, 12, 15);
}
} // namespace nvme_decoder::detail
