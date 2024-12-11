#include "pi.h"
#include <stdlib.h>
#include <stdio.h>

int main()
  {
    int choice;
    int n=0;
    double eps=0;
    
    printf("Veuillez choisir la formule par laquelle pi va être approximé : 1. Liebniz   2. Machin\n");
    scanf("%d", &choice);

    switch (choice){
      case 1:
        printf("Choissisez la précision de pi souhaitée : ");
        scanf("%lf", &eps);
        printf("Pi ≈ %.15f\n", pi_leibniz(eps, &n));
      break;

      case 2:
        printf("Choissisez la précision de pi souhaitée : ");
        scanf("%lf", &eps);
        printf("Pi ≈ %.15f\n", pi_machin(eps, &n));
      break;

      case 3:
        printf("Pi ≈ %f\n", pi_ramanujan(n));
      break;
    }

    return EXIT_SUCCESS ;
  }
