# HPC Parallel Linear Regression

> MPI-based parallel linear regression implementation

---

## Overview

This project implements parallel linear regression algorithms using MPI:
- OLS (Ordinary Least Squares) - Direct solver
- GD (Gradient Descent) - Iterative solver (optional)

Performance evaluation through Strong Scaling experiments with Speedup and Efficiency analysis.

---

## Documentation

### [IMPLEMENTATION_GUIDE.md](IMPLEMENTATION_GUIDE.md)
Implementation guide - Execution steps and strategies
- Time planning
- Core and extended tasks
- Key technical points
- Experiment and report workflow

---

## Quick Start

### Requirements
- **MPI Implementation**: OpenMPI or Intel MPI
- **Compiler**: GCC or Intel Compiler
- **Cluster Environment**: HPC3 @ University of Trento, PBS job scheduler

### Compilation and Execution
```bash
# Load MPI module
module load gompi/2023a

# Compile
cd /home/da.li/HPC-Linear-Regression
make

# Quick test
make test

# Run OLS experiment
mpirun -np 4 ./parallel_lr -a ols

# Run GD experiment
mpirun -np 4 ./parallel_lr -a gd -i 1000
```

### Submit Batch Experiments
```bash
# OLS experiment (20 runs)
cd scripts
qsub run_ols_experiment.pbs

# GD experiment (20 runs)
qsub run_gd_experiment.pbs

# Monitor jobs
qstat -u $USER
tail -f scripts/ols_experiment.log
```

### Data Analysis
```bash
# View experiment results
cat results/ols_strong_scaling.csv
cat results/gd_strong_scaling.csv

# Analysis and visualization with Jupyter Notebook
# See analysis.ipynb
```

> **Detailed Commands**: For complete development workflow commands, see [IMPLEMENTATION_GUIDE.md](IMPLEMENTATION_GUIDE.md)

---

## Deliverables

| Item | Description |
|------|-------------|
| Source Code | `ols_parallel.c`, `gd_parallel.c`, `utils.c/h` |
| Build Script | `Makefile` |
| Job Scripts | `run_experiments.pbs` |
| Experiment Data | `results/*.csv` |
| Performance Plots | Speedup, Efficiency, Amdahl's Law |
| Technical Report | 8-10 page PDF |

---

