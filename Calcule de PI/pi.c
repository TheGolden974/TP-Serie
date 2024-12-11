#include "pi.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double pi_leibniz(double eps, int* n)
  {
    double pi = 0 ;
    for (*n = 0; *n <= 2.0/(eps) - (3.0/2.0); (*n)++) {
        pi += 4.0*pow(-1, *n)/(2*(*n)+1);
    }
    
    return pi ;
  }

double pi_machin(double eps, int *n)
  {
    double pi = 0;
    for (*n = 0; *n <= log(16./375.)/log(25.) + log(1./eps)/log(25.); (*n)++) {
        pi += (4. * pow(-1, *n)/(2*(*n)+1))*(4.0*pow(1.0/5.0,2*(*n)+1)-pow(1/239.0,2*(*n)+1));
    }

    return pi ;
  }

double factorial(int n) {
    if (n == 0 || n == 1) {
        return 1.0;
    }
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// FACULTATIF
double pi_ramanujan(int n)
  {
    double pi_inverse = 0 ;
    //for (n = 0; n <= ; n++) {
    //    pi_inverse += ((factorial(4*(n)))/pow(factorial(n),4))*((1103.+26390.*(n))/(pow(4.*99.,4.*(n))));
    //}

    return 1.0/(((2.0*sqrt(2.0))/9701.0)*(pi_inverse)) ;
  }
