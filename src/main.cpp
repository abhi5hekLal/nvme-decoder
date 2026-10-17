#include "nvme_decoder/decoder.hpp"
#include "nvme_decoder/parser.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
        std::cout << "Usage: nvme-decoder [--set admin|nvm] [16 hexadecimal dwords]\n"
                     "Without dwords, prompts for the command set and all 16 dwords.\n"
                     "The opcode is the low byte of CDW0.\n";
        return 0;
    }
    std::string input;
    nvme_decoder::CommandSet command_set = nvme_decoder::CommandSet::admin;
    int first_value = 1;
    if (argc > 2 && std::string(argv[1]) == "--set") {
        const std::string name = argv[2];
        if (name == "admin")
            command_set = nvme_decoder::CommandSet::admin;
        else if (name == "nvm")
            command_set = nvme_decoder::CommandSet::nvm;
        else {
            std::cerr << "Command set must be 'admin' or 'nvm'.\n";
            return 2;
        }
        first_value = 3;
    }
    if (argc > first_value) {
        for (int i = first_value; i < argc; ++i) {
            if (!input.empty())
                input += ' ';
            input += argv[i];
        }
    } else {
        std::cout << "Command set (admin/nvm): ";
        std::string set_name;
        if (!std::getline(std::cin, set_name))
            return 1;
        if (set_name == "nvm")
            command_set = nvme_decoder::CommandSet::nvm;
        else if (set_name != "admin") {
            std::cerr << "Command set must be 'admin' or 'nvm'.\n";
            return 2;
        }
        std::cout << "Enter 16 hexadecimal dwords (CDW0 through CDW15):\n> ";
        if (!std::getline(std::cin, input))
            return 1;
    }

    try {
        const auto decoded = nvme_decoder::decode(nvme_decoder::parse_command(input), command_set);
        std::cout << "Command: " << decoded.command_name << "\n"
                  << "Use: " << decoded.use << "\n\n"
                  << "Details on the command follow:\n";
        for (const auto& field : decoded.fields) {
            std::cout << "\n"
                      << field.location << " -> " << field.name << ": " << field.decoded_value
                      << "\n"
                      << "  Meaning: " << field.meaning << '\n';
        }
    } catch (const nvme_decoder::InputError& error) {
        std::cerr << "Input error: " << error.what() << '\n';
        return 2;
    }
    return 0;
}
