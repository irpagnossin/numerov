//------------------------------------------------------------------------------
// File:    shroedinger.cpp
// author:  Ivan Ramos Pagnossin
// date:    2006.12.09
//
//   Example of using the Numerov method to solve the Schrödinger equation for
// the finite quantum-well potential.
//------------------------------------------------------------------------------
#include <math.h>
#include <iostream.h>
#include <fstream>

#include "Numerov.h"

//#define SELF_ENERGY

using std::cout;
using std::endl;

double energia = 0.006076;
//------------------------------------------------------------------------------
// Schrödinger's equation: y'' = F(x)y, being F(x) = 2m/hbar^2 [V(x)-E].
//------------------------------------------------------------------------------
double schroedinger( double x0 ){

    const double mEff  = 0.067,
                 hbar2 = 0.076199682 * mEff;

    double V = ( x0 > -70 && x0 < 70? 0 : 0.399 );

    return( (2 * mEff / hbar2) * (energia-V) );
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int main( int argc, char *argv[] ){

    int nPtos = 10000;

    double xI  = -200,
           xF  = +200;

    double dx = (xF-xI)/(nPtos-1), next_x;
                                   
    //------------------------------------
    // Abre o arquivo para escrita.
    //------------------------------------
    if ( argc != 2 ){
        cout << "usage: NumerovExample <output.dat>" << endl;
        return( -1 );
    }

    std::ofstream arquivo ( argv[1] );

    if( !arquivo.is_open() ){
        cout << "unable to open the output file " << argv[1] << "." << endl;
        return( -1 );
    }

    //------------------------------------
    // Monta os vetores *xE e *xD.
    //------------------------------------
    int ptosComuns = 3;
    if( nPtos < ptosComuns ){
        if( nPtos % 2 != 0 ) ptosComuns = nPtos;
        else ptosComuns = nPtos-1;
    }

    int meio = (nPtos-1) >> 1;

    ptosComuns = ptosComuns >> 1;
    if( ptosComuns == 0 ) ptosComuns = 1;

    const int nPtos_E =     1 + meio + ptosComuns,
              nPtos_D = nPtos - meio + ptosComuns;

    double *xE = new double[nPtos_E],
           *xD = new double[nPtos_D];

    for( register int i = 0; i <= nPtos-1; i++ ){

        next_x = xI + i*dx;

	    if( i <= meio+ptosComuns ) xE[i] = next_x;
	    if( i >= meio-ptosComuns ) xD[nPtos-1-i] = next_x;

    }

    //------------------------------------
    // Resolve a eq. de Schrödinger.
    //------------------------------------
    double *yE = new double[nPtos],
           *yD = new double[nPtos];

    for( register int i = 0; i <= nPtos-1; i++ ){
        yE[i] = yD[i] = 0;
    }

    yE[1] = yD[1] = 1e-5;

    #ifdef SELF_ENERGY
    const double E_i = 1e-3, E_f = 0.4;
    const int N = 100;

    for( register int iEnergy = 0; iEnergy <= N-1; iEnergy++ ){

        energia = E_i + iEnergy * ( E_f - E_i ) / ( N - 1 );
        cout << energia << endl;
    #endif

        Numerov( nPtos_E, xE, yE, schroedinger );
        Numerov( nPtos_D, xD, yD, schroedinger );

    #ifdef SELF_ENERGY
        double avaliacao = fabs(yE[1][meio] / yE[0][meio] - yD[1][meio] / yD[0][meio] );
        arquivo << energia << "\t" << avaliacao << endl;
    }
    #endif

    /*
     * Imprime a solução no arquivo.
     */
    #ifndef SELF_ENERGY
    for( register int i = 0; i <= nPtos-1; i++ ){

        if( i <= meio+ptosComuns ){
            arquivo << xE[i] << "\t" << yE[i] << "\t";
        }

   	    if( i >= meio-ptosComuns ){
            if( i > meio+ptosComuns ){
                arquivo << xD[nPtos-1-i] << "\t\t";
            }
            arquivo << yD[nPtos-1-i];
        }
        arquivo << endl;
    }
    #endif

    arquivo.close();

    return( 0 );
}


