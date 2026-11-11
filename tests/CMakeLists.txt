add_executable(p4-fusion-test 
    main.cc

    ../p4-fusion/utils/std_helpers.cc
    ../p4-fusion/utils/time_helpers.cc
    ../p4-fusion/git_api.cc
    ../p4-fusion/log.cc
)

target_include_directories(p4-fusion-test PRIVATE
    ../p4-fusion/
    ../vendor/libgit2/include/
    ../vendor/minitrace/
)

target_link_libraries(p4-fusion-test PRIVATE
    helix-api
    git2
    minitrace
)

add_test(NAME p4-fusion-test COMMAND p4-fusion-test)
