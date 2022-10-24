add_executable(MinimalExample MinimalExample.cpp)

target_link_libraries(MinimalExample PUBLIC D02pipipipi_binning_scheme)
target_link_libraries(MinimalExample PUBLIC ROOT::Physics ROOT::RIO ROOT::Tree)

install(TARGETS MinimalExample DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../bin)
