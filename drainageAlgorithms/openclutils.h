#ifndef OPENCL_UTILS_H_
#define OPENCL_UTILS_H_

#include <CL/cl.h>
const char *err_code (cl_int err_in);
void check_error(cl_int err, const char *operation, const char *filename, int line);
char * loadKernel(const char *filename);

#define checkError(E, S) check_error(E, S, __FILE__, __LINE__)
#endif
