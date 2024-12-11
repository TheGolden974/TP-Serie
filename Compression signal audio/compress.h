/** Compression audio
  * @author Romain DUJOL
  */

#ifndef _COMPRESS_H
#define _COMPRESS_H

/** Type de méthode de compression utilisée
  * @param NONE     : n'élague rien
  *        TRUNCATE : élague les derniers coefficients (liés aux hautes fréquences)
  *        BESTS    : élague les plus petits coefficients
  */
typedef enum {NONE, TRUNCATE, BESTS} pad_method ;

/** Compresse un fichier audio WAV
  * @param wavfile le fichier WAV à compresser
  * @param suffix  suffixe à ajouter au fichier résultat de la compression
  * @param method  méthode de compression
  * @param samples (entrée) le nombre d'échantillons à traiter
  *                (sortie) le nombre d'échantillons traités
  * @param rate    (entrée) le taux de compression minimal désiré
  *                (sortie) le taux de compression réel
  * @param verbose mode "bavard" ou non
  * @return l'erreur quadratique entre le son original et le son compressé
  * @return -1 si le fichier n'a pas pu être ouvert
  * @return -2 si les fichiers n'ont pas pu être créés
  */
double compress(const char *wavfile, const char *suffix,
                const pad_method method, size_t *samples, double *rate,
                const int verbose) ;

#endif // _COMPRESS_H

