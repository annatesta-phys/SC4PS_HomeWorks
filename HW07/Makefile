CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -Iinclude
LDLIBS  = -lm

SRC_DIR       = src
BIN_DIR       = bin
OUT_DIR       = output
PLOT_DIR      = plots
MPLCONFIG_DIR = .mplconfig

# All the C src files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# One executable for every src file
BINS = $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%)

all: $(BINS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(PLOT_DIR):
	mkdir -p $(PLOT_DIR)

$(MPLCONFIG_DIR):
	mkdir -p $(MPLCONFIG_DIR)

# Compiles and executes all the executable
run: all | $(OUT_DIR)
	@for exe in $(BINS); do \
		echo "==> Running $$exe"; \
		./$$exe; \
	done

# Plot with Python after regenerating the data
plot-python: run | $(PLOT_DIR) $(MPLCONFIG_DIR)
	MPLCONFIGDIR=$(MPLCONFIG_DIR) python3 scripts/plots.py

clean:
	rm -rf $(BIN_DIR) $(OUT_DIR) $(PLOT_DIR) $(MPLCONFIG_DIR)

.PHONY: all run clean plot-python
