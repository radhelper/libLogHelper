 /*log_helper.i */
%module log_helper
// Make log_helper.cxx include this header:
%{
#include "log_helper.h"
%}

// Make SWIG look into this header:
%include "log_helper.h"
