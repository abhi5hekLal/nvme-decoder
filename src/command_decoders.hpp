#pragma once
#include "nvme_decoder/command.hpp"
#include <cstdint>
namespace nvme_decoder::detail {
DecodedCommand decode_admin(const Command& command);
DecodedCommand decode_nvm(const Command& command);
DecodedCommand decode_set_feature(const Command& command);
DecodedCommand decode_get_feature(const Command& command);
DecodedCommand decode_feature_command(const Command& command, bool get);
DecodedCommand decode_firmware_commit(const Command& command);
const char* identify_name(std::uint32_t cns);
const char* log_name(std::uint32_t lid);
void reserved_dwords(DecodedCommand& out, const Command& command, unsigned first, unsigned last);
void queue_fields(DecodedCommand& out, const Command& command, bool create, bool completion);
void nvm_slba(DecodedCommand& out, const Command& command);
void nvm_nlb(DecodedCommand& out, const Command& command);
void nvm_reserved_dwords(DecodedCommand& out, const Command& command, unsigned first, unsigned last,
                         const char* reason = "Reserved; host shall set to zero.");
void nvm_rw_fields(DecodedCommand& out, const Command& command, bool write);
void nvm_verify_compare_fields(DecodedCommand& out, const Command& command, bool compare);
void decode_nvm_fields_flush(DecodedCommand& out, const Command& command, std::uint8_t opcode);
void decode_nvm_fields_write(DecodedCommand& out, const Command& command, std::uint8_t opcode);
void decode_nvm_fields_read(DecodedCommand& out, const Command& command, std::uint8_t opcode);
void decode_nvm_fields_write_uncorrectable(DecodedCommand& out, const Command& command,
                                           std::uint8_t opcode);
void decode_nvm_fields_compare(DecodedCommand& out, const Command& command, std::uint8_t opcode);
void decode_nvm_fields_write_zeroes(DecodedCommand& out, const Command& command,
                                    std::uint8_t opcode);
void decode_nvm_fields_dataset_management(DecodedCommand& out, const Command& command,
                                          std::uint8_t opcode);
void decode_nvm_fields_verify(DecodedCommand& out, const Command& command, std::uint8_t opcode);
void decode_nvm_fields_reservation_register(DecodedCommand& out, const Command& command,
                                            std::uint8_t opcode);
void decode_nvm_fields_reservation_report(DecodedCommand& out, const Command& command,
                                          std::uint8_t opcode);
void decode_nvm_fields_reservation_acquire(DecodedCommand& out, const Command& command,
                                           std::uint8_t opcode);
void decode_nvm_fields_io_management_receive(DecodedCommand& out, const Command& command,
                                             std::uint8_t opcode);
void decode_nvm_fields_reservation_release(DecodedCommand& out, const Command& command,
                                           std::uint8_t opcode);
void decode_nvm_fields_cancel(DecodedCommand& out, const Command& command, std::uint8_t opcode);
void decode_nvm_fields_copy(DecodedCommand& out, const Command& command, std::uint8_t opcode);
void decode_nvm_fields_io_management_send(DecodedCommand& out, const Command& command,
                                          std::uint8_t opcode);
void decode_admin_fields_delete_io_submission_queue(DecodedCommand& out, const Command& command,
                                                    std::uint8_t opcode);
void decode_admin_fields_create_io_submission_queue(DecodedCommand& out, const Command& command,
                                                    std::uint8_t opcode);
void decode_admin_fields_get_log_page(DecodedCommand& out, const Command& command,
                                      std::uint8_t opcode);
void decode_admin_fields_delete_io_completion_queue(DecodedCommand& out, const Command& command,
                                                    std::uint8_t opcode);
void decode_admin_fields_create_io_completion_queue(DecodedCommand& out, const Command& command,
                                                    std::uint8_t opcode);
void decode_admin_fields_identify(DecodedCommand& out, const Command& command, std::uint8_t opcode);
void decode_admin_fields_abort(DecodedCommand& out, const Command& command, std::uint8_t opcode);
void decode_admin_fields_asynchronous_event_request(DecodedCommand& out, const Command& command,
                                                    std::uint8_t opcode);
void decode_admin_fields_namespace_management(DecodedCommand& out, const Command& command,
                                              std::uint8_t opcode);
void decode_admin_fields_firmware_image_download(DecodedCommand& out, const Command& command,
                                                 std::uint8_t opcode);
void decode_admin_fields_device_self_test(DecodedCommand& out, const Command& command,
                                          std::uint8_t opcode);
void decode_admin_fields_namespace_attachment(DecodedCommand& out, const Command& command,
                                              std::uint8_t opcode);
void decode_admin_fields_keep_alive(DecodedCommand& out, const Command& command,
                                    std::uint8_t opcode);
void decode_admin_fields_directive_send(DecodedCommand& out, const Command& command,
                                        std::uint8_t opcode);
void decode_admin_fields_directive_receive(DecodedCommand& out, const Command& command,
                                           std::uint8_t opcode);
void decode_admin_fields_virtualization_management(DecodedCommand& out, const Command& command,
                                                   std::uint8_t opcode);
void decode_admin_fields_nvme_mi_send(DecodedCommand& out, const Command& command,
                                      std::uint8_t opcode);
void decode_admin_fields_nvme_mi_receive(DecodedCommand& out, const Command& command,
                                         std::uint8_t opcode);
void decode_admin_fields_capacity_management(DecodedCommand& out, const Command& command,
                                             std::uint8_t opcode);
void decode_admin_fields_lockdown(DecodedCommand& out, const Command& command, std::uint8_t opcode);
void decode_admin_fields_doorbell_buffer_config(DecodedCommand& out, const Command& command,
                                                std::uint8_t opcode);
void decode_admin_fields_format_nvm(DecodedCommand& out, const Command& command,
                                    std::uint8_t opcode);
void decode_admin_fields_security_send(DecodedCommand& out, const Command& command,
                                       std::uint8_t opcode);
void decode_admin_fields_security_receive(DecodedCommand& out, const Command& command,
                                          std::uint8_t opcode);
void decode_admin_fields_sanitize(DecodedCommand& out, const Command& command, std::uint8_t opcode);
} // namespace nvme_decoder::detail
