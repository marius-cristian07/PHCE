# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.1)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(webserver C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1


add_executable(webserver
    webserver.cpp
    Connection.cpp
)

pico_set_program_name(webserver "webserver")
pico_set_program_version(webserver "0.1")

# Enable USB Serial Monitor
pico_enable_stdio_uart(webserver 0)
pico_enable_stdio_usb(webserver 1)

# Build HTML content into Pico flash
pico_add_library(pico_httpd_content NOFLAG)

pico_set_lwip_httpd_content(pico_httpd_content INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/content/404.html
    ${CMAKE_CURRENT_LIST_DIR}/content/index.shtml
)

# Required libraries
target_link_libraries(webserver
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
    pico_lwip_mdns
    pico_httpd_content
    hardware_adc
)


# Load Pico hotspot credentials
include(${CMAKE_CURRENT_LIST_DIR}/wifi.cmake)

target_compile_definitions(webserver PRIVATE
    AP_SSID=\"${AP_SSID}\"
    AP_PASSWORD=\"${AP_PASSWORD}\"
)

# DHCP server

target_sources(webserver PRIVATE
    ${PICO_SDK_PATH}/lib/btstack/3rd-party/lwip/dhcp-server/dhserver.c
)

target_include_directories(webserver PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${PICO_SDK_PATH}/lib/btstack/3rd-party/lwip/dhcp-server
)


pico_add_extra_outputs(webserver)


