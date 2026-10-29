
#ifndef VITRUVIO_UTILS_H
#define VITRUVIO_UTILS_H

//void generate_filename(char *out, int maxlen, const char *name_exp, const char *det_name, const char *timestamp, int step);
void generate_filename(char *fullpath, int maxlen_path, char *filename, int maxlen_name, const char *base_path, const char *name_exp, const char *det_name, const char *timestamp, int step);

//void generate_filename_with_time(char *out, int maxlen, const char *name_exp, const char *det_name, int step);
void generate_filename_with_time(char *fullpath, int maxlen_path, char *filename, int maxlen_name, const char *base_path, const char *name_exp, const char *det_name, int step);

void generate_general_filename(char *fullpath, int maxlen_path, char *filename, int maxlen_name, const char *base_path, const char *name_exp);

#endif
