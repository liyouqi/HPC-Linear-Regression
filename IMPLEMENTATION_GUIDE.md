# HPC Parallel Linear Regression - Implementation Guide

> Step-by-step guide for implementing and running the project.

---

## Project Strategy

### Scope
- **Core algorithm**: OLS (required) + GD (for comparison)
- **Target grade**: 90-95 points
- **Estimated time**: 18-22 hours

### Development Environment
- **Local development**: Debug and verify logic locally first
  - If you can't install OpenMPI locally, use serial version for debugging
  - Make sure algorithm logic is correct before moving to cluster
- **Cluster environment**: To be confirmed (PBS or SLURM)
  - Need cluster for Strong Scaling experiments
  - Test basic MPI program first to confirm environment

### Technical Choices

**Data generation**:
- Generate synthetic data at runtime
- Use fixed random seed for reproducibility
- Advantage: No need to store large files, easy to adjust parameters

**Linear system solver**:
- Pure C implementation using Gaussian elimination
- No external library dependencies
- Works for medium-scale problems (d ≤ 1000)

**Code structure**:
```
src/
├── main.c              # Main entry point
├── data.c/h            # Data generation module
├── ols.c/h             # OLS algorithm
├── gd.c/h              # GD algorithm (optional)
├── linear_solver.c/h   # Linear system solver
├── utils.c/h           # Helper functions (timing, validation, etc.)
└── Makefile            # Build script
```

---

## Time Planning

**Total workload: 15-22 hours**

| Timeline | Route | Expected Grade |
|---------|-------|----------------|
| Within 2 weeks | OLS + Strong Scaling only | 85-88 |
| 2-3 weeks | OLS + GD + Amdahl's Law | 90-95 |
| 3+ weeks | OLS + GD + Hybrid | 95-100 |

---

## Environment Setup

### Local Environment (for debugging)

**Option 1: Install OpenMPI (recommended)**
```bash
# macOS
brew install open-mpi

# Verify installation
mpicc --version
mpirun --version
```

**Option 2: Serial debugging (if OpenMPI install fails)**
- Use conditional compilation to separate serial and parallel code
- Verify algorithm logic in serial mode
- Leave parallel communication parts empty or use mock data

**Verify local setup**:
- Write a simple "Hello World" MPI program
- Run locally: `mpirun -np 4 ./test`
- Make sure basic MPI works

### Cluster Environment

**First login checklist**:
1. Check available MPI implementations (`module avail mpi`)
2. Check job scheduler (qsub or sbatch)
3. Check compiler version (`gcc --version`)
4. Understand queue policies and walltime limits
5. Test simple job submission and output retrieval

**Environment module loading** (example):
```bash
# Usually need to load these modules
module load gcc/11.2
module load openmpi/4.1.1
# or
module load intel/2021.4
module load impi/2021.4
```

---

## Implementation Steps

### Phase 1: Basic Framework (Day 1-4)

**Step 1.1: Project initialization**
- Create directory structure
- Write basic Makefile
- Prepare .gitignore (exclude build artifacts)

**Step 1.2: Data generation module**
- Implement `data.c/h`
- Test data generation correctness
- Verify reproducibility (same seed → same data)

**Step 1.3: Serial OLS implementation**
- Implement Gaussian elimination (`linear_solver.c`)
- Implement serial OLS (no MPI)
- Verify results with small data
- This is the baseline for parallel version

**Acceptance criteria**:
- Serial version correctly solves for beta
- Error with beta_true is reasonable
- Code compiles and runs

### Phase 2: OLS Parallelization (Day 5-7)

**Step 2.1: MPI initialization and data distribution**
- Add MPI_Init/Finalize
- Implement load-balanced data distribution
- Use MPI_Scatterv to distribute X
- Use MPI_Bcast to distribute y

**Step 2.2: Parallel computation and aggregation**
- Each process computes local_XtX and local_Xty
- Use MPI_Reduce to gather to Rank 0
- Rank 0 solves and outputs result

**Step 2.3: Correctness verification**
- Compare parallel and serial results
- Test different process counts (p=1,2,4)
- Make sure results match (error < 1e-6)

**Step 2.4: Timing measurement**
- Add MPI_Wtime timing
- Measure communication and computation time separately
- Output to stdout or file

**Acceptance criteria**:
- Results consistent across different process counts
- Local tests pass (if possible)
- Code ready for cluster deployment

### Phase 3: GD Implementation (Day 7)

**Step 3.1: GD serial version**
- Implement iterative gradient descent
- Fix to 10 iterations
- Verify convergence

**Step 3.2: GD parallel version**
- Reuse OLS data distribution code
- Implement communication in iteration loop
- Each iteration: Reduce gradient, Bcast parameters

**Step 3.3: Comparison testing**
- Record GD and OLS running times
- Verify both methods get similar beta values

**Acceptance criteria**:
- GD converges correctly
- Parallel version works properly
- Ready for comparison with OLS

### Phase 4: Cluster Deployment and Experiments (Day 8-14)

**Step 4.1: Environment setup (Day 8)**
- Login to cluster, check environment
- Load necessary modules
- Compile code, fix environment issues

**Step 4.2: Test jobs (Day 9)**
- Write simple PBS/SLURM script
- Submit small test job
- Verify output and logs

**Step 4.3: Batch experiments (Day 10-12)**
- Write experiment scripts (loop over different p)
- OLS: p=1,2,4,8,16, 3-5 runs each
- GD: same configs
- Collect all output files

**Step 4.4: Data organization (Day 13-14)**
- Parse output files, extract timing data
- Organize into CSV format
- Calculate mean and standard deviation
- Preliminary check for data reasonableness

**Acceptance criteria**:
- At least 3 valid data points for all configs
- Data trend is reasonable (Speedup increases)
- No obvious outliers

### Phase 5: Analysis and Report (Day 15-21)

**Step 5.1: Performance analysis (Day 15-16)**
- Calculate Speedup and Efficiency
- Generate plots (Python/MATLAB)
- OLS vs GD comparison
- Amdahl's Law fitting (optional)

**Step 5.2: Report writing (Day 17-19)**
- Complete chapter by chapter
- Organize figures as you write
- Ensure logical flow

**Step 5.3: Final check (Day 20-21)**
- Grammar and spelling check
- Figure quality check
- Reference format check
- Package code and data

**Acceptance criteria**:
- Report 8-10 pages
- All required figures included
- No obvious errors
- Ready for submission

---

## Task Checklist

### Core Tasks

**OLS MPI Implementation** (4-6 hours)
- Data distribution (row-wise partitioning)
- Local computation of XtX and Xty
- MPI Reduce aggregation
- Solve linear system (Gaussian elimination)

**Strong Scaling Experiment** (3-5 hours)
- Fixed problem size: n=100000, d=100
- Test process counts: p = 1, 2, 4, 8, 16
- 3-5 runs per configuration
- Record timing data

**Performance Analysis** (2-3 hours)
- Calculate Speedup and Efficiency
- Generate 2 plots
- Python/MATLAB data processing

**Technical Report** (6-8 hours)
- 8-10 pages
- Include plots and analysis
- English writing

### Extended Tasks (Optional)

**GD MPI Implementation** (2-3 hours)
- Fixed 10 iterations
- Show communication overhead impact
- Compare with OLS performance

**Amdahl's Law Analysis** (1-2 hours)
- Fit serial fraction f
- Predict theoretical limit

### Advanced Tasks (Optional)

**Hybrid MPI+OpenMP** (2-3 hours)
- Test different ratios (4×4, 8×2, etc.)
- Find optimal configuration

**Weak Scaling** (optional, 2-3 hours)
- Fixed load per process
- Evaluate scalability

---

## Key Technical Points

### 0. Data Generation Strategy

**Runtime dynamic generation (using this approach)**

**Advantages**:
- No need to store large files
- Easy to adjust parameters (n, d)
- Ensures reproducibility (fixed random seed)

**Implementation**:
- Implement generation function in `data.c`
- Use standard library `rand()` with fixed `srand(seed)`
- Only Rank 0 generates complete data
- Then distribute via MPI to other processes


**Data Generation Function Interface**:
```
void generate_synthetic_data(
    double *X,      // n × d matrix
    double *y,      // n × 1 vector
    double *beta_true,  // d × 1 true parameters (for verification)
    int n,          // number of samples
    int d,          // number of features
    unsigned int seed   // random seed
);
```

**Generation Logic**:
1. Generate random X matrix (uniform or normal distribution)
2. Generate true parameters beta_true
3. Calculate y = X * beta_true + noise
4. Add small noise for realism

**Verification approach**:
- Verify with small-scale data (n=100, d=5) by hand
- Compare serial and parallel version results
- Make sure different process counts get same beta (error < 1e-6)

### 1. Data Distribution Strategy

**Goal**: Evenly distribute n×d matrix X by rows to p processes

**Load balancing formula**:
- `local_n = n / p + (rank < n % p ? 1 : 0)`
- `start_row = rank * (n / p) + min(rank, n % p)`

**Notes**:
- Make sure each process gets roughly equal load
- Last process might have 1-2 extra rows (acceptable)

---

### 2. MPI Communication Patterns

**Required MPI operations**:

1. **MPI_Bcast** - Broadcast dimension info
   - Usage: All processes get n, d parameters

2. **MPI_Scatterv** - Distribute data
   - Usage: Rank 0 distributes different rows of X to each process
   - Note: Need to calculate sendcounts and displs arrays

3. **MPI_Reduce** - Aggregate results
   - Usage: Aggregate local XtX and Xty
   - Operation: MPI_SUM
   - Target: Rank 0

4. **MPI_Barrier** - Synchronization (optional)
   - Usage: Ensure accurate timing measurement
   - Position: Before and after main computation

---

### 3. Linear System Solver

**Implementation: Pure C Gaussian elimination**

- No external library dependencies
- Works for medium-scale problems (d ≤ 1000)

**Algorithm steps**:
1. Forward elimination (convert matrix to upper triangular)
   - Choose pivot (optional: partial pivoting)
   - Eliminate elements below
2. Back substitution (compute from bottom to top)
   - Start from last row
   - Solve unknowns row by row

**Implementation tips**:
- Implement in `linear_solver.c`
- Interface: `void solve_linear_system(double *A, double *b, double *x, int n)`
- Only call on Rank 0
- For d=100, execution time < 1ms, negligible

**Notes**:
- XtX is d×d symmetric matrix, computation O(d³)
- Need numerical stability check (pivot non-zero)
- Can start with simple implementation, optimize later if needed

**Debugging strategy**:
- Verify with small problems (d=3, compare with hand calculation)
- Test with identity matrix
- Compare with Python/MATLAB results

---

### 4. OLS Parallel Workflow

**Phase 1: Initialization**
- Rank 0 reads data or generates synthetic data
- Broadcast n, d to all processes

**Phase 2: Data distribution**
- Calculate each process's local_n
- Scatterv to distribute rows of X
- Bcast to distribute y (each process needs corresponding part)

**Phase 3: Local computation**
- Each process computes:
  - `local_XtX = local_X^T * local_X` (d×d matrix)
  - `local_Xty = local_X^T * local_y` (d×1 vector)

**Phase 4: Global aggregation**
- Reduce local_XtX → global_XtX (to Rank 0)
- Reduce local_Xty → global_Xty (to Rank 0)

**Phase 5: Solve (Rank 0)**
- Use Gaussian elimination to solve `XtX * beta = Xty`
- Output results

---

### 5. GD Parallel Workflow

**Initialization**:
- Rank 0 initializes beta = 0
- Broadcast beta to all processes

**Iteration loop (repeat 10 times)**:

1. **Local gradient computation**
   - Compute `local_pred = local_X * beta`
   - Compute `local_error = local_pred - local_y`
   - Compute `local_grad = local_X^T * local_error`

2. **Global aggregation**
   - Reduce local_grad → global_grad

3. **Parameter update (Rank 0)**
   - `beta = beta - (learning_rate / n) * global_grad`

4. **Broadcast new parameters**
   - Bcast beta to all processes

**Key difference**:
- GD needs communication every iteration (10 times)
- OLS only communicates once
- **That's why GD has lower parallel efficiency**

---

## Experiment Design

### Strong Scaling Experiment

**Problem size**:
- n = 100000 (number of samples)
- d = 100 (number of features)
- Fixed

**Process configurations**:
- p = 1, 2, 4, 8, 16
- 3-5 runs per configuration

**Data recording**:
- Total execution time (MPI_Wtime)
- Computation time (optional)
- Communication time (optional)

**Metrics to calculate**:
- Speedup(p) = T(1) / T(p)
- Efficiency(p) = Speedup(p) / p

---

### PBS Job Script Essentials

**Required settings**:
- Number of nodes and processes
- Walltime (suggest 10-30 minutes)
- Output file paths
- Module loading (MPI, GCC, etc.)

**Environment variables**:
- `OMP_NUM_THREADS=1` (pure MPI)
- `OMP_NUM_THREADS=4` (Hybrid)

**Run command**:
- `mpirun -np $NPROCS ./ols_parallel`

---

## Performance Analysis Workflow

### Data Processing

**Steps**:
1. Aggregate CSV data from all experiments
2. Calculate mean time and standard deviation for each configuration
3. Compute Speedup and Efficiency

**Tools**:
- Python: pandas, numpy, matplotlib
- MATLAB: readtable, plot
- Excel: works for simple cases

---

### Plotting Charts

**Required Charts**:

1. **Speedup vs Process Count**
   - X-axis: Process count (log scale)
   - Y-axis: Speedup
   - Include ideal line (y=x)

2. **Efficiency vs Process Count**
   - X-axis: Process count
   - Y-axis: Efficiency (0-1)
   - Include 100% reference line

**Bonus Charts**:

3. **OLS vs GD Comparison**
   - Show both Speedup curves in same chart
   - Label why they differ

4. **Amdahl's Law Fit**
   - Measured points + fitted curve
   - Label serial fraction f

**Requirements**:
- Resolution ≥ 300 dpi
- Clear titles and legends
- Professional colors (not defaults)

---

## Report Writing Tips

### Report Structure (8-10 pages)

1. **Introduction** (0.5 pages)
   - Project background and objectives

2. **Background** (1 page)
   - Linear regression theory
   - Parallel computing basics

3. **Parallel Design** (1.5 pages)
   - Data distribution strategy
   - MPI communication patterns
   - Algorithm parallelization

4. **Implementation** (1 page)
   - Development environment
   - Key implementation details

5. **Experiments** (2 pages)
   - Experimental environment
   - Experimental parameters
   - Performance metrics

6. **Results** (2-3 pages)
   - Data tables
   - Performance charts
   - Analysis and discussion

7. **Discussion** (1 page)
   - Performance bottlenecks
   - Optimization opportunities
   - Limitations

8. **Conclusion** (0.5 pages)
   - Main contributions
   - Experimental conclusions

---

### Key Analysis Points

**OLS Performance Analysis**:
- Why is Speedup < p?
- Why does efficiency drop as p increases?
- Estimate communication overhead percentage

**GD Performance Analysis** (if implemented):
- Why is GD parallel efficiency lower than OLS?
- Impact of communication frequency
- Difference in computation/communication ratio

**Amdahl's Law Analysis** (if done):
- Fitted serial fraction f
- Theoretical maximum speedup
- Analysis of serial portion sources

---

## Timeline

### Recommended Plan (3 weeks, OLS + GD)

**Week 1: Algorithm Implementation and Local Testing**
- Day 1-2: Environment prep, learn MPI basics
  - Set up local dev environment (if possible)
  - Study cluster environment and job submission
- Day 3-4: Data generation and serial OLS
  - Implement data generation module
  - Implement serial OLS as baseline
  - Verify algorithm correctness
- Day 5-6: OLS parallelization
  - Implement data distribution (Scatterv)
  - Implement local computation and Reduce
  - Local small-scale testing (if possible)
- Day 7: GD serial and parallel implementation
  - Reuse OLS communication framework
  - Implement iterative gradient descent

**Week 2: Cluster Experiments and Data Collection**
- Day 8-9: Cluster setup and initial testing
  - Confirm PBS/SLURM submission method
  - Test basic MPI program
  - Fix environment issues
- Day 10-12: Strong Scaling experiments
  - OLS experiments (p = 1, 2, 4, 8, 16)
  - GD experiments (same configs)
  - 3-5 runs per configuration
- Day 13-14: Data processing and initial analysis
  - Organize experimental data
  - Calculate Speedup and Efficiency
  - Validate data

**Week 3: Performance Analysis and Report Writing**
- Day 15-16: Performance charts
  - Speedup vs process count
  - Efficiency vs process count
  - OLS vs GD comparison
  - Amdahl's Law fit (optional)
- Day 17-19: Report writing
  - Introduction, Background, Design
  - Implementation, Experiments
  - Results and Discussion
- Day 20-21: Report polish and final check
  - Check chart quality
  - Grammar and spelling
  - Completeness check

### Key Milestones
- Day 4: Serial version verified
- Day 6: Parallel version passes local tests
- Day 9: First successful run on cluster
- Day 12: All experimental data collected
- Day 16: All charts completed
- Day 21: Report submission

---

## Submission Checklist

### Pre-submission Self-check

**Code**:
- Code compiles without warnings
- Serial and parallel results match
- Includes Makefile and README

**Experiments**:
- At least 5 different process counts
- 3+ runs per configuration
- Raw data files included

**Charts**:
- At least 2 charts (required)
- Charts are clear and nice-looking
- Have titles and legends

**Report**:
- Complete structure (8 chapters)
- All required charts included
- No obvious grammar errors
- PDF format, 8-12 pages

---

## FAQ

### Q1: How to verify parallel version correctness?
**A**: Compare beta results from serial (p=1) and parallel versions. Error should be < 1e-6

### Q2: What if cluster job is stuck in queue?
**A**:
- Choose shorter walltime (like 10 mins)
- Submit during off-peak hours
- Request fewer resources

### Q3: What if experiments take too long?
**A**:
- Reduce problem size (but keep n >> d)
- Fewer repetitions (minimum 3)

### Q4: How many algorithms do I need?
**A**:
- Minimum: OLS only (85 points)
- Recommended: OLS + GD (90-95 points)

---

## Development Workflow Commands

> Key commands for each implementation step, for quick reference and reproduction

### Phase 1: Basic Module Verification

#### Step 1.2 - Test Data Generation Module
```bash
# Enter source directory
cd /home/da.li/HPC-Linear-Regression/src

# Compile data generation test
gcc -o tests/test_data tests/test_data.c data.c -lm -std=c99 -Wall

# Run test
./tests/test_data
```

**Expected output**:
- Generate test data with n=10, d=3
- Show beta_true, first few rows of X, y values
- Reproducibility test passed

---

#### Step 1.3 - Test Serial OLS
```bash
# Load MPI module (need mpicc compiler)
module load gompi/2023a

# Compile serial OLS test
cd /home/da.li/HPC-Linear-Regression/src
mpicc -o tests/test_serial tests/test_serial.c data.c ols.c linear_solver.c utils.c -lm -std=c99 -Wall

# Run test
./tests/test_serial
```

**Expected output**:
- Generate data with n=100, d=5
- Show true beta and computed beta
- Error around 0.1 (due to noise)
- TEST PASSED

---

### Phase 2: OLS Parallelization

#### Step 2.1-2.3 - Implement and Test Parallel OLS
```bash
# Compile parallel OLS test
cd /home/da.li/HPC-Linear-Regression/src
mpicc -o tests/test_parallel tests/test_parallel.c data.c ols.c linear_solver.c utils.c -lm -std=c99 -Wall

# Test with different process counts
mpirun -np 1 ./tests/test_parallel
mpirun -np 2 ./tests/test_parallel
mpirun -np 4 ./tests/test_parallel
```

**Expected output**:
- Parallel results match serial results for all process counts
- Difference < 1e-14 (within numerical precision)
- TEST PASSED

---

#### Step 2.4 - Main Program and Timing
```bash
# Compile main program
cd /home/da.li/HPC-Linear-Regression
make

# Show help
./parallel_lr -h

# Quick test (small scale)
make test

# Manual runs with different process counts
mpirun -np 1 ./parallel_lr -n 1000 -d 10
mpirun -np 2 ./parallel_lr -n 1000 -d 10
mpirun -np 4 ./parallel_lr -n 1000 -d 10

# Run full-scale experiment (n=100000, d=100)
mpirun -np 1 ./parallel_lr  # ~13.6s
mpirun -np 2 ./parallel_lr  # ~6.3s (Speedup: 2.15x)
mpirun -np 4 ./parallel_lr  # ~3.3s (Speedup: 4.13x)
```

**Expected output**:
- Show problem size and MPI process count
- Output execution time
- Show computed beta coefficients (first 5)
- CSV format output (for data analysis)

**Verification success**:
- Timing works properly (MPI_Barrier + MPI_Wtime)
- Results consistent across different process counts
- Observed clear speedup (approaching ideal)

---

### Experiment Job Submission and Monitoring

#### Submit Full Experiment Job
```bash
cd /home/da.li/HPC-Linear-Regression/scripts
qsub run_ols_experiment.pbs
```

#### Monitor Job Status
```bash
# Check if job is running
qstat -u $USER

# Watch log in real-time (Ctrl+C to exit)
tail -f scripts/ols_experiment.log

# View last 20 lines of log
tail -20 scripts/ols_experiment.log

# Count generated result files
ls results/*.txt 2>/dev/null | wc -l

# Check if job completed
qstat -u $USER | grep ols_experiment || echo "Job completed!"

# View CSV data after completion
cat results/ols_strong_scaling.csv
```

---

### Phase 3: GD (Gradient Descent) Experiment

#### Step 3.1 - Quick Test of GD Algorithm
```bash
# Compile main program (if not already done)
cd /home/da.li/HPC-Linear-Regression
make

# Run GD algorithm (small-scale test)
mpirun -np 1 ./parallel_lr -n 1000 -d 10 -a gd -i 1000
mpirun -np 2 ./parallel_lr -n 1000 -d 10 -a gd -i 1000
mpirun -np 4 ./parallel_lr -n 1000 -d 10 -a gd -i 1000

# Run full-scale GD experiment (n=100000, d=100, 1000 iterations)
mpirun -np 1 ./parallel_lr -a gd -i 1000
mpirun -np 2 ./parallel_lr -a gd -i 1000
mpirun -np 4 ./parallel_lr -a gd -i 1000
mpirun -np 8 ./parallel_lr -a gd -i 1000
```

**Expected output**:
- Show problem size and iteration count
- Output execution time (slower than OLS due to multiple iterations)
- Show computed beta coefficients
- CSV format output

---

#### Step 3.2 - Submit Full GD Experiment Job
```bash
# Make sure PBS script exists
cd /home/da.li/HPC-Linear-Regression/scripts
ls -lh run_gd_experiment.pbs

# Submit GD experiment job
qsub run_gd_experiment.pbs
```

**Job info**:
- Run 4 process configurations: p = 1, 2, 4, 8
- 5 runs per configuration
- Total 20 experiment runs
- Estimated total time: ~15-20 minutes

---

#### Step 3.3 - Monitor GD Job Status
```bash
# Check if GD job is running
qstat -u $USER

# Watch GD log in real-time
tail -f scripts/gd_experiment.log

# Count generated GD result files
ls results/gd_*.txt 2>/dev/null | wc -l

# Check if GD job completed
qstat -u $USER | grep gd_experiment || echo "GD job completed!"

# View GD experiment CSV data
cat results/gd_strong_scaling.csv
```

**Success indicators**:
- Generated 20 `results/gd_n100000_d100_p*_run*.txt` files
- Generated `results/gd_strong_scaling.csv` summary file
- CSV contains 20 rows (4 processes × 5 repeats)
- Timing data shows speedup trend

---

#### Step 3.4 - Verify GD Experiment Data
```bash
# Check GD CSV data format
head -5 results/gd_strong_scaling.csv

# Count runs per process configuration
cut -d',' -f2 results/gd_strong_scaling.csv | tail -n +2 | sort | uniq -c

# Calculate average time (simple verification)
awk -F',' 'NR>1 {sum[$2]+=$3; count[$2]++} END {for(p in sum) print "p="p": avg="sum[p]/count[p]"s"}' results/gd_strong_scaling.csv | sort
```

**Expected output**:
```
5 1    # p=1 ran 5 times
5 2    # p=2 ran 5 times
5 4    # p=4 ran 5 times
5 8    # p=8 ran 5 times

p=1: avg=50-55s
p=2: avg=25-28s
p=4: avg=13-15s
p=8: avg=7-9s
```

---

## Final Deliverables

**Must submit**:
- Source code (`src/`)
- Makefile
- PBS scripts (`scripts/`)
- Experiment data (`results/`)
- Performance plots (`plots/`)
- Technical report (`report.pdf`)
- README.md

**File organization**:
```
project/
├── src/
│   ├── ols_parallel.c
│   ├── gd_parallel.c (optional)
│   ├── utils.c/h
│   └── Makefile
├── scripts/
│   └── run_experiments.pbs
├── results/
│   └── *.csv
├── plots/
│   └── *.png
├── report.pdf
└── README.md
```

---

*Last updated: January 11, 2026*
