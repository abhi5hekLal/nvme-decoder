cmake_minimum_required(VERSION 3.20)
project(nvme_decoder VERSION 0.1.0 LANGUAGES CXX)

file(GLOB ADMIN_COMMAND_DECODERS CONFIGURE_DEPENDS src/commands/admin/*.cpp)
file(GLOB NVM_COMMAND_DECODERS CONFIGURE_DEPENDS src/commands/nvm/*.cpp)

add_library(nvme_decoder_lib
    src/decoder.cpp
    src/parser.cpp
    src/admin_commands.cpp
    ${ADMIN_COMMAND_DECODERS}
    ${NVM_COMMAND_DECODERS}
    src/nvm_commands.cpp
    src/features.cpp
)
target_include_directories(nvme_decoder_lib PUBLIC include)
target_compile_features(nvme_decoder_lib PUBLIC cxx_std_20)

add_executable(nvme-decoder src/main.cpp)
target_link_libraries(nvme-decoder PRIVATE nvme_decoder_lib)

if(MSVC)
    target_compile_options(nvme_decoder_lib PRIVATE /W4 /permissive-)
    target_compile_options(nvme-decoder PRIVATE /W4 /permissive-)
else()
    target_compile_options(nvme_decoder_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(nvme-decoder PRIVATE -Wall -Wextra -Wpedantic)
endif()
