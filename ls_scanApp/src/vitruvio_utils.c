#include <stdio.h>
#include <time.h>
#include "vitruvio_utils.h"

//void generate_filename(char *out, int maxlen, const char *name_exp, const char *det_name, const char *timestamp, int step)
//{
//	snprintf(out, maxlen, "scan_%s_%s_%s_step%04d.dat", name_exp, det_name, timestamp, step);
//}
void generate_filename(char *fullpath, int maxlen_path, char *filename, int maxlen_name, const char *base_path, const char *name_exp, const char *det_name, const char *timestamp, int step)
{
    /* solo nome file */
    snprintf(filename, maxlen_name, "ls_%s_%s_%s_step%04d.dat", name_exp, det_name, timestamp, step);

    /* path completo */
    snprintf(fullpath, maxlen_path, "%s/%s", base_path, filename);
}


//void generate_filename_with_time(char *out, int maxlen, const char *name_exp, const char *det_name, int step)
//{
//    char timestamp[64];
//    time_t now;
//    struct tm *tm_info;
//
//    time(&now);
//    tm_info = localtime(&now);
//    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", tm_info);
//
//    snprintf(out, maxlen, "scan_%s_%s_%s_step%04d.dat", name_exp, det_name, timestamp, step);
//}
void generate_filename_with_time(char *fullpath, int maxlen_path, char *filename, int maxlen_name, const char *base_path, const char *name_exp, const char *det_name, int step)
{
    char timestamp[64];
    time_t now;
    struct tm *tm_info;

    time(&now);
    tm_info = localtime(&now);
//    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", tm_info);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", tm_info);

    /* solo nome file */
//    snprintf(filename, maxlen_name, "scan_%s_%s_%s_step%04d.dat", name_exp, det_name, timestamp, step);
//
//	Here the problem is the lenght of the file out string... should be greater than 40 characters
//
//    snprintf(filename, maxlen_name, "scan_%s_%s_%s_step%04d.dat", timestamp, name_exp, det_name, step);
    snprintf(filename, maxlen_name, "%s%s_%04d.dat", timestamp, det_name, step);

    /* path completo */
    snprintf(fullpath, maxlen_path, "%s/%s", base_path, filename);
}

void generate_general_filename(char *fullpath, int maxlen_path, char *filename, int maxlen_name, const char *base_path, const char *name_exp)
{
    char timestamp[64];
    time_t now;
    struct tm *tm_info;

    time(&now);
    tm_info = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", tm_info);

    /* solo nome file */
    snprintf(filename, maxlen_name, "ls_%s_%s.dat", timestamp, name_exp);

    /* path completo */
    snprintf(fullpath, maxlen_path, "%s/%s", base_path, filename);
}

