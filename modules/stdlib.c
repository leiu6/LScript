#include "stdlib.h"

#include <math.h>

#include "../lib/runtime-module.h"

/* Math Functions */
double stdlib_sin(int argc, double *argv) {
    return sin(argv[0]);
}

double stdlib_cos(int argc, double *argv) {
    return cos(argv[0]);
}

double stdlib_tan(int argc, double *argv) {
    return tan(argv[0]);
}

double stdlib_asin(int argc, double *argv) {
    return asin(argv[0]);
}

double stdlib_acos(int argc, double *argv) {
    return acos(argv[0]);
}

double stdlib_atan(int argc, double *argv) {
    return atan(argv[0]);
}

double stdlib_log(int argc, double *argv) {
    return log(argv[0]);
}

double stdlib_log10(int argc, double *argv) {
    return log10(argv[0]);
}

double stdlib_log2(int argc, double *argv) {
    return log2(argv[0]);
}

double stdlib_pi(int argc, double *argv) {
    return 3.14159265359;
}

double stdlib_exp(int argc, double *argv) {
    return exp(argv[0]);
}

double stdlib_max(int argc, double *argv) {
    return argv[0] > argv[1] ? argv[0] : argv[1];
}

double stdlib_min(int argc, double *argv) {
    return argv[0] < argv[1] ? argv[0] : argv[1];
}

double stdlib_sqrt(int argc, double *argv) {
    return sqrt(argv[0]);
}

struct runtime_method_def methods[] = {
    {"sin", stdlib_sin, 1},
    {"cos", stdlib_cos, 1},
    {"tan", stdlib_tan, 1},
    {"asin", stdlib_asin, 1},
    {"acos", stdlib_acos, 1},
    {"atan", stdlib_atan, 1},
    {"log", stdlib_log, 1},
    {"ln", stdlib_log, 1},
    {"log10", stdlib_log10, 1},
    {"log2", stdlib_log2, 1},
    {"pi", stdlib_pi, 0},
    {"exp", stdlib_exp, 1},
    {"max", stdlib_max, 2},
    {"min", stdlib_min, 2},
    {"sqrt", stdlib_sqrt, 1}
};

struct runtime_module_def __module_def__ = (struct runtime_module_def){
    "stdlib", 
    "The standard library for Mathstr", 
    runtime_module_autocount(methods),
    methods
    };
