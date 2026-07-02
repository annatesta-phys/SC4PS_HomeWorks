# Monte Carlo simulations with LCG

This project covers five classic Monte Carlo applications, all built
on top of the same pseudo-random number source: a simple 32-bit **linear
congruential generator (LCG)**. Each program generates a dataset (written
to a CSV file), and a single Python script turns every dataset into a plot
for visual/statistical validation.

## Structure of the project

```
HW07/
├── include/
│   └── lcg.h                  # the random number generator (shared by all programs)
├── src/
│   ├── coin_tosses.c          # law of large numbers: running head fraction
│   ├── pi_estimate.c          # Monte Carlo estimate of π
│   ├── U_squared.c            # change of variables: Y = U^2
│   └── inverse_exponential.c  # inverse-transform sampling: Y = -ln(1-U)/λ
├── scripts/
│   └── plots.py               # reads all CSVs in output/ and produces plots/
├── Makefile
├── output/                    # created by `make run` — one CSV per program
└── plots/                     # created by `make plot-python` — one or more PNG per program
```

Each `.c` file in `src/` is a **standalone program** (each has its own
`main`), not a module meant to be linked together. The Makefile compiles
every source file in `src/` into its own executable in `bin/`, and `make
run` runs all of them in a row so that `output/` ends up with every CSV
needed by `scripts/plots.py`.

## The random number generator: `include/lcg.h`

All four programs share the same generator:

```
x_{n+1} = a * x_n + c   (mod 2^32),   a = 1664525, c = 1013904223
```

exposed through two `static inline` functions:

- `lcg32_next(state)` — advances the internal 32-bit state and returns it
  as a `uint32_t`.
- `lcg32_next_double(state)` — same, but rescaled to a `double` in
  `[0, 1)` by dividing by 2³².

Being `static inline` and defined entirely in the header, `lcg.h` can be
`#include`d in every `.c` file without needing a separate `lcg.c` /
linking step. This LCG is a simple didactic generator (a variant of the
classic "Numerical Recipes" constants); it is **not** suitable for
cryptographic use or for simulations demanding very high-quality
randomness, but it is more than adequate for illustrating Monte Carlo
methods, since its period (2³²) and statistical properties are enough
for the values of `N` used here (10⁵–5·10⁵ samples).

## The programs in `src/`

### 1. `coin_tosses.c` — law of large numbers

Simulates `N_TOT = 100000` fair coin tosses: draws `u = lcg32_next_double`,
counts it as heads if `u < 0.5`. At each step it tracks the **running head
fraction** `heads / i` and writes it to `output/coin_tosses.csv`
(`N_TOT,n_heads/N_TOT` header, columns `i, heads/i`), sampling densely for
the first 30 tosses and then every 500 tosses (to keep the file small
while still resolving the early, noisy behaviour). The final head
fraction is printed to stdout and should be close to 0.5 (a run gave
`0.5010100000`).

This dataset illustrates the **law of large numbers**: the running
fraction is expected to converge to the true probability (0.5) as the
number of tosses grows, with fluctuations shrinking roughly as `1/√n`.

Note: the file wraps its content in `#ifndef/#define/#endif COIN_TOSSES_C`
include guards. This is a habit that makes sense for header files meant
to be `#include`d in several translation units, but is not needed for a
`.c` file compiled directly into its own executable — it is harmless here,
just redundant.

### 2. `pi_estimate.c` — Monte Carlo estimate of π

Draws `N_POINTS = 100000` points `(x, y)` uniformly in the unit square
using **two independent generator states** (`state_x`, `state_y`, seeded
differently), and counts how many fall inside the unit quarter-circle
(`x²+y² ≤ 1`). The classical estimator is

```
π ≈ 4 * (points inside the circle) / (total points)
```

At every 100th step (from `i = 100` onward) it writes to
`output/pi_estimate.csv` the current number of points and the **absolute
error** `|4·inside/i − π|` (using `M_PI` from `<math.h>` as reference).
The final estimate is printed to stdout (a run gave `3.1380000000`,
about 0.0036 away from π).

Using two separate LCG states for `x` and `y` is important: reusing a
single state to draw both coordinates in sequence would still work
because consecutive LCG outputs are far apart in the state space, but
keeping them independent removes any doubt about hidden 2D correlations
between consecutive outputs of a simple LCG (a known weakness of LCGs,
the "lattice structure" / spectral test issue).

### 3. `U_squared.c` — change of variables `Y = U^2`

Draws `N_POINTS = 500000` uniform samples `u` and writes both `u` and
`y = u²` to `output/U_squared.csv` (columns `U`, `Y = U^2`). This is a
standard change-of-variables example: if `U ~ Uniform(0,1)`, then `Y=U²`
has probability density

```
f_Y(y) = 1 / (2*sqrt(y)),   for 0 < y < 1,
```

obtained analytically from `f_Y(y) = f_U(u) * |du/dy|` with `u = sqrt(y)`.
The histogram of the simulated `Y` values, compared against this density
in `scripts/plots.py`, is the empirical check that the transformation was
implemented correctly (confirmed visually: the simulated histogram
follows the `1/(2√y)` divergence at `y → 0` closely).

### 4. `inverse_exponential.c` — inverse-transform sampling

Draws `N_POINTS = 500000` uniform samples `u` and applies the inverse
of the exponential CDF,

```
Y = -ln(1 - U) / λ,   λ = 1.5,
```

which is the standard **inverse-transform method**: if `U ~ Uniform(0,1)`
then `Y` follows an `Exponential(λ)` distribution. The samples are sorted
with `qsort` before being written, so that the **empirical CDF** can be
computed by rank: after sorting, the `i`-th smallest sample (0-based) is
less than or equal to exactly `i+1` samples, so its empirical CDF is
`(i+1) / N_POINTS`. The output file `output/inverse_exp.csv` (columns
`Y, CDF`) therefore contains, for each sorted sample, its value and its
empirical CDF — both monotonically increasing from ~0 to 1.

This is verified numerically against the theoretical CDF
`F(y) = 1 - e^{-λy}`: comparing empirical vs. theoretical CDF at several
points gives a near-perfect match (e.g. at `y=1`: empirical ≈ 0.776,
theoretical ≈ 0.777), consistent with the Glivenko–Cantelli theorem
(the empirical CDF converges uniformly to the true CDF as `N → ∞`).

**Design note:** sorting is not just a display convenience here — it is
*required* for the rank-based empirical CDF formula to be meaningful.

## Plotting: `scripts/plots.py`

A single script reads every CSV from `output/` and writes the
corresponding figure(s) to `plots/`, using a generic
`read_csv_columns(path)` helper that skips the header row and reads
exactly two float columns.

| Function | Input CSV | Output figure(s) | What it shows |
|---|---|---|---|
| `plot_head_fraction` | `coin_tosses.csv` | `head_fraction_vs_n.png` | Running head fraction vs. `n` (log-x), with a reference line at 0.5 |
| `plot_pi_error` | `pi_estimate.csv` | `error_pi_estimate_vs_n.png` | Absolute error of the π estimate vs. `n` (log-x), overlaid with a `1/√n` reference curve showing the expected Monte Carlo convergence rate |
| `plot_Y_Usquared` | `U_squared.csv` | `U_squared.png` | Normalized histogram of `Y=U²` vs. the theoretical density `1/(2√y)` |
| `plot_inverse_exp` | `inverse_exp.csv` | `inverse_exp.png`, `inverse_exp_cdf.png` | (1) Histogram of `Y` vs. the theoretical exponential density `λe^{-λy}`; (2) empirical CDF (`Y` vs. `CDF`, as a curve) vs. the theoretical CDF `1-e^{-λy}` |

Two details worth calling out:

- In `plot_pi_error`, the `1/√n` curve is **not** a rigorous confidence
  bound — it is the expected *order of magnitude* of the Monte Carlo
  error (which formally decreases as `O(1/√n)` with a problem-dependent
  constant). It is meant to show the correct convergence *rate*, i.e. the
  two curves should have similar slope on a log-log-style view, not
  overlap exactly.
- In `plot_inverse_exp`, the second figure deliberately plots `Y` vs.
  `CDF` as a **curve**, not as a histogram of the `CDF` column. Since the
  CDF column is, by construction, an evenly spaced sequence between 0 and
  1 (the normalized rank), histogramming it would just show that it is
  uniform and would not be comparable to `F(y) = 1-e^{-λy}` (a function of
  `y`, not a density over `[0,1]`). Plotting `Y` against `CDF` and
  overlaying the theoretical CDF evaluated at the same `Y` values is the
  correct way to validate the sampling.

## Building and running

The `Makefile` compiles every file under `src/` into its own executable
under `bin/`, runs them all, and (optionally) produces the plots:

```bash
make            # compile every program in src/ into bin/
make run        # compile (if needed), then run every program;
                #   creates output/ and fills it with the CSV files
make plot-python  # run the programs, then run scripts/plots.py;
                  #   creates plots/ with all PNG figures
make clean      # remove bin/, output/, plots/, .mplconfig/
```

`plot-python` sets `MPLCONFIGDIR` to a local `.mplconfig/` directory
before invoking Python, so that Matplotlib has a writable cache directory
even in restricted environments (CI runners, shared machines) where the
default `~/.config/matplotlib` might not be writable.

Running `make plot-python` end-to-end (verified while preparing this
documentation) compiles all four programs without warnings
(`-Wall -Wextra`), runs them successfully, and produces all five PNG
files described above, with results consistent with the expected
theoretical behaviour (head fraction → 0.5, π estimate → π, histograms
matching their theoretical densities, empirical CDF matching the
theoretical CDF).

## Known limitations / things to keep in mind

- The LCG in `lcg.h` is a simple teaching generator; it is not intended
  for cryptography or for studies requiring very high-quality randomness
  (e.g. very-high-dimensional integration, or very large sample sizes
  where the LCG's period or lattice structure could become visible).
- CSV headers are for human readability only; `read_csv_columns` always
  skips the first row and reads columns purely by position, so header
  text (including the stray leading spaces after commas, e.g.
  `"N_POINTS, absolute error"`, or the LaTeX-flavoured header in
  `U_squared.csv`) does not affect parsing.
- `coin_tosses.c` wraps its content in include guards even though it is
  compiled as a standalone `.c` file; this is unnecessary but harmless.
