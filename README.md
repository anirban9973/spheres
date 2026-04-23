# Hard Sphere MD — Modifications to Skoge Code

Original code by Monica Skoge (2006, Princeton University).

---

## Changes Made

### 1. User-controlled random seed (`seed`)

**Problem:** The original code seeded the RNG with `srandom(time(0))` (wall-clock time), making runs non-reproducible.

**Files changed:**
- `read_input.h` — added `unsigned int seed` field
- `read_input.C` — reads seed from command-line argument, updated syntax to `spheres input seed`
- `box.h` — updated constructor signature to accept `unsigned int seed`
- `box.C` — constructor now calls `srandom(seed)` instead of `srandom(time(0))`
- `spheres.C` — passes `input.seed` to the `box` constructor

**Usage:** Pass the seed as a command-line argument: `./spheres input 42`. Two runs with the same seed produce identical initial positions and velocities.

---

### 2. Equilibration phase after compression (`equilsteps`)

**Problem:** The original code had no equilibration phase — it stopped as soon as the target packing fraction or max pressure was reached, with no time for the system to relax at the final density.

**Files changed:**
- `read_input.h` — added `int equilsteps` field
- `read_input.C` — parses and prints `equilsteps` from the input file
- `input` — added `int equilsteps = 1000`
- `spheres.C` — added equilibration loop after the growth loop

**What it does:** After the growth loop reaches the target packing fraction, the code sets `growthrate = 0` and runs `equilsteps` cycles of `eventspercycle * N` events each, followed by `Synchronize(true)` (velocity rescaling + event reset), keeping the radius fixed. The final configuration written to disk reflects the equilibrated state.

**Usage:** Set `equilsteps` in the `input` file to the desired number of equilibration cycles. Set to `0` to skip equilibration entirely (reproduces original behavior).

---

## Changing the Dimension

The spatial dimension is set at compile time in `vector.h` line 7:

```c
#define DIM 2
```

Change this value and recompile (`make clean && make`) to run in a different dimension.

---

## Input File Format

Run as: `./spheres <inputfile> <seed> <initialpf> <maxpf>`

Example: `./spheres input 42 0.40 0.73`

The `input` file must contain fields in this exact order:

```
int eventspercycle = 1         # avg events per sphere between output/sync steps (controls sampling frequency, not pf accuracy)
int N = 10000                  # number of spheres
double temp = 0.2              # initial temperature (0 = zero velocities)
double growthrate = 0.0001     # sphere growth rate
double maxpressure = 100.      # max pressure (stop condition)
int equilsteps = 100           # equilibration cycles at fixed radius (0 to skip)
char* readfile = new           # config file to read; "new" creates fresh config
char* writefile = config.dat   # file to write final configuration
char* datafile = stats.dat     # file to write packing fraction / pressure data
```
