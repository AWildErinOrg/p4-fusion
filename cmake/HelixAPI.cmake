if (NOT EXISTS "${HELIX_API}/include/p4/clientapi.h")
    message(FATAL_ERROR
        "Helix Core C++ API headers not found in ${HELIX_API}/include/p4. "
        "Extract the SDK there or configure with -DHELIX_API=<SDK directory>.")
endif()

# Retain the existing Unix default, while allowing normal Windows discovery.
if (UNIX AND NOT OPENSSL_ROOT_DIR AND EXISTS /usr/local/ssl)
    set(OPENSSL_ROOT_DIR /usr/local/ssl)
endif()
set(OPENSSL_USE_STATIC_LIBS TRUE)
if (MSVC)
    set(OPENSSL_MSVC_STATIC_RT TRUE)
endif()
find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)

add_library(helix-api INTERFACE)
target_include_directories(helix-api INTERFACE "${HELIX_API}/include")
foreach(component client rpc supp p4script_cstub)
    # Windows SDK names are libclient.lib, librpc.lib, etc.
    find_library(HELIX_${component}_LIBRARY
        NAMES ${component} lib${component}
        PATHS "${HELIX_API}/lib"
        NO_DEFAULT_PATH)
    if (NOT HELIX_${component}_LIBRARY)
        message(FATAL_ERROR "Perforce ${component} library not found in ${HELIX_API}/lib")
    endif()
    target_link_libraries(helix-api INTERFACE "${HELIX_${component}_LIBRARY}")
endforeach()
target_link_libraries(helix-api INTERFACE OpenSSL::SSL OpenSSL::Crypto)

if (WIN32)
    target_compile_definitions(helix-api INTERFACE OS_NT CASE_INSENSITIVE NOMINMAX)
    target_link_libraries(helix-api INTERFACE ws2_32 advapi32 iphlpapi bcrypt crypt32)
endif()
