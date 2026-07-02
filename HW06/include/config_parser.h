#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#define CONFIG_MAX_LINE   256
#define CONFIG_MAX_KEY     64
#define CONFIG_MAX_VALUE  192
#define CONFIG_MAX_ENTRIES 32

/** Read couple key/value from the config file. */
typedef struct {
    char key[CONFIG_MAX_KEY];
    char value[CONFIG_MAX_VALUE];
} ConfigEntry;

/** Collection of the couples key/value read from the file. */
typedef struct {
    ConfigEntry entries[CONFIG_MAX_ENTRIES];
    int n_entries;
} ConfigTable;

/**
 * Read config file with "Key = Value" sintax and populate a ConfigTable
 * return -1 if the file cannot be opened
 */
int config_load(const char *filename, ConfigTable *table);

/**
 * Look for a key in the table and gives its value as a string
 * return NULL if the key is absent
 */
const char *config_get(const ConfigTable *table, const char *key);

/** Same as above, but gives the value as a double/long int/int */
double config_get_double(const ConfigTable *table, const char *key);
long int config_get_long(const ConfigTable *table, const char *key);
int config_get_int(const ConfigTable *table, const char *key);

/**
 * Same as above, but copies the value (string) in a buffer given by the user
 */
void config_get_string(const ConfigTable *table, const char *key,
                        char *out, size_t out_size);

#endif /* CONFIG_PARSER_H */
