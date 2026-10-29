#ifndef LISSAJOUS_UTILS_H
#define LISSAJOUS_UTILS_H

/*
 * lissajous_utils.h
 * Funzioni C per generazione path Lissajous e rivelatore simulato.
 * Da linkare all'IOC insieme a lissajous_test.stt
 */

/*
 * generate_lissajous_path()
 *
 * Genera un path di Lissajous nell'area [xmin,xmax] x [ymin,ymax]
 * con coordinate quantizzate a xstep/ystep e decimazione skip.
 *
 * Parametri:
 *   xmin,xmax,ymin,ymax  - range della scansione
 *   xstep, ystep         - passo di quantizzazione
 *   a_freq, b_freq       - rapporto frequenze (es. 3,2 oppure 11,9)
 *   delta                - sfasamento in radianti (es. M_PI/2)
 *   skip                 - prende 1 campione ogni skip (1=tutti)
 *   x_coords, y_coords   - array di output (pre-allocati dal chiamante)
 *   max_points           - dimensione massima array di output
 *
 * Ritorna: numero punti generati, -1 se max_points insufficiente
 */
int generate_lissajous_path(double xmin, double xmax,
                             double ymin, double ymax,
                             double xstep, double ystep,
                             double a_freq, double b_freq,
                             double delta,
                             int skip,
                             double *x_coords, double *y_coords,
                             int max_points);

/*
 * find_lissajous_skip()
 *
 * Trova automaticamente il valore di skip per ottenere
 * circa target_points punti, qualunque sia la configurazione.
 * Chiama generate_lissajous_path() internamente con skip crescenti.
 *
 * Ritorna: valore ottimale di skip
 */
int find_lissajous_skip(double xmin, double xmax,
                         double ymin, double ymax,
                         double xstep, double ystep,
                         double a_freq, double b_freq,
                         double delta,
                         int target_points,
                         double *x_coords, double *y_coords,
                         int max_points);

/*
 * simulate_detector()
 *
 * Simula un rivelatore con picco gaussiano centrato nell'area
 * + rumore casuale. Utile per test senza hardware reale.
 *
 * Parametri:
 *   pos_x, pos_y         - posizione corrente
 *   xmin,xmax,ymin,ymax  - range (serve per calcolare il centro)
 *   noise_amplitude      - ampiezza del rumore (es. 5.0)
 *
 * Ritorna: valore simulato [0, 100+noise]
 */
double simulate_detector(double pos_x, double pos_y,
                          double xmin, double xmax,
                          double ymin, double ymax,
                          double noise_amplitude);

#endif /* LISSAJOUS_UTILS_H */
