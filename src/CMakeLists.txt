add_library(D02pipipipi_binning_scheme
	    BinningBase.cpp
	    HistogramBase.cpp
	    HyperBinning.cpp
	    HyperBinningMemRes.cpp
            HyperCuboid.cpp
	    HyperHistogram.cpp
	    HyperPoint.cpp
	    HyperVolume.cpp
	    Utilities.cpp)

target_include_directories(D02pipipipi_binning_scheme PUBLIC ../include)

target_link_libraries(D02pipipipi_binning_scheme PUBLIC ROOT::Physics ROOT::Tree ROOT::Gpad ROOT::MathMore)
