add_executable(MinimalExample MinimalExample.cpp)
add_executable(BinD02pipipipiEvents BinD02pipipipiEvents.cpp)
add_executable(BinAmpGen BinAmpGen.cpp)

target_link_libraries(MinimalExample PUBLIC D02pipipipi_binning_scheme)
target_link_libraries(MinimalExample PUBLIC ROOT::Physics ROOT::RIO ROOT::Tree)

target_link_libraries(BinD02pipipipiEvents PUBLIC D02pipipipi_binning_scheme)
target_link_libraries(BinD02pipipipiEvents PUBLIC ROOT::Physics ROOT::RIO ROOT::Tree)

target_link_libraries(BinAmpGen PUBLIC D02pipipipi_binning_scheme)
target_link_libraries(BinAmpGen PUBLIC ROOT::Physics ROOT::RIO ROOT::Tree)

install(TARGETS BinD02pipipipiEvents BinAmpGen MinimalExample DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../bin)
