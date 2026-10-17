#include "command_decoders.hpp"
#include "field_helpers.hpp"
#include <array>
#include <string>
namespace nvme_decoder::detail {
constexpr std::array<std::uint8_t, 16> nvm_opcodes{0x00, 0x01, 0x02, 0x04, 0x05, 0x08, 0x09, 0x0c,
                                                   0x0d, 0x0e, 0x11, 0x12, 0x15, 0x18, 0x19, 0x1d};
bool is_nvm_opcode(std::uint8_t opcode) {
    for (auto value : nvm_opcodes)
        if (value == opcode)
            return true;
    return false;
}
void nvm_slba(DecodedCommand& r, const Command& c) {
    const auto v = (static_cast<std::uint64_t>(c[11]) << 32) | c[10];
    field(r, "Starting LBA", "CDW10:CDW11[63:0]", v, std::to_string(v),
          "Starting logical block address for the command.", 16);
}
void nvm_nlb(DecodedCommand& r, const Command& c) {
    const auto raw = c[12] & 0xffffu;
    field(r, "Number of Logical Blocks", "CDW12[15:0]", raw, std::to_string(raw + 1u),
          "Zero-based transfer length: the command covers NLB + 1 logical blocks.");
}
void nvm_reserved_dwords(DecodedCommand& r, const Command& c, unsigned first, unsigned last,
                         const char* reason) {
    if (first > last)
        return;
    const auto location = first == last
                              ? "CDW" + std::to_string(first)
                              : "CDW" + std::to_string(first) + "-CDW" + std::to_string(last);
    field_text(r, "Reserved command dwords", location, reserved_dwords_raw(c, first, last),
               reserved_dwords_decoded(c, first, last), reason);
}
void nvm_rw_fields(DecodedCommand& r, const Command& c, bool write) {
    field(r, "Limited Retry", "CDW12[31]", c[12] >> 31, std::to_string(c[12] >> 31),
          "Requests limited retry effort when set; applies to error recovery for this read/write "
          "operation.");
    field(r, "Force Unit Access", "CDW12[30]", (c[12] >> 30) & 1u,
          std::to_string((c[12] >> 30) & 1u),
          write ? "When set, data and metadata for this write are committed to non-volatile media "
                  "before completion."
                : "When set, data and metadata are committed to non-volatile media and returned "
                  "from non-volatile media; no ordering with other commands is implied.");
    field(r, "Protection Information Action and Check", "CDW12[29:26]", (c[12] >> 26) & 0xfu,
          hex_value((c[12] >> 26) & 0xfu, 1),
          "PRINFO: bit 29 is PRACT; bits 28:26 are PRCHK (guard, application-tag, and "
          "reference-tag checks).");
    field(r, "Protection Information Action", "CDW12[29]", (c[12] >> 29) & 1u,
          std::to_string((c[12] >> 29) & 1u),
          "PRACT controls whether the controller inserts or removes protection information "
          "according to the namespace format.");
    field(r, "Protection Information Checks", "CDW12[28:26]", (c[12] >> 26) & 7u,
          hex_value((c[12] >> 26) & 7u, 1),
          "PRCHK bits select guard, application-tag, and reference-tag checks.");
    field(r, "Guard Check", "CDW12[28]", (c[12] >> 28) & 1u,
          ((c[12] >> 28) & 1u) ? "1 (check guard)" : "0 (do not check)",
          "PRCHK bit 2 requests a guard field check.");
    field(r, "Application Tag Check", "CDW12[27]", (c[12] >> 27) & 1u,
          ((c[12] >> 27) & 1u) ? "1 (check application tag)" : "0 (do not check)",
          "PRCHK bit 1 requests an application tag check.");
    field(r, "Reference Tag Check", "CDW12[26]", (c[12] >> 26) & 1u,
          ((c[12] >> 26) & 1u) ? "1 (check reference tag)" : "0 (do not check)",
          "PRCHK bit 0 requests a reference tag check.");
    field(r, "Reserved", "CDW12[25]", (c[12] >> 25) & 1u, std::to_string((c[12] >> 25) & 1u),
          "Reserved; host shall set to zero.");
    field(r, "Storage Tag Check", "CDW12[24]", (c[12] >> 24) & 1u,
          std::to_string((c[12] >> 24) & 1u),
          "Requests storage-tag checking as part of end-to-end protection processing.");
    field(r, "Directive Type", "CDW12[23:20]", (c[12] >> 20) & 0xfu,
          std::to_string((c[12] >> 20) & 0xfu),
          "Selects the directive type associated with the directive-specific field.");
    const auto cetype = (c[12] >> 16) & 0xfu;
    field(
        r, "Command Extension Type", "CDW12[19:16]", cetype, std::to_string(cetype),
        "Selects the command extension interpretation for CDW13; zero means no command extension.");
    field(r, "Directive Specific", "CDW13[31:16]", c[13] >> 16, std::to_string(c[13] >> 16),
          "Directive-specific value associated with DTYPE.");
    if (cetype == 0)
        field(r, "Reserved", "CDW13[15:0]", c[13] & 0xffffu, std::to_string(c[13] & 0xffffu),
              "Reserved when CETYPE is zero; host shall set to zero.");
    else
        field(r, "Command Extension Value", "CDW13[15:0]", c[13] & 0xffffu,
              hex_value(c[13] & 0xffffu, 4),
              "Command-extension value interpreted according to CETYPE.");
    const auto upper = (static_cast<std::uint64_t>(c[3] & 0xffffu) << 32) | c[2];
    field(r, "Logical Block Tags Upper", "CDW2:CDW3[47:0]", upper, hex_value(upper, 12),
          "Upper portion of the variable-size logical block storage/reference tag; used with CDW14 "
          "when protection information applies.",
          12);
    field(
        r, write ? "Logical Block Tags Lower" : "Expected Logical Block Tags Lower", "CDW14[31:0]",
        c[14], hex_value(c[14], 8),
        "Lower portion of the logical block storage/reference tag (write) or expected tag (read).",
        8);
    field(r, write ? "Logical Block Application Tag" : "Expected Logical Block Application Tag",
          "CDW15[15:0]", c[15] & 0xffffu, hex_value(c[15] & 0xffffu, 4),
          "Application tag written for a write or expected during a read.", 4);
    field(r,
          write ? "Logical Block Application Tag Mask"
                : "Expected Logical Block Application Tag Mask",
          "CDW15[31:16]", c[15] >> 16, hex_value(c[15] >> 16, 4),
          "Application-tag mask used with the application tag.", 4);
}
void nvm_verify_compare_fields(DecodedCommand& r, const Command& c, bool compare) {
    field(r, "Limited Retry", "CDW12[31]", c[12] >> 31, std::to_string(c[12] >> 31),
          "Requests limited retry effort for error recovery.");
    field(r, "Force Unit Access", "CDW12[30]", (c[12] >> 30) & 1u,
          std::to_string((c[12] >> 30) & 1u),
          compare ? "When set, commits associated data and metadata to non-volatile media and "
                    "reads them from non-volatile media for comparison."
                  : "When set, flushes specified data/metadata from volatile cache and verifies "
                    "data committed to non-volatile media.");
    field(r, "Protection Information Action", "CDW12[29]", (c[12] >> 29) & 1u,
          std::to_string((c[12] >> 29) & 1u),
          "PRACT is required to be cleared for Verify and Compare; setting it makes the command "
          "invalid.");
    field(r, "Protection Information Checks", "CDW12[28:26]", (c[12] >> 26) & 7u,
          hex_value((c[12] >> 26) & 7u, 1),
          "PRCHK selects the protection-information checks requested for this operation.");
    field(r, "Guard Check", "CDW12[28]", (c[12] >> 28) & 1u,
          ((c[12] >> 28) & 1u) ? "1 (check guard)" : "0 (do not check)",
          "PRCHK bit 2 requests a guard field check.");
    field(r, "Application Tag Check", "CDW12[27]", (c[12] >> 27) & 1u,
          ((c[12] >> 27) & 1u) ? "1 (check application tag)" : "0 (do not check)",
          "PRCHK bit 1 requests an application tag check.");
    field(r, "Reference Tag Check", "CDW12[26]", (c[12] >> 26) & 1u,
          ((c[12] >> 26) & 1u) ? "1 (check reference tag)" : "0 (do not check)",
          "PRCHK bit 0 requests a reference tag check.");
    field(r, "Reserved", "CDW12[25]", (c[12] >> 25) & 1u, std::to_string((c[12] >> 25) & 1u),
          "Reserved; host shall set to zero.");
    field(r, "Storage Tag Check", "CDW12[24]", (c[12] >> 24) & 1u,
          std::to_string((c[12] >> 24) & 1u),
          "Requests storage-tag checking as part of end-to-end protection processing.");
    if (compare) {
        field(r, "Reserved", "CDW12[23:20]", (c[12] >> 20) & 0xfu,
              std::to_string((c[12] >> 20) & 0xfu), "Reserved for the Compare command.");
        const auto cetype = (c[12] >> 16) & 0xfu;
        field(r, "Command Extension Type", "CDW12[19:16]", cetype, std::to_string(cetype),
              "Selects command-extension interpretation for CDW13; zero means no extension.");
        if (cetype == 0)
            field(r, "Reserved", "CDW13[15:0]", c[13] & 0xffffu, std::to_string(c[13] & 0xffffu),
                  "Reserved when CETYPE is zero; host shall set to zero.");
        else
            field(r, "Command Extension Value", "CDW13[15:0]", c[13] & 0xffffu,
                  hex_value(c[13] & 0xffffu, 4),
                  "Command-extension value interpreted according to CETYPE.");
        field(r, "Reserved", "CDW13[31:16]", c[13] >> 16, std::to_string(c[13] >> 16),
              "Reserved for Compare command extension value.");
    } else
        field(r, "Reserved", "CDW12[23:16]", (c[12] >> 16) & 0xffu,
              std::to_string((c[12] >> 16) & 0xffu), "Reserved for Verify.");
    const auto upper = (static_cast<std::uint64_t>(c[3] & 0xffffu) << 32) | c[2];
    field(r, "Expected Logical Block Tags Upper", "CDW2:CDW3[47:0]", upper, hex_value(upper, 12),
          "Upper portion of the variable-sized expected Logical Block Storage Tag and Expected "
          "Initial Logical Block Reference Tag fields. Interpretation depends on the namespace "
          "protection-information format.",
          12);
    field(r, "Reserved", "CDW2:CDW3[63:48]", c[3] >> 16, std::to_string(c[3] >> 16),
          "Reserved; host shall set to zero.");
    field(r, "Expected Logical Block Tags Lower", "CDW14[31:0]", c[14], hex_value(c[14], 8),
          "Lower portion of the variable-sized expected storage/reference tag fields. "
          "Interpretation depends on the namespace protection-information format.",
          8);
    field(r, "Expected Logical Block Application Tag", "CDW15[15:0]", c[15] & 0xffffu,
          hex_value(c[15] & 0xffffu, 4), "Application tag expected for the logical blocks.", 4);
    field(r, "Expected Logical Block Application Tag Mask", "CDW15[31:16]", c[15] >> 16,
          hex_value(c[15] >> 16, 4), "Mask applied when checking the expected application tag.", 4);
}
DecodedCommand decode_nvm(const Command& c) {
    const auto op = static_cast<std::uint8_t>(c[0]);
    if (!is_nvm_opcode(op))
        return {"Unknown NVM command",
                "Opcode " + hex_value(op, 2) + " is not a decoded standard NVM command.",
                {}};
    DecodedCommand r{};
    const bool uses_command_dwords_2_3 =
        op == 0x01 || op == 0x02 || op == 0x05 || op == 0x0c || op == 0x08 || op == 0x19;
    const bool uses_data_pointer = op == 0x01 || op == 0x02 || op == 0x05 || op == 0x09 ||
                                   op == 0x0d || op == 0x0e || op == 0x11 || op == 0x12 ||
                                   op == 0x15 || op == 0x19 || op == 0x1d;
    const bool uses_metadata_pointer = op == 0x01 || op == 0x02 || op == 0x05;
    common_header(r, c, true, uses_command_dwords_2_3, uses_data_pointer, uses_metadata_pointer);
    switch (op) {
    case 0x00:
        decode_nvm_fields_flush(r, c, op);
        break;
    case 0x01:
        decode_nvm_fields_write(r, c, op);
        break;
    case 0x02:
        decode_nvm_fields_read(r, c, op);
        break;
    case 0x04:
        decode_nvm_fields_write_uncorrectable(r, c, op);
        break;
    case 0x05:
        decode_nvm_fields_compare(r, c, op);
        break;
    case 0x08:
        decode_nvm_fields_write_zeroes(r, c, op);
        break;
    case 0x09:
        decode_nvm_fields_dataset_management(r, c, op);
        break;
    case 0x0c:
        decode_nvm_fields_verify(r, c, op);
        break;
    case 0x0d:
        decode_nvm_fields_reservation_register(r, c, op);
        break;
    case 0x0e:
        decode_nvm_fields_reservation_report(r, c, op);
        break;
    case 0x11:
        decode_nvm_fields_reservation_acquire(r, c, op);
        break;
    case 0x12:
        decode_nvm_fields_io_management_receive(r, c, op);
        break;
    case 0x15:
        decode_nvm_fields_reservation_release(r, c, op);
        break;
    case 0x18:
        decode_nvm_fields_cancel(r, c, op);
        break;
    case 0x19:
        decode_nvm_fields_copy(r, c, op);
        break;
    case 0x1d:
        decode_nvm_fields_io_management_send(r, c, op);
        break;
    default:
        break;
    }
    return r;
}
} // namespace nvme_decoder::detail
