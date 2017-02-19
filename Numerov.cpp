//------------------------------------------------------------------------------
// File:    Numerov.cpp
// author:  Ivan Ramos Pagnossin (irpagnossin@hotmail.com)
// date:    2006.12.09

// - A distância entre dois x sucessivos deve ser constante.
//------------------------------------------------------------------------------
#include <math>
#include <iostream>
#include "Numerov.h"

void Numerov( unsigned const int nPtos, const double *x, double *y, double (*F)(double) ){

    if ( nPtos < 2 ){
        std::cerr << "Numerov solver needs vectors with at least two elements." << std::endl;
        return;
    }

    double  dx   = x[1] - x[0],
            aux1 = 0,
            aux2 = 0,
            aux3 = 0;

    for( register unsigned int i = 2; i <= nPtos-1; i++ ){

        aux1 = 1. - 5./12 * pow(dx,2) * (*F)( x[i-1] );
        aux2 = 1. + 1./12 * pow(dx,2) * (*F)( x[i-2] );
        aux3 = 1. + 1./12 * pow(dx,2) * (*F)( x[i]   );

        y[i] = ( 2 * aux1 * y[i-1] - aux2 * y[i-2] ) / aux3;
    }
};

