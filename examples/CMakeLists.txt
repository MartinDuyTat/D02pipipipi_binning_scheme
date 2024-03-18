add_executable(MinimalExample MinimalExample.cpp)
add_executable(BinD02pipipipiEvents BinD02pipipipiEvents.cpp)

target_link_libraries(MinimalExample PUBLIC D02pipipipi_binning_scheme)
target_link_libraries(MinimalExample PUBLIC ROOT::Physics ROOT::RIO ROOT::Tree)

target_link_libraries(BinD02pipipipiEvents PUBLIC D02pipipipi_binning_scheme)
target_link_libraries(BinD02pipipipiEvents PUBLIC ROOT::Physics ROOT::RIO ROOT::Tree)

install(TARGETS BinD02pipipipiEvents MinimalExample DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../bin)
