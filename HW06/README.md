# Vector sum with single cycle and chunks

The goal of this homework is to extend the `vectorlinearcombination_dynamic.c`
code, where the computation of `D = a*X + Y` was performed with a single
`for` loop. The linear combination is now split into chunks, partial sums
are evaluated for each chunk, the input parameters are read from a
configuration file, and the results are saved to an HDF5 file.

## Why split the computation into chunks?

At first sight, splitting a single loop into an outer loop over chunks and
an inner loop over the elements of each chunk looks like unnecessary
complexity, since a single `for` loop already produces the correct `D` vector.
The chunked structure is introduced on purpose, as a pedagogical example of
how many improvements can be implemented by splitting the structure of more complex
codes in chunks:

- **It is the basic building block of parallel computing.** Once the work
  is expressed as independent chunks, each chunk can be assigned to a
  different thread, process, or GPU block, with little or no change to the inner loop.
  It is the preparatory step to parallelizing the code later.
- **It enables a parallel/hierarchical reduction for the sum.** Summing
  `D` in one direct loop is a strictly sequential reduction: every
  addition depends on the previous one. Computing one `partial_chunk_sum`
  per chunk turns the reduction into two independent stages (i.e., many
  independent partial sums, followed by one small final sum) which is
  exactly the pattern used to compute reductions in parallel.
- **It improves memory and cache locality.** Processing data in fixed-size
  blocks that fit comfortably in cache reduces cache misses, and is especially
  relevant when `N` becomes so large that `X`, `Y`, and `D` no longer fit in cache
  together.
- **It allows incremental processing and I/O.** Because each chunk is
  self-contained, it can be saved, checked, or streamed to disk as soon as
  it is ready, without waiting for the whole vector to be computed. This
  is exactly how the results are organized in the HDF5 file produced by
  this program (one dataset per chunk).
- **It makes correctness easier to localize.** If a check fails, knowing
  which chunk produced the wrong result narrows down the search
  immediately, instead of having to inspect the entire vector.

## Structure of the project

The project has a modular structure, organized as follows:

1. an `include/` directory, containing the header files:
   - `config_parser.h`, declaring the functions that read input from a
     configuration file;
   - `daxpy_chunks.h`, declaring the functions that build the `D` vector
     and perform the sums and correctness checks;
   - `hdf5_writer.h`, declaring the function that saves the outputs to an
     HDF5 file.
2. a `src/` directory, containing:
   - `config_parser.c`, `daxpy_chunks.c`, `hdf5_writer.c`, which implement
     the functions declared in the corresponding header files;
   - `main.c`, which implements the main function of the program: it
     computes both the single-loop and chunked versions of `D`, computes
     the sums, and compares the outcomes.
3. a `Makefile`, which allows an optimized compilation of the program;
4. a `config.txt` file, from which the program reads its input parameters
   at runtime. Reading parameters at runtime avoids recompiling the code
   every time an input value changes, which can be costly for more
   complex programs.

## How the code works

### 1. `config_parser.h` / `config_parser.c`

This module reads parameters from a `.txt` file written with the syntax
`Key = Value`, as follows:

1. it reads the file row by row;
2. it ignores comments and empty rows (starting with `#` or `;`);
3. it looks for the `=` symbol, splits the row into key and value, and
   trims leading/trailing blank spaces (`trim`);
4. it stores the key/value pairs in a table (`ConfigTable`), which can be
   queried through the functions `config_get_double`, `config_get_long`,
   `config_get_int`, and `config_get_string`.

### 2. `daxpy_chunks.h` / `daxpy_chunks.c`

This module contains both the single-loop (original) version of the
vector combination, `daxpy_single`, and the chunked version,
`daxpy_chunked`. The function `compute_partial_chunk_sums` evaluates, for
each chunk, the sum of the `D` values it contains. `sum_array` is used
both to sum `partial_chunk_sum` and to evaluate the direct sum of `D`,
taken as reference. `vectors_are_close` compares two vectors component by
component within a given tolerance, and is used to check the correctness
of the chunked `D` against the single-loop `D`.

### 3. `hdf5_writer.h` / `hdf5_writer.c`

This module saves the output to an HDF5 file, with the following layout:

- one dataset per chunk, under the `/chunks/` group (e.g.
  `/chunks/chunk_0000`, `/chunks/chunk_0001`, ...);
- the full array of partial sums, in `/partial_chunk_sum`;
- the full `D` vector, in `/D_full`;
- a few input parameters as attributes (`a`, `N`, `chunk_size`,
  `number_of_chunks`), which make it possible to recover the simulation
  parameters with `h5dump` without having to check `config.txt`.

### 4. `main.c`

This file carries out the following steps:

1. reads `config.txt`;
2. allocates and initializes `X` and `Y`;
3. computes `D` with both `daxpy_single` and `daxpy_chunked`, and verifies
   that the two results coincide;
4. computes `partial_chunk_sum` and compares the sum of the partial sums
   with the direct sum of `D`, using a small tolerance (`1e-9`), since the
   two sums perform the additions in a different order and can therefore
   differ by a small rounding error even when both algorithms are correct;
5. saves the results to HDF5 format using `hdf5_writer`.

## Compiling

Compilation requires the HDF5 development library to be installed. Once
this prerequisite is satisfied, run:

```bash
make        # compiles the executable ./HW06
make run    # compiles (if needed) and runs it with config.txt
make clean  # removes the compiled files and output.h5
```

The `Makefile` automatically looks for the HDF5 compiler/linker flags
using `pkg-config`.

## Inspecting the HDF5 file

The HDF5 command-line tools (`hdf5-tools`) allow inspecting the file
directly:

```bash
h5dump -H output.h5                        # structure only (header)
h5dump output.h5                           # structure and full content
h5dump -a a -a N -a chunk_size output.h5   # input parameters only
```

## Modifying the input parameters

The input parameters can be changed by editing `config.txt` (or a copy of
it), then passing its path as the first argument:

```bash
./HW06 config.txt
```
