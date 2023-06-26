#ifndef MATHSTR_H_INCLUDED
#define MATHSTR_H_INCLUDED

#define MATHSTR_NO_ERROR 0

/*
 * Evaluate a mathematical string for the answer
 *
 * mathstr: a mathematical string. E.X: "(5*3 - 2)^3 - sin(5)"
 * out_answer: returns the answer to the math string via pointer
 *
 * Returns 0 if there were no errors
 * Returns a negative integer if an error occurred
 */
int mathstr_eval(const char *mathstr, double *out_answer);

#endif // MATHSTR_H_INCLUDED
