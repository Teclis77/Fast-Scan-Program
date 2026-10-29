/*
 * lissajous_utils.c
 *
 * Funzioni C per:
 *   - Generazione path di Lissajous con quantizzazione e decimazione
 *   - Auto-calibrazione del parametro skip per target di punti
 *   - Simulazione rivelatore con gaussiana + rumore
 *
 * Compilare e linkare all'IOC EPICS insieme a lissajous_test.stt
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lissajous_utils.h"

/* ─────────────────────────────────────────────────────────
 *  generate_lissajous_path()
 * ───────────────────────────────────────────────────────── */
int generate_lissajous_path(double xmin, double xmax,
                             double ymin, double ymax,
                             double xstep, double ystep,
                             double a_freq, double b_freq,
                             double delta,
                             int skip,
                             double *x_coords, double *y_coords,
                             int max_points)
{
    double cx, cy, Ax, Ay;
    double avg_radius, min_step, circumference;
    int    oversample, n_samples;
    double dt;
    int    n, s;
    double x_prev, y_prev;

    if (xmax <= xmin || ymax <= ymin ||
        xstep <= 0.0 || ystep <= 0.0 ||
        a_freq <= 0.0 || b_freq <= 0.0) {
        printf("ERROR generate_lissajous_path: parametri non validi\n");
        return -1;
    }
    if (skip < 1) skip = 1;

    /* Centro e ampiezze */
    cx = (xmax + xmin) / 2.0;
    cy = (ymax + ymin) / 2.0;
    Ax = (xmax - xmin) / 2.0;
    Ay = (ymax - ymin) / 2.0;

    /*
     * Calcola numero campioni base:
     * abbastanza da campionare ogni cella della griglia
     * piu' un fattore di oversample per non perdere transizioni rapide
     */
    min_step      = (xstep < ystep) ? xstep : ystep;
    avg_radius    = (Ax + Ay) / 2.0;
    circumference = 2.0 * M_PI * avg_radius;
    oversample    = 20;
    n_samples     = (int)(circumference / min_step * oversample * a_freq);

    if (n_samples < 2000)   n_samples = 2000;
    if (n_samples > 500000) n_samples = 500000;

    dt     = 2.0 * M_PI / (double)n_samples;
    n      = 0;
    x_prev = 1e38;
    y_prev = 1e38;

    for (s = 0; s <= n_samples; s++) {

        double t, x_raw, y_raw, x_q, y_q;

        /* Decimazione: prende 1 campione ogni skip */
        if ((s % skip) != 0) continue;

        t = s * dt;

        /* Curve di Lissajous:
         *   x(t) = cx + Ax * sin(a*t + delta)
         *   y(t) = cy + Ay * sin(b*t)
         */
        x_raw = cx + Ax * sin(a_freq * t + delta);
        y_raw = cy + Ay * sin(b_freq * t);

        /* Quantizza alla griglia xstep x ystep */
        x_q = round(x_raw / xstep) * xstep;
        y_q = round(y_raw / ystep) * ystep;

        /* Clamp nel range (gestisce arrotondamenti ai bordi) */
        if (x_q < xmin) x_q = xmin;
        if (x_q > xmax) x_q = xmax;
        if (y_q < ymin) y_q = ymin;
        if (y_q > ymax) y_q = ymax;

        /* Salta duplicati consecutivi (stessa cella della griglia) */
        if (fabs(x_q - x_prev) < xstep * 0.01 &&
            fabs(y_q - y_prev) < ystep * 0.01) {
            continue;
        }

        /* Controlla capienza array */
        if (n >= max_points) {
            printf("WARN generate_lissajous_path: "
                   "max_points=%d raggiunto, path troncato a %d punti\n",
                   max_points, n);
            return -1;
        }

        x_coords[n] = x_q;
        y_coords[n] = y_q;
        x_prev = x_q;
        y_prev = y_q;
        n++;
    }

    printf("INFO generate_lissajous_path: %d punti unici  "
           "range=[%.3f,%.3f]x[%.3f,%.3f]  "
           "step=(%.3f,%.3f)  "
           "a=%.0f b=%.0f delta=%.4f  skip=%d\n",
           n, xmin, xmax, ymin, ymax,
           xstep, ystep, a_freq, b_freq, delta, skip);

    return n;
}

/* ─────────────────────────────────────────────────────────
 *  find_lissajous_skip()
 * ───────────────────────────────────────────────────────── */
int find_lissajous_skip(double xmin, double xmax,
                         double ymin, double ymax,
                         double xstep, double ystep,
                         double a_freq, double b_freq,
                         double delta,
                         int target_points,
                         double *x_coords, double *y_coords,
                         int max_points)
{
    int skip = 1;
    int n    = 0;
    int prev_n;

    printf("INFO find_lissajous_skip: cerco skip per ~%d punti\n",
           target_points);

    /*
     * Strategia: cerca il valore di skip minimo tale che
     * n <= target_points. Usa incrementi progressivi per
     * non fare troppe iterazioni su range grandi.
     */
    while (skip <= 10000) {
        prev_n = n;
        n = generate_lissajous_path(
                xmin, xmax, ymin, ymax,
                xstep, ystep,
                a_freq, b_freq, delta,
                skip,
                x_coords, y_coords,
                max_points);

        if (n < 0) {
            /* max_points raggiunto, aumenta skip */
            skip++;
            continue;
        }

        printf("INFO find_lissajous_skip: skip=%d -> %d punti\n",
               skip, n);

        if (n <= target_points) break;

        /*
         * Stima il prossimo skip con interpolazione lineare
         * per convergere piu' velocemente su range grandi
         */
        if (prev_n > 0 && n != prev_n) {
            int estimated = skip * n / target_points;
            if (estimated > skip + 1) {
                skip = estimated;
            } else {
                skip++;
            }
        } else {
            skip++;
        }
    }

    printf("INFO find_lissajous_skip: skip ottimale=%d -> %d punti "
           "(target=%d)\n", skip, n, target_points);

    return skip;
}

/* ─────────────────────────────────────────────────────────
 *  simulate_detector()
 * ───────────────────────────────────────────────────────── */
double simulate_detector(double pos_x, double pos_y,
                          double xmin, double xmax,
                          double ymin, double ymax,
                          double noise_amplitude)
{
    static int initialized = 0;
    double cx, cy, sigma, dx, dy, gauss, noise, value;

    if (!initialized) {
        srand((unsigned int)time(NULL));
        initialized = 1;
    }

    /* Picco gaussiano centrato nell'area di scansione */
    cx    = (xmax + xmin) / 2.0;
    cy    = (ymax + ymin) / 2.0;
    sigma = (xmax - xmin) / 4.0;   /* larghezza ~ 1/4 del range X */
    dx    = pos_x - cx;
    dy    = pos_y - cy;
    gauss = 100.0 * exp(-(dx*dx + dy*dy) / (2.0 * sigma * sigma));

    /* Rumore casuale uniforme [-noise_amplitude, +noise_amplitude] */
    noise = noise_amplitude *
            (2.0 * (double)rand() / (double)RAND_MAX - 1.0);

    value = gauss + noise;
    if (value < 0.0) value = 0.0;

    return value;
}
