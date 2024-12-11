/** TP Séries - Compression d'un signal audio
  * Programme principal
  * @author Romain DUJOL
  */

#include <stdio.h>
#include <stdlib.h>
#include "compress.h"

int main()
  {
    { /* Voix 1, stratégie d'élagage des hautes fréquences */
      size_t samples = 9136 ; double rate = 0.95 ;
      double err1 = compress("voix1.wav", "_truncate", TRUNCATE, &samples, &rate, 1) ;
      printf("%ld échantillons, taux réel %f => erreur : %e\n", samples, rate, err1) ;
    }
    { /* Voix 1, stratégie d'élagage des petits coefficients */
      size_t samples = 9136 ; double rate = 0.95 ;
      double err1 = compress("voix1.wav", "_bests", BESTS, &samples, &rate, 1) ;
      printf("%ld échantillons, taux réel %f => erreur : %e\n", samples, rate, err1) ;
    }

    { /* Voix 2, stratégie d'élagage des hautes fréquences */
      size_t samples = 9136 ; double rate = 0.95 ;
      double err2 = compress("voix2.wav", "_truncate", TRUNCATE, &samples, &rate, 1) ;
      printf("%ld échantillons, taux réel %f => erreur : %e\n", samples, rate, err2) ;
    }
    { /* Voix 2, stratégie d'élagage des petits coefficients */
      size_t samples = 9136 ; double rate = 0.95 ;
      double err2 = compress("voix2.wav", "_bests", BESTS, &samples, &rate, 1) ;
      printf("%ld échantillons, taux réel %f => erreur : %e\n", samples, rate, err2) ;
    }
    
    return EXIT_SUCCESS ;
  }
