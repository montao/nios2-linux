#ifndef __DEBUG_H_
#define __DEBUG_H_
/* ICC DEBUG */
#define DEBUG
#ifdef DEBUG
extern int coreb_debug_level;
void coreb_msg(char *fmt, ...);
#define COREB_DEBUG(level, ...) \
do { \
	if (coreb_debug_level > level) \
		coreb_msg(__VA_ARGS__); \
} while(0)
#else
#define COREB_DEBUG(level, ...) \
do { \
} while(0)
# define coreb_msg(fmt, ...) do {} while (0)
#endif
#endif
