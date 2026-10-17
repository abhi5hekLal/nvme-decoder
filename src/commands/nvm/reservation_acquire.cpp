#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_reservation_acquire(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Reservation Acquire";
    r.use = "Acquires a reservation, preempts a reservation, or preempts and aborts commands.";
    field(r, "Reservation Acquire Action", "CDW10[2:0]", c[10] & 7u,
          (c[10] & 7u) == 0   ? "0 (Acquire)"
          : (c[10] & 7u) == 1 ? "1 (Preempt)"
          : (c[10] & 7u) == 2 ? "2 (Preempt and Abort)"
                              : "reserved",
          "Selects acquire, preempt, or preempt-and-abort action.");
    field(r, "Ignore Existing Key", "CDW10[3]", (c[10] >> 3) & 1u,
          std::to_string((c[10] >> 3) & 1u),
          "Controls whether the current reservation key check is ignored; support and behavior "
          "depend on the applicable specification revision.");
    field(r, "Reserved", "CDW10[7:4]", (c[10] >> 4) & 0xfu, std::to_string((c[10] >> 4) & 0xfu),
          "Reserved; host shall set to zero.");
    const auto rtype = (c[10] >> 8) & 0xffu;
    const std::string rtype_name = rtype == 1   ? "Write Exclusive"
                                   : rtype == 2 ? "Exclusive Access"
                                   : rtype == 3 ? "Write Exclusive - Registrants Only"
                                   : rtype == 4 ? "Exclusive Access - Registrants Only"
                                   : rtype == 5 ? "Write Exclusive - All Registrants"
                                   : rtype == 6 ? "Exclusive Access - All Registrants"
                                                : "reserved";
    field(r, "Reservation Type", "CDW10[15:8]", rtype, rtype_name,
          "Selects the reservation type to acquire.");
    field(r, "Reserved", "CDW10[31:16]", c[10] >> 16, std::to_string(c[10] >> 16),
          "Reserved; host shall set to zero.");
    field_text(r, "Current Reservation Key", "Data buffer[63:0]", "N/A",
               "Provided in the command data buffer", "64-bit key used to verify the registrant.");
    field_text(r, "Preempt Reservation Key", "Data buffer[127:64]", "N/A",
               "Provided in the command data buffer",
               "64-bit key identifying registrations to preempt.");
    nvm_reserved_dwords(r, c, 11, 15);
}
} // namespace nvme_decoder::detail
