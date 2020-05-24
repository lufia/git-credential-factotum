#include <u.h>
#include <libc.h>

/*
 * The _POSIX_VERSION will be defined by unistd.h.
 * p9p's u.h includes that header.
*/
#ifdef _POSIX_VERSION
#include <libString.h>
#else
#include <String.h>
#endif

typedef struct Params Params;

struct Params {
	char *protocol;
	char *host;
	char *username;
	char *password;
};

extern char *estrdup(char*);
extern Params *parseparams(int);
extern void freeparams(Params*);
extern String *makeString(Params*);

extern void noauth(void);
extern int ctlwrite(char*);
