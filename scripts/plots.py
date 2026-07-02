#!/usr/bin/env python3

from __future__ import annotations

import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUTPUT_DIR = ROOT / "output"
PLOT_DIR = ROOT / "plots"

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import numpy as np


def read_csv_columns(path: Path):
    """Read coin_tosses.csv, which has two columns:
    the toss index n, and the running head fraction n_heads/n."""
    x_var = []
    y_var = []

    with path.open(newline="", encoding="utf-8") as handle:
        reader = csv.reader(handle)
        next(reader)  # skip header row

        for row in reader:
            x_var.append(float(row[0]))
            y_var.append(float(row[1]))

    return x_var, y_var


def plot_head_fraction() -> None:
    n_values, head_fraction = read_csv_columns(OUTPUT_DIR / "coin_tosses.csv")

    fig, ax = plt.subplots(figsize=(10, 5), constrained_layout=True)

    ax.plot(n_values, head_fraction, label="n_heads / n",
             linewidth=1.2, marker='o', markersize=2)
    ax.axhline(0.5, color="gray", linestyle="--", linewidth=1,
               label="expected value (0.5)")

    ax.set_title("Coin tosses: running head fraction vs number of tosses")
    ax.set_xlabel("n (number of tosses)")
    ax.set_ylabel("Running head fraction")
    ax.set_xscale("log")
    ax.grid(True, alpha=0.3)
    ax.legend()

    fig.savefig(PLOT_DIR / "head_fraction_vs_n.png", dpi=150)
    plt.close(fig)
    
def plot_pi_error() -> None:
    n_points, pi_error = read_csv_columns(OUTPUT_DIR / "pi_estimate.csv")

    fig, ax = plt.subplots(figsize=(10, 5), constrained_layout=True)

    ax.plot(n_points, pi_error, label="n_points / n",
             linewidth=1.2, marker='o', markersize=2)
             
     # True distribution
    x = np.linspace(100, 100000, 1000)
    f = 1 / (np.sqrt(x))
    
    ax.plot(x, f,
            "r",
            linewidth=2,
            label=r"$\frac{1}{\sqrt{n}}$")
    
    ax.set_title("Pi estimate: absolute error vs number of points")
    ax.set_xlabel("n (number of points)")
    ax.set_ylabel("Absolute error of pi estimate")
    ax.set_xscale("log")
    ax.grid(True, alpha=0.3)
    ax.legend()

    fig.savefig(PLOT_DIR / "error_pi_estimate_vs_n.png", dpi=150)
    plt.close(fig)
    
def plot_Y_Usquared() -> None:
    _, Y = read_csv_columns(OUTPUT_DIR / "U_squared.csv")

    fig, ax = plt.subplots(figsize=(10, 5), constrained_layout=True)

    # Normalized histogram
    ax.hist(Y,
            bins=100,
            density=True,
            alpha=0.6,
            label="Simulation")

    # True distribution
    x = np.linspace(1e-3, 1, 1000)
    f = 1 / (2 * np.sqrt(x))

    ax.plot(x, f,
            "r",
            linewidth=2,
            label=r"$f_Y(y)=\frac{1}{2\sqrt{y}}$")

    ax.set_title(r"Distribution of $Y=U^2$")
    ax.set_xlabel(r"$Y$")
    ax.set_ylabel("Probability density")
    ax.grid(True, alpha=0.3)
    ax.legend()

    fig.savefig(PLOT_DIR / "U_squared.png", dpi=150)
    plt.close(fig)
    
def plot_inverse_exp() -> None:
    Y, CDF = read_csv_columns(OUTPUT_DIR / "inverse_exp.csv")

    # --- First figure: histogram of Y vs true density ---
    fig, ax = plt.subplots(figsize=(10, 5), constrained_layout=True)

    ax.hist(Y,
            bins=100,
            density=True,
            alpha=0.6,
            label="Simulation")

    x = np.linspace(0, 5, 1000)
    f = 1.5*np.exp(-1.5*x)

    ax.plot(x, f,
            "r",
            linewidth=2,
            label=r"$f_Y(y)=\lambda e^{-\lambda y}$")

    ax.set_title(r"Distribution of $Y=-\frac{1}{\lambda} ln(1-U)$")
    ax.set_xlabel(r"$Y$")
    ax.set_ylabel("Probability density")
    ax.grid(True, alpha=0.3)
    ax.legend()

    fig.savefig(PLOT_DIR / "inverse_exp.png", dpi=150)
    plt.close(fig)

    # --- Second figure: histogram of CDF vs true CDF ---
    fig, ax = plt.subplots(figsize=(10, 5), constrained_layout=True)

    # CDF empirica: Y (ordinati) sull'asse x, rango normalizzato sull'asse y
    ax.plot(Y, CDF, linewidth=1.5, label="Empirical CDF")

    # CDF teorica, valutata sugli stessi punti Y
    F_theoretical = 1 - np.exp(-1.5 * np.array(Y))
    ax.plot(Y, F_theoretical, "r--", linewidth=2,
            label=r"$F_Y(y)=1-e^{-\lambda y}$")

    ax.set_title(r"CDF of $Y=-\frac{1}{\lambda}\ln(1-U)$")
    ax.set_xlabel(r"$Y$")
    ax.set_ylabel(r"$F_Y(y)$")
    ax.grid(True, alpha=0.3)
    ax.legend()

    fig.savefig(PLOT_DIR / "inverse_exp_cdf.png", dpi=150)
    plt.close(fig)

def main() -> None:
    """Create the output directory and generate the head-fraction figure."""
    PLOT_DIR.mkdir(exist_ok=True)
    plot_head_fraction()
    plot_pi_error()
    plot_Y_Usquared()
    plot_inverse_exp()


if __name__ == "__main__":
    main()
