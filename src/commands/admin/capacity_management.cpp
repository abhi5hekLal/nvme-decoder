#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_capacity_management(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Capacity Management";
    r.use = "Selects a capacity configuration or creates/deletes an Endurance Group or NVM Set.";
    const auto oper = c[10] & 0xfu;
    const auto elid = c[10] >> 16;
    const std::string on = oper == 0   ? "Select Capacity Configuration"
                           : oper == 1 ? "Create Endurance Group"
                           : oper == 2 ? "Delete Endurance Group"
                           : oper == 3 ? "Create NVM Set"
                           : oper == 4 ? "Delete NVM Set"
                                       : "Reserved operation";
    field(r, "Operation", "CDW10[3:0]", oper, std::to_string(oper) + " (" + on + ")",
          "Selects the capacity-management operation.");
    field(r, "Reserved", "CDW10[15:4]", (c[10] >> 4) & 0xfffu,
          std::to_string((c[10] >> 4) & 0xfffu), "Reserved; host shall set to zero.");
    field(r, "Element Identifier", "CDW10[31:16]", elid, std::to_string(elid),
          "Identifies the capacity configuration, Endurance Group, or NVM Set according to the "
          "selected operation. For create operations, zero requests controller selection of the "
          "parent domain/group where specified.");
    if (oper == 1 || oper == 3) {
        const auto capacity = (static_cast<std::uint64_t>(c[12]) << 32) | c[11];
        field(r, "Capacity Lower", "CDW11[31:0]", c[11], hex_value(c[11], 8),
              "Least significant 32 bits of requested capacity in bytes.", 8);
        field(r, "Capacity Upper", "CDW12[31:0]", c[12], hex_value(c[12], 8),
              "Most significant 32 bits of requested capacity in bytes.", 8);
        field(r, "Requested Capacity", "CDW12:CDW11[63:0]", capacity,
              std::to_string(capacity) + " bytes",
              "64-bit requested capacity in bytes, CAPU:CAPL.");
    } else {
        field_text(r, "Reserved command dwords", "CDW11-CDW12", reserved_dwords_raw(c, 11, 12),
                   reserved_dwords_decoded(c, 11, 12),
                   "Reserved for this operation; host shall set to zero.");
    }
    reserved_dwords(r, c, 13, 15);
}
} // namespace nvme_decoder::detail
