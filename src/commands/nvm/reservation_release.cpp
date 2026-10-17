#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_reservation_release(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Reservation Release";
    r.use = "Releases the current reservation or clears registrations and the reservation.";
    field(r, "Reservation Release Action", "CDW10[2:0]", c[10] & 7u,
          (c[10] & 7u) == 0   ? "0 (Release)"
          : (c[10] & 7u) == 1 ? "1 (Clear)"
                              : "reserved",
          "Selects release or clear action.");
    field(r, "Reserved", "CDW10[31:3]", c[10] >> 3, std::to_string(c[10] >> 3),
          "Reserved; host shall set to zero.");
    field_text(r, "Current Reservation Key", "Data buffer[63:0]", "N/A",
               "Provided in the command data buffer", "64-bit key used to verify the registrant.");
    nvm_reserved_dwords(r, c, 11, 15);
}
} // namespace nvme_decoder::detail
