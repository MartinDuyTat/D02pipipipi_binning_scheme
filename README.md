# D02pipipipi_binning_scheme
Binning scheme code for the D0 -> pipipipi decay, based on Hyperplot by Sam Harnew and examples by Xiaodong Shi

Instructions for installation:
```
git clone git@github.com:MartinDuyTat/D02pipipipi_binning_scheme.git
cd D02pipipipi_binning_scheme
mkdir build
cd build
cmake ..
make install -j 4
```

To bin a random event, generated with the seed ```42```, with a binning scheme called ```BesOptimEqualV0.root```, run:
```
MinimalExample BesOptimEqualV0.root 42
```
