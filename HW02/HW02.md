# Linear combination of two vectors in C
  This guide is meant to give a detailed description of how the C codes `vectorlinearcombination_static.c` and `vectorlinearcombination_dynamic.c` are built and work.

## Goal of the codes
  These C program have the goal of computing the linear combination of two N-dimensional vectors, according to the formula D = a · X + Y, where:
- **a** is a scalar,
- **X** and **Y** are **N**-dimensional vectors, with the same value for all entries (**Xi**, **Yi** fixed for all **i**),
- **D** is the resulting vector.

## Compiling the codes
To compile the codes and obtain the executable file, use the `gcc` compiler in the following way:

```bash
gcc vectorlinearcombination_typeofmemoryallocation.c -o nameofexecutable
```
The `-o` option allows to specify the name of the executable file.

## Structure of the codes
- The user is asked to provide the constant values of **a**, **Xi**, **Yi** and the dimensionality **N** of the vectors from the command line, as shown in the following:
  ```bash
  ./nameofexecutable a Xi Yi N
  ```
- If the syntax is incorrect, the program prints:
  ```bash
    Usage: ./nameofexecutable a Xi Yi N.
  ```
- Instructions are printed at runtime.
  
- The **X** and **Y** vectors are created either using static allocation (in `vectorlinearcombination_static.c`) or dynamic allocation (in `vectorlinearcombination_dynamic.c`).
  
- In the static version, the computation is performed directly in `main`.
  
- In the dynamic version, a function `VectorLinearCombination(a, X, Y, N)` is used, which allocates memory dynamically and computes **D**.
  
- A check that the programs successfully computed the result vector is implemented through the function `CheckCorrectness()`: the expected result for every entry is asked as input from the user and then compared to the resulting vector **D** entry values, checking as a first step if they are exactly equal (`==`) and as a second step if the absolute value of their difference is greater than a certain threshold. If component `k` is different from the expected outcome, an error message will be printed out:
  ```bash
  Component number k is wrong
  ```
and the wrong result will be printed out.
- When memory is dynamically allocated, it is then freed using `free()`, after the code's task is accomplished.

## Running the codes for different dimensionalities
  A performance and stability test can be performed by running the program with increasing values of **N**, for example:
 ```bash
 N = 10
 N = 10^6
 N = 10^8
```
This allows us to observe the impact of memory allocation size on program behavior.

In the case of static allocation (`vectorlinearcombination_static.c`), large values of **N** may lead to a 
```bash
  segmentation fault (core dumped)
```
This happens because the arrays are allocated on the stack, which has a limited size. When the requested memory exceeds the stack limit, a stack overflow occurs, causing the program to crash.

In contrast, the dynamic allocation version avoids this issue by allocating memory on the heap, which is significantly larger and managed at runtime. However, memory must be explicitly freed using free() to avoid leaks.
   
## Running the codes for floating numbers
   A numerical accuracy test can be performed using values such as:
   ```bash
     a = 3, Xi = 0.1, Yi = 7.1
   ```
   The expected result for each component is `Di = 7.4`. It is possible to verify that each component is correct using the program’s internal check, whose logic was discussed above. Floating-point arithmetic is not exact due to binary representation limitations, in fact the outcome reads as
```bash
    Di = 7.399999999999999
```
The first check is not passed since the expected result, manually defined, and the `Di` component, evaluated as `a · Xi + Yi`, have slightly different machine representations and turn out to not be exactly equal. 

The second check instead is passed, since the two values are equal within the chosen threshold (`10^-6`).
