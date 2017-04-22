/int seed;/c\
time_t seed;
/long seed;/c\
time_t seed;

s/sprintf_/sprintf/g

/extern .* time(/d
/extern .* system(/d
/extern .* isatty(/d
/extern .* snprintf(/d
/extern .* sprintf(/d

/p2c.h/a\
#include <time.h>

/extern long random/c\
#if defined(__MSDOS__) || defined(RAND) \
#undef random\
#define random() rand()\
#else\
extern long random(void);\
#if !defined(RAND_MAX)\
#undef randmax\
#define randmax (double) LONG_MAX\
#endif\
#endif
/extern int random/c\
#if defined(__MSDOS__) || defined(RAND) \
#undef random\
#define random() rand()\
#else\
extern long random(void);\
#if !defined(RAND_MAX)\
#undef randmax\
#define randmax (double) LONG_MAX\
#endif\
#endif

/extern.*srandom/c\
#if defined(_POSIX_SOURCE) || defined(__sun)\
extern void srandom(unsigned s);\
#elif defined(__MSDOS__) || defined(RAND) \
extern void srand(unsigned s);\
#define srandom(x) srand((unsigned)(x))\
#elif defined(mips)\
#elif defined(__APPLE__)\
#elif defined(__OpenBSD__)\
extern void srand(unsigned int s);\
#else\
extern void srandom(int s);\
#endif

/define randmax/a\
#if defined(RAND_MAX)\
#undef randmax\
#define randmax (double) RAND_MAX\
#endif

/typedef.* uchar;/c\
#if (!defined(_POSIX_SOURCE)) || defined(linux)\
typedef unsigned char uchar;\
#endif
