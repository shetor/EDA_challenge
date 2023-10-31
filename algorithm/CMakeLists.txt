include_directories(commands)

target_link_libraries(imap
    ifpga_header
    alice
)
target_compile_definitions(imap PUBLIC ALICE_ENV_HOME=ALICE_HOME)


add_alice_python_module(imap
    ${PROJECT_SOURCE_DIR}/algorithm/imap.cpp
)