/int seed;/c\
time_t seed;
/long seed;/c\
time_t seed;

s/sprintf_/sprintf/g

/extern .* time(/d
/extern .* system(/d
/extern .* isatty(/d
/extern .* sprintf(/d
/extern .* snprintf(/d
/extern .* random(/d
/extern .* srandom(/d

/p2c.h/a\
#include <time.h>
