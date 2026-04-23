# Hard Sphere MD — Modifications to Skoge Code

Original code by Monica Skoge (2006, Princeton University).

---

## Changes Made

### 1. User-controlled random seed (`seed`)

**Problem:** The original code seeded the RNG with `srandom(time(0))` (wall-clock time), making runs non-reproducible.

**Files changed:**
- `read_input.h` — added `unsigned int seed` field
- `read_input.C` — parses and prints `seed` from the input file
- `input` — added `unsigned int seed = 42`
- `box.h` — updated constructor signature to accept `unsigned int seed`
- `box.C` — constructor now calls `srandom(seed)` instead of `srandom(time(0))`
- `spheres.C` — passes `input.seed` to the `box` constructor

**Usage:** Set `seed` in the `input` file to any positive integer. Two runs with the same seed produce identical initial positions and velocities.

---

### 2. Equilibration phase before growth (`equilsteps`)

**Problem:** The original code had no equilibration phase — it immediately started growing spheres from the initial (random) configuration. This means the system has no time to relax before compression begins.

**Files changed:**
- `read_input.h` — added `int equilsteps` field
- `read_input.C` — parses and prints `equilsteps` from the input file
- `input` — added `int equilsteps = 100`
- `spheres.C` — added equilibration loop before the growth loop

**What it does:** Before the growth loop starts, the code temporarily sets `growthrate = 0` and runs `equilsteps` cycles of `eventspercycle * N` events each, followed by `Synchronize(true)` (velocity rescaling + event reset). After equilibration, `growthrate` is restored to the value in the input file and the normal compression loop proceeds.

**Usage:** Set `equilsteps` in the `input` file to the desired number of equilibration cycles. Set to `0` to skip equilibration entirely (reproduces original behavior).

---

## Input File Format

The `input` file must contain fields in this exact order:

```
int eventspercycle = 1         # avg events per sphere between output/sync steps (controls sampling frequency, not pf accuracy)
int N = 10000                  # number of spheres
double initialpf = 0.01        # initial packing fraction
double maxpf = 0.25            # max packing fraction (stop condition)
double temp = 0.2              # initial temperature (0 = zero velocities)
double growthrate = 0.0001     # sphere growth rate
double maxpressure = 100.      # max pressure (stop condition)
int equilsteps = 100           # equilibration cycles at fixed radius (0 to skip)
unsigned int seed = 42         # RNG seed for reproducibility
char* readfile = new           # config file to read; "new" creates fresh config
char* writefile = config.dat   # file to write final configuration
char* datafile = stats.dat     # file to write packing fraction / pressure data
```
