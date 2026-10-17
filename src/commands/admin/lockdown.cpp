#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_admin_fields_lockdown(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Lockdown";
    r.use = "Prohibits or allows execution of a selected command or Set Features identifier on the "
            "selected interface.";
    const auto scope = c[10] & 0xfu, prohibit = (c[10] >> 4) & 1u, iface = (c[10] >> 5) & 3u,
               target = (c[10] >> 8) & 0xffu;
    const std::string scope_name = scope == 0   ? "Admin command opcode"
                                   : scope == 2 ? "Set Features Feature Identifier"
                                   : scope == 3 ? "NVMe-MI command opcode"
                                   : scope == 4 ? "PCIe command opcode"
                                                : "reserved";
    const std::string iface_name =
        iface == 0   ? "Admin Submission Queue"
        : iface == 1 ? "Admin Submission Queue and out-of-band Management Endpoint"
        : iface == 2 ? "out-of-band Management Endpoint"
                     : "reserved";
    field(r, "Scope", "CDW10[3:0]", scope, std::to_string(scope) + " (" + scope_name + ")",
          "Determines whether the target field names an Admin opcode, a Set Features FID, or an "
          "interface-specific management opcode.");
    field(r, "Prohibit", "CDW10[4]", prohibit,
          prohibit ? "1 (prohibit execution)" : "0 (allow execution)",
          "Prohibits or allows execution of the identified command or feature within the selected "
          "scope and interface.");
    field(r, "Interface", "CDW10[6:5]", iface, std::to_string(iface) + " (" + iface_name + ")",
          "Selects the interface(s) to which the lockdown rule applies.");
    field(r, "Reserved", "CDW10[7]", (c[10] >> 7) & 1u, std::to_string((c[10] >> 7) & 1u),
          "Reserved; host shall set to zero.");
    field(r, "Opcode or Feature Identifier", "CDW10[15:8]", target, hex_value(target, 2),
          "Target command opcode or Feature Identifier, interpreted according to Scope.", 2);
    field(r, "Reserved", "CDW10[31:16]", c[10] >> 16, std::to_string(c[10] >> 16),
          "Reserved; host shall set to zero.");
    field(r, "UUID Index", "CDW14[6:0]", c[14] & 0x7fu, std::to_string(c[14] & 0x7fu),
          "Selects the UUID for applicable vendor-specific Set Features lockdown rules when UUID "
          "selection is supported.");
    field(r, "Reserved", "CDW14[31:7]", c[14] >> 7, std::to_string(c[14] >> 7),
          "Reserved; host shall set to zero.");
    reserved_dwords(r, c, 11, 13);
    field(r, "Reserved", "CDW15[31:0]", c[15], hex_value(c[15], 8),
          "Reserved; host shall set to zero.", 8);
}
} // namespace nvme_decoder::detail
