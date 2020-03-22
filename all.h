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
