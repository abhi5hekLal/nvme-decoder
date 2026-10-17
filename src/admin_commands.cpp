#include "command_decoders.hpp"
#include "field_helpers.hpp"
#include <array>
#include <cstdint>
#include <string>

namespace nvme_decoder::detail {
constexpr std::array<std::uint8_t, 28> admin_opcodes{
    0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0a, 0x0c, 0x0d, 0x10, 0x11, 0x14,
    0x15, 0x18, 0x19, 0x1a, 0x1c, 0x1d, 0x1e, 0x20, 0x24, 0x7c, 0x80, 0x81, 0x82, 0x84};
bool is_admin_opcode(std::uint8_t opcode) {
    for (auto value : admin_opcodes)
        if (value == opcode)
            return true;
    return false;
}
const char* identify_name(std::uint32_t cns) {
    switch (cns) {
    case 0x00:
        return "Identify Namespace";
    case 0x01:
        return "Identify Controller";
    case 0x02:
        return "Active Namespace ID List";
    case 0x03:
        return "Namespace Identification Descriptor List";
    case 0x04:
        return "NVM Set List";
    case 0x05:
        return "I/O Command Set Specific Identify Namespace";
    case 0x06:
        return "I/O Command Set Specific Identify Controller";
    case 0x07:
        return "I/O Command Set Specific Active Namespace ID List";
    case 0x08:
        return "I/O Command Set Independent Identify Namespace";
    case 0x09:
        return "NVM Namespace Capabilities for Format";
    case 0x0a:
        return "I/O Command Set Specific Namespace Capabilities for Format";
    case 0x10:
        return "Allocated Namespace ID List";
    case 0x11:
        return "Allocated Namespace Identify";
    case 0x12:
        return "Attached Controller List";
    case 0x13:
        return "Controller List";
    case 0x14:
        return "Primary Controller Capabilities";
    case 0x15:
        return "Secondary Controller List";
    case 0x16:
        return "Namespace Granularity List";
    case 0x17:
        return "UUID List";
    case 0x18:
        return "Domain List";
    case 0x19:
        return "Domain Identifier";
    case 0x1a:
        return "NVM Set List";
    case 0x1b:
        return "Endurance Group List";
    case 0x1c:
        return "I/O Command Set Independent Identify Controller";
    case 0x1d:
        return "I/O Command Set Specific Identify Controller";
    default:
        return "Reserved or command-set-specific CNS";
    }
}
const char* log_name(std::uint32_t lid) {
    switch (lid) {
    case 0x01:
        return "Error Information";
    case 0x02:
        return "SMART / Health Information";
    case 0x03:
        return "Firmware Slot Information";
    case 0x04:
        return "Changed Namespace List";
    case 0x05:
        return "Commands Supported and Effects";
    case 0x06:
        return "Device Self-test";
    case 0x07:
        return "Telemetry Host-Initiated";
    case 0x08:
        return "Telemetry Controller-Initiated";
    case 0x09:
        return "Endurance Group Information";
    case 0x0a:
        return "Predictable Latency Per NVM Set";
    case 0x0b:
        return "Predictable Latency Event Aggregate";
    case 0x0c:
        return "Asymmetric Namespace Access";
    case 0x0d:
        return "Persistent Event Log";
    case 0x0e:
        return "LBA Status Information";
    case 0x0f:
        return "Endurance Group Event Aggregate";
    case 0x10:
        return "Media Unit Status";
    case 0x11:
        return "Supported Capacity Configuration";
    default:
        return nullptr;
    }
}
void reserved_dwords(DecodedCommand& r, const Command& c, unsigned first, unsigned last) {
    if (first > last)
        return;
    const auto location = first == last
                              ? "CDW" + std::to_string(first)
                              : "CDW" + std::to_string(first) + "-CDW" + std::to_string(last);
    field_text(r, "Reserved command dwords", location, reserved_dwords_raw(c, first, last),
               reserved_dwords_decoded(c, first, last),
               "Reserved for this command; host shall set to zero.");
}
void queue_fields(DecodedCommand& r, const Command& c, bool create, bool completion) {
    if (create) {
        const auto psdt = (c[0] >> 14) & 3u;
        if (psdt != 0)
            field(r, "Reserved Pointer Type (PSDT)", "CDW0[15:14]", psdt, std::to_string(psdt),
                  "Create I/O Submission/Completion Queue uses PRP1; PSDT shall select PRP (00b). "
                  "A non-zero value makes the command invalid.");
        field(r, "Queue Identifier", "CDW10[15:0]", c[10] & 0xffffu,
              std::to_string(c[10] & 0xffffu), "Host-assigned identifier of the I/O queue.");
        const auto encoded = (c[10] >> 16) & 0xffffu;
        field(r, "Queue Size", "CDW10[31:16]", encoded,
              std::to_string(encoded) + " encoded; " + std::to_string(encoded + 1u) + " entries",
              "Zero-based queue size: the queue contains this value plus one entries.");
        field(r, "Physically Contiguous", "CDW11[0]", c[11] & 1u,
              (c[11] & 1u) ? "1 (physically contiguous)" : "0 (not physically contiguous)",
              "Selects whether PRP1 points to a physically contiguous queue buffer or a PRP list.");
        if (completion) {
            field(r, "Interrupts Enabled", "CDW11[1]", (c[11] >> 1) & 1u,
                  ((c[11] >> 1) & 1u) ? "1 (enabled)" : "0 (disabled)",
                  "Enables interrupt notification for this completion queue.");
            field(r, "Interrupt Vector", "CDW11[31:16]", c[11] >> 16, std::to_string(c[11] >> 16),
                  "Interrupt vector associated with the completion queue; used when interrupts are "
                  "enabled.");
            field(r, "Reserved", "CDW11[15:2]", (c[11] >> 2) & 0x3fffu,
                  std::to_string((c[11] >> 2) & 0x3fffu), "Reserved; host shall set to zero.");
            const auto prp1 = (static_cast<std::uint64_t>(c[7]) << 32) | c[6];
            field(r, "Queue Base Address (PRP1)", "CDW6:CDW7[63:0]", prp1, hex_value(prp1, 16),
                  "PRP1 points to the completion queue base address when PC is set; otherwise PRP1 "
                  "points to a PRP list for the queue memory.",
                  16);
            if (c[8] || c[9])
                field_text(r, "Reserved PRP Entry 2", "CDW8:CDW9[63:0]",
                           hex_value((static_cast<std::uint64_t>(c[9]) << 32) | c[8], 16),
                           "Non-zero; command is invalid",
                           "PRP Entry 2 is reserved and shall be cleared for Create I/O Completion "
                           "Queue.");
            reserved_dwords(r, c, 12, 15);
        } else {
            const auto prio = (c[11] >> 1) & 3u;
            field(
                r, "Queue Priority", "CDW11[2:1]", prio,
                prio == 0   ? "0 (urgent)"
                : prio == 1 ? "1 (high)"
                : prio == 2 ? "2 (medium)"
                            : "3 (low)",
                "Selects the submission queue priority class when weighted-round-robin arbitration "
                "with urgent priority is selected; ignored for other arbitration mechanisms.");
            field(r, "Reserved", "CDW11[15:3]", (c[11] >> 3) & 0x1fffu,
                  std::to_string((c[11] >> 3) & 0x1fffu), "Reserved; host shall set to zero.");
            field(r, "Completion Queue Identifier", "CDW11[31:16]", c[11] >> 16,
                  std::to_string(c[11] >> 16),
                  "Completion queue associated with this submission queue; I/O CQ ID 0 (Admin CQ) "
                  "is not valid here.");
            const auto prp1 = (static_cast<std::uint64_t>(c[7]) << 32) | c[6];
            field(r, "Queue Base Address (PRP1)", "CDW6:CDW7[63:0]", prp1, hex_value(prp1, 16),
                  "PRP1 points to the submission queue base address when PC is set; otherwise PRP1 "
                  "points to a PRP list for the queue memory.",
                  16);
            if (c[8] || c[9])
                field_text(r, "Reserved PRP Entry 2", "CDW8:CDW9[63:0]",
                           hex_value((static_cast<std::uint64_t>(c[9]) << 32) | c[8], 16),
                           "Non-zero; command is invalid",
                           "PRP Entry 2 is reserved and shall be cleared for Create I/O Submission "
                           "Queue.");
            reserved_dwords(r, c, 12, 15);
        }
    } else {
        field(r, "Queue Identifier", "CDW10[15:0]", c[10] & 0xffffu,
              std::to_string(c[10] & 0xffffu), "Identifier of the I/O queue to delete.");
        field(r, "Reserved", "CDW10[31:16]", c[10] >> 16, std::to_string(c[10] >> 16),
              "Reserved; host shall set to zero.");
        reserved_dwords(r, c, 11, 15);
    }
}
DecodedCommand decode_admin(const Command& c) {
    const auto op = static_cast<std::uint8_t>(c[0]);
    if (op == 0x10)
        return decode_firmware_commit(c);
    if (!is_admin_opcode(op))
        return {"Unknown Admin command",
                "Opcode " + hex_value(op, 2) + " is not a decoded standard Admin command.",
                {}};
    DecodedCommand r{};
    const bool uses_data_pointer = op == 0x02 || op == 0x06 || (op == 0x0d && (c[10] & 3u) == 0) ||
                                   op == 0x11 || (op == 0x15 && (c[10] & 3u) <= 1) || op == 0x19 ||
                                   op == 0x1a || op == 0x1d || op == 0x1e || op == 0x7c ||
                                   op == 0x81 || op == 0x82;
    common_header(r, c, op == 2 || op == 6 || op == 0x0d || op == 0x15 || op == 0x80, false,
                  uses_data_pointer);
    switch (op) {
    case 0x00:
        decode_admin_fields_delete_io_submission_queue(r, c, op);
        break;
    case 0x01:
        decode_admin_fields_create_io_submission_queue(r, c, op);
        break;
    case 0x02:
        decode_admin_fields_get_log_page(r, c, op);
        break;
    case 0x04:
        decode_admin_fields_delete_io_completion_queue(r, c, op);
        break;
    case 0x05:
        decode_admin_fields_create_io_completion_queue(r, c, op);
        break;
    case 0x06:
        decode_admin_fields_identify(r, c, op);
        break;
    case 0x08:
        decode_admin_fields_abort(r, c, op);
        break;
    case 0x0c:
        decode_admin_fields_asynchronous_event_request(r, c, op);
        break;
    case 0x0d:
        decode_admin_fields_namespace_management(r, c, op);
        break;
    case 0x11:
        decode_admin_fields_firmware_image_download(r, c, op);
        break;
    case 0x14:
        decode_admin_fields_device_self_test(r, c, op);
        break;
    case 0x15:
        decode_admin_fields_namespace_attachment(r, c, op);
        break;
    case 0x18:
        decode_admin_fields_keep_alive(r, c, op);
        break;
    case 0x19:
        decode_admin_fields_directive_send(r, c, op);
        break;
    case 0x1a:
        decode_admin_fields_directive_receive(r, c, op);
        break;
    case 0x1c:
        decode_admin_fields_virtualization_management(r, c, op);
        break;
    case 0x1d:
        decode_admin_fields_nvme_mi_send(r, c, op);
        break;
    case 0x1e:
        decode_admin_fields_nvme_mi_receive(r, c, op);
        break;
    case 0x20:
        decode_admin_fields_capacity_management(r, c, op);
        break;
    case 0x24:
        decode_admin_fields_lockdown(r, c, op);
        break;
    case 0x7c:
        decode_admin_fields_doorbell_buffer_config(r, c, op);
        break;
    case 0x80:
        decode_admin_fields_format_nvm(r, c, op);
        break;
    case 0x81:
        decode_admin_fields_security_send(r, c, op);
        break;
    case 0x82:
        decode_admin_fields_security_receive(r, c, op);
        break;
    case 0x84:
        decode_admin_fields_sanitize(r, c, op);
        break;
    case 0x09:
        return decode_set_feature(c);
    case 0x0a:
        return decode_get_feature(c);
    default:
        break;
    }
    return r;
}
} // namespace nvme_decoder::detail
