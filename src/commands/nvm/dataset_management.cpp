#include "../../command_decoders.hpp"
#include "../../field_helpers.hpp"

namespace nvme_decoder::detail {
void decode_nvm_fields_dataset_management(DecodedCommand& r, const Command& c, std::uint8_t op) {
    r.command_name = "Dataset Management";
    r.use = "Provides dataset attributes for one or more logical block ranges, including "
            "deallocation requests.";
    field(r, "Number of Ranges", "CDW10[7:0]", c[10] & 0xffu, std::to_string((c[10] & 0xffu) + 1u),
          "Zero-based number of 16-byte range descriptors in the data buffer.");
    field(r, "Dataset Management Attributes", "CDW11[2:0]", c[11] & 7u, hex_value(c[11] & 7u, 1),
          "AD: deallocate; IDW: integral dataset for write; IDR: integral dataset for read.");
    field(r, "Reserved", "CDW11[31:3]", c[11] >> 3, std::to_string(c[11] >> 3),
          "Reserved; host shall set to zero.");
    nvm_reserved_dwords(r, c, 12, 15);
}
} // namespace nvme_decoder::detail
