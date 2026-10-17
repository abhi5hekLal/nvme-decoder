cmake_minimum_required(VERSION 3.20)
project(nvme_decoder VERSION 0.1.0 LANGUAGES CXX)

option(NVME_DECODER_BUILD_UI "Build the Qt Quick desktop interface when Qt is available" ON)

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

if(NVME_DECODER_BUILD_UI)
    find_package(Qt6 6.2 QUIET COMPONENTS Quick Qml QuickControls2)

    if(Qt6_FOUND)
        qt_add_executable(nvme-decoder-ui
            app/main.cpp
            app/decoder_controller.cpp
            app/decoder_controller.hpp
        )
        set_target_properties(nvme-decoder-ui PROPERTIES AUTOMOC ON)

        set_source_files_properties(app/qml/Main.qml PROPERTIES QT_RESOURCE_ALIAS Main.qml)

        qt_add_qml_module(nvme-decoder-ui
            URI NVMeDecoder
            VERSION 1.0
            RESOURCE_PREFIX /qt/qml
            QML_FILES app/qml/Main.qml
        )

        target_link_libraries(nvme-decoder-ui PRIVATE
            nvme_decoder_lib
            Qt6::Quick
            Qt6::Qml
            Qt6::QuickControls2
        )
    else()
        message(STATUS "Qt 6 Quick was not found; building the decoder library and CLI only")
    endif()
endif()

if(MSVC)
    target_compile_options(nvme_decoder_lib PRIVATE /W4 /permissive-)
    target_compile_options(nvme-decoder PRIVATE /W4 /permissive-)
    if(TARGET nvme-decoder-ui)
        target_compile_options(nvme-decoder-ui PRIVATE /W4 /permissive-)
    endif()
else()
    target_compile_options(nvme_decoder_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(nvme-decoder PRIVATE -Wall -Wextra -Wpedantic)
    if(TARGET nvme-decoder-ui)
        target_compile_options(nvme-decoder-ui PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
