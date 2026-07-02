#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "config_parser.h"

/* Removes initial and final blank spaces from a string, in place. */
static char *trim(char *s)
{
    /* initial blank spaces */
    while (isspace((unsigned char)*s)) s++;

    if (*s == '\0') return s;

    /* final blank spaces */
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
    return s;
}

int config_load(const char *filename, ConfigTable *table)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "config_parser: failed in opening the file '%s'\n", filename);
        return -1;
    }

    table->n_entries = 0;
    char line[CONFIG_MAX_LINE];

    while (fgets(line, sizeof(line), fp) != NULL) {

        char *trimmed = trim(line);

        /* empty row or comment -> ignored */
        if (trimmed[0] == '\0' || trimmed[0] == '#' || trimmed[0] == ';') {
            continue;
        }

        /* look for the separator '=' */
        char *eq = strchr(trimmed, '=');
        if (eq == NULL) {
            /* corrupted line, skip it */
            continue;
        }

        *eq = '\0';
        char *key_part   = trim(trimmed);
        char *value_part = trim(eq + 1);

        /* comment that may be there after the value of interest (e.g. "N = 100 # righe") */
        char *hash = strchr(value_part, '#');
        if (hash != NULL) {
            *hash = '\0';
            value_part = trim(value_part);
        }

        if (table->n_entries >= CONFIG_MAX_ENTRIES) {
            fprintf(stderr, "config_parser: too many keys in the file (max %d)\n", CONFIG_MAX_ENTRIES);
            break;
        }

        ConfigEntry *entry = &table->entries[table->n_entries];
        strncpy(entry->key, key_part, CONFIG_MAX_KEY - 1);
        entry->key[CONFIG_MAX_KEY - 1] = '\0';
        strncpy(entry->value, value_part, CONFIG_MAX_VALUE - 1);
        entry->value[CONFIG_MAX_VALUE - 1] = '\0';

        table->n_entries++;
    }

    fclose(fp);
    return 0;
}

const char *config_get(const ConfigTable *table, const char *key)
{
    for (int i = 0; i < table->n_entries; i++) {
        if (strcmp(table->entries[i].key, key) == 0) {
            return table->entries[i].value;
        }
    }
    return NULL;
}

double config_get_double(const ConfigTable *table, const char *key)
{
    const char *v = config_get(table, key);
    return strtod(v, NULL);
}

long int config_get_long(const ConfigTable *table, const char *key)
{
    const char *v = config_get(table, key);
    return strtol(v, NULL, 10);
}

int config_get_int(const ConfigTable *table, const char *key)
{
    const char *v = config_get(table, key);
    return (int) strtol(v, NULL, 10);
}

void config_get_string(const ConfigTable *table, const char *key,
                        char *out, size_t out_size)
{
    const char *v = config_get(table, key);
    strncpy(out, v, out_size - 1);
    out[out_size - 1] = '\0';
}
