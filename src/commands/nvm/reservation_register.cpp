#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_reservation_register(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Reservation Register";
    r.use = "Registers, replaces, or removes a reservation key for the namespace.";
    field(r, "Reservation Register Action", "CDW10[2:0]", c[10] & 7u,
          (c[10] & 7u) == 0   ? "0 (Register)"
          : (c[10] & 7u) == 1 ? "1 (Unregister)"
          : (c[10] & 7u) == 2 ? "2 (Replace)"
                              : "reserved",
          "Selects register, unregister, or replace-key action.");
    field(r, "Ignore Existing Key", "CDW10[3]", (c[10] >> 3) & 1u,
          std::to_string((c[10] >> 3) & 1u),
          "When set, ignore the current reservation key comparison.");
    field(r, "Reserved", "CDW10[29:4]", (c[10] >> 4) & 0x03ffffffu,
          std::to_string((c[10] >> 4) & 0x03ffffffu), "Reserved; host shall set to zero.");
    field(r, "Change Persist Through Power Loss State", "CDW10[31:30]", c[10] >> 30,
          (c[10] >> 30) == 0   ? "0 (no change)"
          : (c[10] >> 30) == 1 ? "1 (set persist through power loss)"
          : (c[10] >> 30) == 2 ? "2 (clear persist through power loss)"
                               : "3 (reserved)",
          "CPTPL controls whether the persist-through-power-loss state is changed.");
    field_text(r, "Current Reservation Key", "Data buffer[63:0]", "N/A",
               "Provided in the command data buffer",
               "The 64-bit current reservation key is supplied in the data buffer.");
    field_text(r, "New Reservation Key", "Data buffer[127:64]", "N/A",
               "Provided in the command data buffer",
               "The 64-bit replacement key is supplied in the data buffer.");
    nvm_reserved_dwords(r, c, 11, 15);
}
} // namespace nvme_decoder::detail
