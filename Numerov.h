//------------------------------------------------------------------------------
// File:    Numerov.h
// author:  Ivan Ramos Pagnossin (irpagnossin@hotmail.com)
// date:    2006.12.09
//------------------------------------------------------------------------------
#ifndef NUMEROV
#define NUMEROV

#include <math>

void Numerov( unsigned const int, const double *, double *, double (*)(double) );
#endif
