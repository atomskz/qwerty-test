#include <stdio.h>

#define print_out(format, ...)           \
fprintf(stdout, format, ## __VA_ARGS__); \
fprintf(stdout, "\n")

#define print_err(format, ...)           \
fprintf(stderr, format, ## __VA_ARGS__); \
fprintf(stderr, "\n")
