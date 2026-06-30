#!/usr/bin/env python3

"""
Plot the sampling-demonstration data with matplotlib.

This script is intentionally simple and beginner-friendly:

- it reads plain CSV files produced by the C programs
- it creates the same figures as the gnuplot workflow
- it demonstrates a minimal reproducible plotting script in Python

Because the figures are generated from files rather than from in-memory objects,
the plotting step is fully decoupled from the numerical simulation step. That is
often a good habit in research workflows.
"""

from __future__ import annotations

import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUTPUT_DIR = ROOT / "output"
PLOT_DIR = ROOT / "plots"

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt


def read_csv_columns(path: Path):
    l_values = []
    x_values = []
    P_forward = []
    P_reference = []
    P_backward = []
    abserr_forward = []
    relerr_forward = []
    abserr_backward = []
    relerr_backward = []

    with path.open(newline="", encoding="utf-8") as handle:
        reader = csv.reader(handle)
        next(reader)

        for row in reader:
            l_values.append(float(row[0]))
            x_values.append(float(row[1]))
            P_forward.append(float(row[2]))
            P_reference.append(float(row[3]))
            P_backward.append(float(row[4]))
            abserr_forward.append(float(row[5]))
            relerr_forward.append(float(row[6]))
            abserr_backward.append(float(row[7]))
            relerr_backward.append(float(row[8]))

    return l_values, x_values, P_forward, P_reference, P_backward, abserr_forward, relerr_forward, abserr_backward, relerr_backward


def plot_relative_errors() -> None:
    cases = [
        ("data_legendre_x0.csv", "x = 0.1"),
        ("data_legendre_x1.csv", "x = 0.5"),
        ("data_legendre_x2.csv", "x = 0.9"),
        ("data_legendre_x3.csv", "x = 0.99"),
    ]

    fig, axes = plt.subplots(4, 1, figsize=(14, 10), constrained_layout=True)

    for ax, (filename, title) in zip(axes, cases):

        l_values, x_values, P_forward, P_reference, P_backward, abserr_forward, relerr_forward, abserr_backward, relerr_backward = read_csv_columns(OUTPUT_DIR / filename)

        ax.plot(l_values, relerr_forward, label="forward recurrence", linewidth=1.8, marker='o', markersize=3)

        ax.plot(l_values, relerr_backward, label="backward recurrence", linewidth=1.8, marker='o', markersize=3)

        ax.set_title(title)
        ax.set_xlabel("l")
        ax.set_ylabel("Relative error")
        ax.set_yscale("log")
        ax.grid(True, alpha=0.3)
        ax.legend()

    fig.savefig(PLOT_DIR / "relative_errors_comparison.png", dpi=150)
    plt.close(fig)
    
def plot_absolute_errors() -> None:
    cases = [
        ("data_legendre_x0.csv", "x = 0.1"),
        ("data_legendre_x1.csv", "x = 0.5"),
        ("data_legendre_x2.csv", "x = 0.9"),
        ("data_legendre_x3.csv", "x = 0.99"),
    ]

    fig, axes = plt.subplots(4, 1, figsize=(14, 10), constrained_layout=True)

    for ax, (filename, title) in zip(axes, cases):

        l_values, x_values, P_forward, P_reference, P_backward, abserr_forward, relerr_forward, abserr_backward, relerr_backward = read_csv_columns(OUTPUT_DIR / filename)

        ax.plot(l_values, abserr_forward, label="forward recurrence", linewidth=1.8, marker='o', markersize=3)

        ax.plot(l_values, abserr_backward, label="backward recurrence", linewidth=1.8, marker='o', markersize=3)

        ax.set_title(title)
        ax.set_xlabel("l")
        ax.set_ylabel("Absolute error")
        ax.set_yscale("log")
        ax.grid(True, alpha=0.3)
        ax.legend()

    fig.savefig(PLOT_DIR / "absolute_errors_comparison.png", dpi=150)
    plt.close(fig)
    
def plot_Legendre_poly() -> None:
    cases = [
        ("data_legendre_x0.csv", "x = 0.1"),
        ("data_legendre_x1.csv", "x = 0.5"),
        ("data_legendre_x2.csv", "x = 0.9"),
        ("data_legendre_x3.csv", "x = 0.99"),
    ]

    fig, axes = plt.subplots(4, 1, figsize=(14, 10), constrained_layout=True)

    for ax, (filename, title) in zip(axes, cases):

        l_values, x_values, P_forward, P_reference, P_backward, abserr_forward, relerr_forward, abserr_backward, relerr_backward = read_csv_columns(OUTPUT_DIR / filename)

        ax.plot(l_values, P_forward, label="forward recurrence", linewidth=1.8, marker='o', markersize=3)

        ax.set_title(title)
        ax.set_xlabel("l")
        ax.set_ylabel("Pl(x)")
        ax.set_ylim(-0.5,1.2)
        ax.grid(True, alpha=0.3)
        ax.legend()

    fig.savefig(PLOT_DIR / "Legendre_poly.png", dpi=150)
    plt.close(fig)
    
def main() -> None:
    """Create the output directory and generate both sampling figures."""
    PLOT_DIR.mkdir(exist_ok=True)
    plot_relative_errors()
    plot_Legendre_poly()
    plot_absolute_errors()


if __name__ == "__main__":
    main()
