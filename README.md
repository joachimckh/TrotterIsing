# TrotterIsing
Trotter 1D transverse field Ising model

## Installation
To install, you must have make installed on your system. Makefile must be modified to point to your compiler, and to point to the Eigen library location.
Install via Makefile, run the following command:
```bash 
make 
```
To execute the program, run:
```bash
sh run.sh
```

## Description
Results are written to a .txt file, and can be plotted via
```bash
python state.py
```

The input parameters can be changed in the ising_params.txt file.
