CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -Iinclude
LDLIBS  = -lm

SRC_DIR       = src
BIN_DIR       = bin
OUT_DIR       = output
PLOT_DIR      = plots
MPLCONFIG_DIR = .mplconfig

# Tutti i sorgenti C
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Un eseguibile per ogni sorgente
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

# Compila tutti gli eseguibili e li esegue
run: all | $(OUT_DIR)
	@for exe in $(BINS); do \
		echo "==> Running $$exe"; \
		./$$exe; \
	done

# Plot con Python (dopo aver rigenerato i dati)
plot-python: run | $(PLOT_DIR) $(MPLCONFIG_DIR)
	MPLCONFIGDIR=$(MPLCONFIG_DIR) python3 scripts/plots.py

clean:
	rm -rf $(BIN_DIR) $(OUT_DIR) $(PLOT_DIR) $(MPLCONFIG_DIR)

.PHONY: all run clean plot-python
