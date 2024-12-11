#include <math.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <sndfile.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include "compress.h"

// Comparator function for qsort
int compare(const void *a, const void *b) {
    double diff = fabs(*(double*)a) - fabs(*(double*)b);
    return (diff > 0) - (diff < 0);
}

/** Ajoute un suffixe avant l'extension
  * @param filename nom de fichier (avec extension)
  * @param suffix   suffixe à ajouter
  * @return le nom de fichier avec suffixe et extension
  */
char *add_suffix(const char *filename, const char *suffix)
  {
    size_t name_len = strlen(filename) ;
    size_t sufx_len = strlen(suffix  ) ;
    char *suffixed = (char *) malloc ((name_len + sufx_len) * sizeof(char)) ;

    char  *ext_pos = strrchr(filename, '.') ;
    size_t ext_ind = ext_pos - filename ;

    strncpy(suffixed                     , filename, ext_ind       ) ;
    strncpy(suffixed + ext_ind           , suffix  , strlen(suffix)) ;
    strcpy (suffixed + ext_ind + sufx_len, ext_pos) ;

    return suffixed ;
  }

/** Calcule la plus grande puissance de `base` inférieure ou égale à `n`
  * Renvoie -1 si `base` ou `n` est strictement négatif
  */
size_t highest_power(int base, size_t n)
  {
    if (n    <  0) return -1 ;
    if (base <  0) return -1 ;
    if (base <= 1) return  1 ;

    size_t res = 1 ;
    for (res = 1 ; res * base <= n ; res *= base) ;
    return res ;
  }

/** Calcule la distance euclidienne */
double eucl_dist(int n, double x1[], double x2[])
  {
    double d = 0.0 ;
    for (int i = 0 ; i < n ; i++)
      {
        double dx = x2[i] - x1[i] ;
        d = d + dx * dx ;
      }
    return sqrt(d) ;
  }

/** Élague les coefficients associés aux hautes fréquences
  * @param data    en entrée : tableau contenant les données numériques
  *                en sortie : tableaul élagué
  * @param samples longueur de `data`
  * @param rate    en entrée : le taux de compression minimal désiré
  *                en sortie : le taux de compression réel
  */
void _zero_pad_trunc(double data[], size_t samples, double *rate)
{
    size_t new_samples;
    
    if (*rate > 1.0) {
        // Zero-padding
        new_samples = (size_t)(samples * (*rate));
        if (new_samples > samples) {
            memset(data + samples, 0, (new_samples - samples) * sizeof(double));
        }
    } else if (*rate < 1.0) {
        // Truncating
        new_samples = (size_t)(samples * (*rate));
        if (new_samples < samples) {
            // Truncation is implicit as we just consider fewer samples
        }
    } else {
        // No change
        new_samples = samples;
    }

    // Update the rate to reflect the actual compression rate
    *rate = (double)new_samples / samples;
}

/** Élague les coefficients les plus petits
  * @param data    en entrée : tableau contenant les données numériques
  *                en sortie : tableaul élagué
  * @param samples longueur de `data`
  * @param rate    en entrée : le taux de compression minimal désiré
  *                en sortie : le taux de compression réel
  */
void _zero_pad_bests(double data[], size_t samples, double *rate)
{
    size_t new_samples;
    size_t i;

    if (*rate < 1.0) {
        // Determine the number of samples to keep
        new_samples = (size_t)(samples * (*rate));

        // Create a copy of the data array to sort
        double *sorted_data = (double *)malloc(samples * sizeof(double));
        if (sorted_data == NULL) {
            // Handle memory allocation failure
            return;
        }
        memcpy(sorted_data, data, samples * sizeof(double));

        // Sort the copy by absolute value
        qsort(sorted_data, samples, sizeof(double), compare);

        // Determine the threshold value
        double threshold = fabs(sorted_data[samples - new_samples]);

        // Zero out the smallest coefficients in the original data array
        for (i = 0; i < samples; i++) {
            if (fabs(data[i]) < threshold) {
                data[i] = 0.0;
            }
        }

        // Free the sorted data array
        free(sorted_data);
    } else {
        // No change if rate is 1.0 or greater
        new_samples = samples;
    }

    // Update the rate to reflect the actual compression rate
    *rate = (double)new_samples / samples;
}

/** Élague les données numériques issues de la FFT
  * @param method  méthode utilisée (TRUNCATE ou BESTS)
  * @param data    tableau contenant les données numériques
  * @param samples longueur de `data`
  * @param rate    le taux de compression minimal désiré
  */
void _zero_pad(double data[], pad_method method, size_t samples, double *rate)
  {
    switch (method)
      {
        case NONE     : /* Rien à faire ! */                 ; break ;
        case TRUNCATE : _zero_pad_trunc(data, samples, rate) ; break ;
        case BESTS    : _zero_pad_bests(data, samples, rate) ; break ;
      }
  }

/** Compresse les données numériques
  * @param data    tableau contenant les données numériques
  * @param method  méthode utilisée (TRUNCATE ou BESTS)
  * @param samples longueur de `data`
  * @param rate    le taux de compression minimal désiré
  * @param verbose mode "bavard" ou non
  */
void _compress(double data[], pad_method method, size_t samples, double *rate, int verbose)
  {
    if (verbose) printf("*** Calcul de la FFT...") ;
    gsl_fft_real_radix2_transform(data, 1, samples) ;
    if (verbose) printf(" Fait\n") ;

    if (verbose) printf("*** Élagage du résultat de la FFT...") ;
    _zero_pad(data, method, samples, rate) ;
    if (verbose) printf(" Fait\n") ;

    if (verbose) printf("*** Reconstruction à partir de la version élaguée...") ;
    gsl_fft_halfcomplex_radix2_inverse(data, 1, samples) ;
    if (verbose) printf(" Fait\n") ;
  }

double compress(const char *wavfile, const char *suffix,
                const pad_method method, size_t *samples, double *rate, const int verbose)
  {
    // Ouverture du fichier son et extraction des méta-données
    SF_INFO info = {0, 0, 0, 0, 0, 0} ;
    if (verbose) printf("** Ouverture du fichier %s en cours...\n", wavfile) ;
    SNDFILE *sfd = sf_open(wavfile, SFM_READ, &info) ;
    if (sfd == NULL)
      { printf("%s\n", sf_strerror(sfd)) ; return -1.0 ; }
    else
      {
        printf(" Format : %x, taux d'échantillonage : %d\n", info.format, info.samplerate) ;
        printf(" %d canaux de %ld échantillons, %d sections\n", info.channels, info.frames, info.sections) ;
      }

    // Extraction des données numériques
    size_t data_len = highest_power(2, info.frames) ; // On s'assure que l'on prend un nombre d'échantillons qui est une puissance de 2.
    if (verbose) printf("** Tentative d'extraction des %ld premiers échantillons...", data_len) ;
    double *data = (double *) malloc (data_len * info.channels * sizeof(double)) ;
    sf_count_t cnt = sf_read_double(sfd, data, data_len * info.channels) ;
    if (cnt < *samples) *samples = cnt ;
    if (verbose) printf(" %ld échantillons extraits\n", *samples / info.channels) ;

    double *data_orig = (double *) malloc (*samples * sizeof(double)) ;
    memcpy(data_orig, data, *samples * sizeof(double)) ;

    // Fermeture du fichier
    sf_close(sfd) ;

    // Stockage du fichier réellement à compresser
    info.frames = *samples ;
    if (verbose) printf("** Génération du fichier son à traiter...") ;
    char *croppedfile = add_suffix(wavfile, "_cropped") ;
    SNDFILE *cropped_sfd = sf_open(croppedfile, SFM_WRITE, &info) ;
    if (cropped_sfd == NULL)
      { printf("\n%s\n", sf_strerror(cropped_sfd)) ; return -2.0 ; }
    sf_write_double(cropped_sfd, data, *samples) ;
    sf_close(cropped_sfd) ;
    if (verbose) printf(" Fait\n") ;

    // Compression des données numériques
    if (verbose) printf("** Transformation des données numériques\n") ;
    _compress(data, method, *samples, rate, verbose) ;
    
    // Stockage du fichier compressé
    if (verbose) printf("** Génération du fichier compressé...") ;
    char *compressedfile = add_suffix(wavfile, suffix) ;
    SNDFILE *compressed_sfd = sf_open(compressedfile, SFM_WRITE, &info) ;
    if (compressed_sfd == NULL)
      { printf("\n%s\n", sf_strerror(compressed_sfd)) ; return -2.0 ; }
    sf_write_double(compressed_sfd, data, *samples) ;
    sf_close(compressed_sfd) ;
    if (verbose) printf(" Fait\n") ;

    // Calcul de l'erreur numérique
    double err = eucl_dist(*samples, data_orig, data) ;

    // Libération de la mémoire
    free(data_orig) ;
    free(data) ;

    return err ;
  }
