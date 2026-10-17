#include "command_decoders.hpp"
#include "field_helpers.hpp"
#include <string>
namespace nvme_decoder::detail {
namespace {
const char* feature_name(std::uint32_t id) {
    switch (id) {
    case 0x01:
        return "Arbitration";
    case 0x02:
        return "Power Management";
    case 0x03:
        return "LBA Range Type";
    case 0x04:
        return "Temperature Threshold";
    case 0x05:
        return "Error Recovery";
    case 0x06:
        return "Volatile Write Cache";
    case 0x07:
        return "Number of Queues";
    case 0x08:
        return "Interrupt Coalescing";
    case 0x09:
        return "Interrupt Vector Configuration";
    case 0x0a:
        return "Write Atomicity Normal";
    case 0x0b:
        return "Asynchronous Event Configuration";
    case 0x0c:
        return "Autonomous Power State Transition";
    case 0x0d:
        return "Host Memory Buffer";
    case 0x0e:
        return "Timestamp";
    case 0x0f:
        return "Keep Alive Timer";
    case 0x10:
        return "Host Controlled Thermal Management";
    case 0x11:
        return "Non-Operational Power State Config";
    case 0x12:
        return "Read Recovery Level";
    case 0x13:
        return "Predictable Latency Mode Config";
    case 0x14:
        return "Predictable Latency Mode Window";
    case 0x15:
        return "LBA Status Information Report Interval";
    case 0x16:
        return "Host Behavior Support";
    case 0x17:
        return "Sanitize Configuration";
    case 0x18:
        return "Endurance Group Event Configuration";
    case 0x19:
        return "I/O Command Set Profile";
    case 0x7d:
        return "Enhanced Controller Metadata";
    case 0x7e:
        return "Controller Metadata";
    case 0x7f:
        return "Namespace Metadata";
    case 0x80:
        return "Software Progress Marker";
    case 0x81:
        return "Host Identifier";
    case 0x82:
        return "Reservation Notification Mask";
    case 0x83:
        return "Reservation Persistence";
    case 0x84:
        return "Namespace Write Protection Config";
    case 0x85:
        return "Boot Partition Write Protection Config";
    default:
        return nullptr;
    }
}
void value_fields(DecodedCommand& r, std::uint32_t fid, const Command& c, bool get) {
    const auto v = c[11];
    switch (fid) {
    case 0x01:
        field(r, "Arbitration Burst", "CDW11[2:0]", v & 7u,
              (v & 7u) == 7 ? "7 (no limit)" : std::to_string(1u << ((v & 7u))),
              "AB encodes the maximum commands fetched from a submission queue in one arbitration "
              "burst: values 0–6 encode 1, 2, 4, 8, 16, 32, and 64; 7 means no limit.");
        field(r, "Low Priority Weight", "CDW11[15:8]", (v >> 8) & 0xffu,
              std::to_string((v >> 8) & 0xffu),
              "LPW controls the relative arbitration weight of the low-priority queue.");
        field(r, "Medium Priority Weight", "CDW11[23:16]", (v >> 16) & 0xffu,
              std::to_string((v >> 16) & 0xffu),
              "MPW controls the relative arbitration weight of medium-priority queues.");
        field(r, "High Priority Weight", "CDW11[31:24]", v >> 24, std::to_string(v >> 24),
              "HPW controls the relative arbitration weight of high-priority queues.");
        break;
    case 0x02:
        field(r, "Power State", "CDW11[4:0]", v & 0x1fu, std::to_string(v & 0x1fu),
              "Requested power state number from the controller's Identify Controller power-state "
              "descriptors.");
        field(r, "Workload Hint", "CDW11[5]", (v >> 5) & 1u,
              ((v >> 5) & 1u) ? "1 (non-operational workload)" : "0 (operational workload)",
              "Indicates whether the workload is expected to be operational or non-operational.");
        field(r, "Reserved", "CDW11[31:6]", v >> 6, std::to_string(v >> 6),
              "Reserved; host shall set to zero.");
        break;
    case 0x04:
        field(r, "Temperature Threshold", "CDW11[15:0]", v & 0xffffu,
              std::to_string(v & 0xffffu) + " K", "Threshold temperature in kelvins.");
        field(r, "Threshold Temperature Select", "CDW11[19:16]", (v >> 16) & 0xfu,
              std::to_string((v >> 16) & 0xfu),
              "Selects composite temperature (0), temperature sensor 1–8 (1–8), or all implemented "
              "sensors (F, Set Features only).");
        field(r, "Threshold Type Select", "CDW11[21:20]", (v >> 20) & 3u,
              ((v >> 20) & 3u) == 0   ? "0 (over-temperature)"
              : ((v >> 20) & 3u) == 1 ? "1 (under-temperature)"
                                      : "reserved",
              "Selects the over-temperature or under-temperature threshold.");
        field(r, "Temperature Threshold Hysteresis", "CDW11[24:22]", (v >> 22) & 7u,
              std::to_string((v >> 22) & 7u),
              "Hysteresis in kelvins for ending the temperature event; zero disables this "
              "hysteresis field.");
        field(r, "Reserved", "CDW11[31:25]", v >> 25, std::to_string(v >> 25),
              "Reserved; host shall set to zero.");
        break;
    case 0x05:
        field(r, "Time Limited Error Recovery", "CDW11[15:0]", v & 0xffffu,
              std::to_string(v & 0xffffu), "TLER in 100 ms units; zero disables the time limit.");
        field(r, "Deallocate Reads", "CDW11[16]", (v >> 16) & 1u, std::to_string((v >> 16) & 1u),
              "Controls whether deallocated or unwritten logical blocks return zeroes on reads.");
        break;
    case 0x06:
        field(r, "Volatile Write Cache", "CDW11[0]", v & 1u,
              (v & 1u) ? "1 (enabled)" : "0 (disabled)",
              "Enables or disables the volatile write cache.");
        break;
    case 0x07:
        field(r, "Number of I/O Submission Queues", "CDW11[15:0]", v & 0xffffu,
              std::to_string((v & 0xffffu) + 1u),
              "Zero-based count: number of I/O submission queues supported is encoded value plus "
              "one.");
        field(r, "Number of I/O Completion Queues", "CDW11[31:16]", v >> 16,
              std::to_string((v >> 16) + 1u),
              "Zero-based count: number of I/O completion queues supported is encoded value plus "
              "one.");
        break;
    case 0x08:
        field(r, "Aggregation Time", "CDW11[7:0]", v & 0xffu, std::to_string(v & 0xffu),
              "Time in 100 microsecond units before an interrupt is generated for a completion "
              "queue.");
        field(
            r, "Aggregation Threshold", "CDW11[15:8]", (v >> 8) & 0xffu,
            std::to_string(((v >> 8) & 0xffu) + 1u),
            "Zero-based number of completions: encoded threshold plus one triggers an interrupt.");
        field(r, "Reserved", "CDW11[31:16]", v >> 16, std::to_string(v >> 16),
              "Reserved; host shall set to zero.");
        break;
    case 0x09:
        field(r, "Interrupt Vector", "CDW11[15:0]", v & 0xffffu, std::to_string(v & 0xffffu),
              "Interrupt vector number being configured.");
        field(r, "Coalescing Disable", "CDW11[16]", (v >> 16) & 1u, std::to_string((v >> 16) & 1u),
              "Disables interrupt coalescing for the selected vector when set.");
        break;
    case 0x0a:
        field(r, "Disable Normal", "CDW11[0]", v & 1u, (v & 1u) ? "1 (disabled)" : "0 (enabled)",
              "Controls the Write Atomicity Normal feature.");
        break;
    case 0x0b:
        field(r, "Asynchronous Event Configuration", "CDW11[31:0]", v, hex_value(v, 8),
              "Bit mask enables supported asynchronous event classes; meaning of each bit is "
              "specified by the event type definitions.",
              8);
        break;
    case 0x0f:
        field(r, "Keep Alive Timer", "CDW11[15:0]", v & 0xffffu, std::to_string(v & 0xffffu),
              "Keep-alive timeout in milliseconds; zero disables the host keep-alive timer.");
        break;
    case 0x0e:
        field_text(r, "Timestamp Value", "Data buffer", "Not present in CDW11",
                   "Transferred through the command data pointer",
                   "Timestamp payload is carried in the command data buffer, not CDW11.");
        break;
    case 0x0c:
        field(r, "Autonomous Power State Transition Enable", "CDW11[0]", v & 1u,
              (v & 1u) ? "1 (enabled)" : "0 (disabled)",
              "Enables or disables autonomous transitions between power states based on the APST "
              "table.");
        field(r, "Reserved", "CDW11[31:1]", v >> 1, std::to_string(v >> 1),
              "Reserved; host shall set to zero.");
        field_text(r, "Autonomous Power State Transition Table", "Data buffer",
                   "Not present in CDW11", "Transferred through the command data pointer",
                   "The 256-byte APST table has 32 64-bit entries, one per power state; each entry "
                   "gives the idle time and target non-operational state.");
        break;
    case 0x0d:
        field(r, "Host Memory Buffer Enable", "CDW11[0]", v & 1u, std::to_string(v & 1u),
              "Enables or disables the Host Memory Buffer; remaining fields depend on the selected "
              "HMB action and data structures.");
        break;
    case 0x10:
        field(r, "Thermal Management Temperature 1", "CDW11[15:0]", v & 0xffffu,
              std::to_string(v & 0xffffu) + " K",
              "First temperature threshold, expressed in kelvins.");
        field(r, "Thermal Management Temperature 2", "CDW11[31:16]", v >> 16,
              std::to_string(v >> 16) + " K",
              "Second temperature threshold, expressed in kelvins.");
        break;
    case 0x14:
        field(r, "NVM Set Identifier", "CDW11[15:0]", v & 0xffffu, std::to_string(v & 0xffffu),
              "Identifies the NVM Set whose predictable-latency window is changed.");
        field(r, "Reserved", "CDW11[31:16]", v >> 16, std::to_string(v >> 16),
              "Reserved; host shall set to zero.");
        field(r, "Window Select", "CDW12[2:0]", c[12] & 7u,
              (c[12] & 7u) == 1   ? "1 (Deterministic Window)"
              : (c[12] & 7u) == 2 ? "2 (Non-Deterministic Window)"
                                  : "reserved",
              "Selects the window used by namespaces in the specified NVM Set.");
        field(r, "Reserved", "CDW12[31:3]", c[12] >> 3, std::to_string(c[12] >> 3),
              "Reserved; host shall set to zero.");
        break;
    case 0x17:
        field(r, "No-Deallocate Response Mode", "CDW11[0]", v & 1u, std::to_string(v & 1u),
              "Selects response behavior for Sanitize commands requesting no deallocation when "
              "no-deallocate is inhibited.");
        break;
    case 0x18:
        field(r, "Endurance Group Identifier", "CDW11[15:0]", v & 0xffffu,
              std::to_string(v & 0xffffu),
              "Identifies the endurance group whose critical warning events are configured.");
        field(r, "Endurance Group Critical Warnings", "CDW11[23:16]", (v >> 16) & 0xffu,
              hex_value((v >> 16) & 0xffu, 2),
              "Each set bit enables an event for the corresponding critical warning in the "
              "Endurance Group Information log page.",
              2);
        field(r, "Reserved", "CDW11[31:24]", v >> 24, std::to_string(v >> 24),
              "Reserved; host shall set to zero.");
        break;
    case 0x19:
        field(r, "I/O Command Set Combination Index", "CDW11[8:0]", v & 0x1ffu,
              std::to_string(v & 0x1ffu),
              "Selects an I/O Command Set combination reported by Identify I/O Command Set data.");
        field(r, "Reserved", "CDW11[31:9]", v >> 9, std::to_string(v >> 9),
              "Reserved; host shall set to zero.");
        break;
    default:
        field(r, "Feature Value", "CDW11[31:0]", v, hex_value(v, 8),
              "Feature-specific value; exact subfields depend on feature ID and are not decoded in "
              "this implementation.",
              8);
        break;
    }
    if (get)
        field_text(r, "Feature Result", "Completion DW0", "Not present in the command",
                   "Returned in completion entry DW0",
                   "The requested feature value is returned in the command completion, not in the "
                   "16-dword submission entry.");
}
} // namespace
DecodedCommand decode_feature_command(const Command& c, bool get) {
    const auto fid = c[10] & 0xffu;
    const auto* name = feature_name(fid);
    DecodedCommand r{
        get ? "Get Features" : "Set Features",
        std::string(get ? "Reads" : "Sets") + " feature " + hex_value(fid, 2) +
            (name ? std::string(" (") + name + ")" : " (feature name not in this catalogue)"),
        {}};
    const bool uses_data_pointer = fid == 0x0c || fid == 0x0d || fid == 0x0e || fid == 0x16 ||
                                   fid == 0x7d || fid == 0x7e || fid == 0x7f || fid == 0x81;
    common_header(r, c, true, false, uses_data_pointer);
    field(r, "Feature Identifier", "CDW10[7:0]", fid,
          name ? std::string(name) : "Unknown feature identifier",
          "Selects the feature whose value is accessed.", 2);
    if (get) {
        const auto sel = (c[10] >> 8) & 7u;
        const std::string selection = sel == 0   ? "0 (current)"
                                      : sel == 1 ? "1 (default)"
                                      : sel == 2 ? "2 (saved)"
                                      : sel == 3 ? "3 (supported capabilities)"
                                                 : "reserved";
        field(r, "Feature Selection", "CDW10[10:8]", sel, selection,
              "Selects current, default, saved, or supported feature value.");
        field(r, "Reserved", "CDW10[31:11]", c[10] >> 11, std::to_string(c[10] >> 11),
              "Reserved; host shall set to zero.");
    } else {
        field(r, "Save", "CDW10[31]", c[10] >> 31,
              (c[10] >> 31) ? "1 (save value)" : "0 (do not save)",
              "Requests that the value be saved across power cycles when the feature supports "
              "saving.");
        field(r, "Reserved", "CDW10[30:8]", (c[10] >> 8) & 0x7fffffu,
              std::to_string((c[10] >> 8) & 0x7fffffu), "Reserved; host shall set to zero.");
    }
    if (get) {
        field(r, "Reserved Command Value", "CDW11[31:0]", c[11], hex_value(c[11], 8),
              "Command Dword 11 is not the returned feature value for this Get Features command.",
              8);
        field_text(r, "Feature Result", "Completion DW0", "Not present in the command",
                   "Returned by the controller",
                   "Get Features returns the current/default/saved/supported feature attributes in "
                   "completion entry DW0; some features return additional data in a data buffer.");
        if (fid == 0x0c || fid == 0x0d || fid == 0x0e || fid == 0x16 || fid == 0x7d ||
            fid == 0x7e || fid == 0x7f || fid == 0x81)
            field_text(r, "Feature Data", "Data buffer", "Not present in command dwords",
                       "Transferred through the Data Pointer",
                       "This feature uses a data buffer for returned attributes.");
    } else {
        value_fields(r, fid, c, false);
    }
    return r;
}
} // namespace nvme_decoder::detail
