/** Méthodes de calcul de pi */

#ifndef _PI_H
#define _PI_H

/** Calcule une approximation de pi à l'aide de la formule de LEIBNIZ
  * @param eps précision demandée
  * @param n   (sortie uniquement) ordre de la somme partielle retenue
  * @return approximation de pi
  */
double pi_leibniz(double eps, int *n) ;

/** Calcule une approximation de pi à l'aide de la formule de MACHIN
  * @param eps précision demandée
  * @param n   (sortie uniquement) ordre de la somme partielle retenue
  * @return approximation de pi
  */
double pi_machin(double eps, int *n) ;

/** Calcule une approximation de pi à l'aide de la formule de RAMANUJAN
  * @param n ordre de la somme partielle
  * @return approximation de pi
  */
double pi_ramanujan(int n) ;

#endif // _PI_H
