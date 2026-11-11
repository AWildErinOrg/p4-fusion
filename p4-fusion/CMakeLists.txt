file(GLOB_RECURSE Headers **.h)
file(GLOB_RECURSE Sources **.cc)

add_executable(p4-fusion ${Headers} ${Sources})

target_include_directories(p4-fusion PUBLIC
    ../vendor/libgit2/include/
    ../vendor/minitrace/
    ${CMAKE_CURRENT_LIST_DIR}
)

set(Frameworks "")

if (APPLE)
    find_library(COREFOUNDATION_LIB CoreFoundation REQUIRED)
    find_library(CFNETWORK_LIB CFNetwork REQUIRED)
    find_library(COCOA_LIB Cocoa REQUIRED)
    find_library(SECURITY_LIB Security REQUIRED)
    set(Frameworks
        ${Frameworks}
        ${CFNETWORK_LIB}
        ${COREFOUNDATION_LIB}
        ${COCOA_LIB}
        ${SECURITY_LIB}
    )
endif(APPLE)

target_link_libraries(p4-fusion PUBLIC
    ${Frameworks}
    helix-api
    git2
    minitrace
)
