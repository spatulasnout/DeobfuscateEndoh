#ifdef _WIN32

#ifdef __cplusplus
extern "C" {
#endif

extern int _fprintf_w32 (FILE* fp, const char* format, ...);
extern int _fputs_w32(FILE* fp, const char* s);

#ifdef __cplusplus
} // extern "C"
#endif

# define fprintf(...) _fprintf_w32(__VA_ARGS__)
# define printf(...) _fprintf_w32(stdout, __VA_ARGS__)
# define fputs(fp, x) _fputs_w32(fp, x);
# define puts(x) _fputs_w32(stdout, x);
#endif
