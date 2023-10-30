# add_executable( ifpga
#     ${PROJECT_SOURCE_DIR}/examples/ifpga.cpp
# )
# target_link_libraries( ifpga
#     ifpga_header
#     args
# )
add_executable(balance
    ${PROJECT_SOURCE_DIR}/examples/balance.cpp
)
target_link_libraries(balance
    ifpga_header
)
add_executable(rewrite
    ${PROJECT_SOURCE_DIR}/examples/rewrite.cpp
)
target_link_libraries(rewrite
    ifpga_header
)
add_executable(refactor
    ${PROJECT_SOURCE_DIR}/examples/refactor.cpp
)
target_link_libraries(refactor
    ifpga_header
)
