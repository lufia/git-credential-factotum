#include <u.h>
#include <libc.h>
#include <bio.h>
#include "all.h"

char *
estrdup(char *s)
{
	char *t;

	t = strdup(s);
	if(t == nil)
		sysfatal("strdup failed: %r");
	return t;
}

Params *
parseparams(int fd)
{
	Params *p;
	Biobuf b;
	char *s;

	p = malloc(sizeof *p);
	if(p == nil)
		return nil;
	memset(p, 0, sizeof *p);

	Binit(&b, fd, OREAD);
	while((s=Brdline(&b, '\n')) != nil){
		s[Blinelen(&b)-1] = '\0';
		if(strcmp(s, "") == 0)
			break;
		if(strncmp(s, "protocol=", 9) == 0)
			p->protocol = estrdup(s+9);
		else if(strncmp(s, "host=", 5) == 0)
			p->host = estrdup(s+5);
		else if(strncmp(s, "username=", 9) == 0)
			p->username = estrdup(s+9);
		else if(strncmp(s, "password=", 9) == 0)
			p->password = estrdup(s+9);
	}
	return p;
}

void
freeparams(Params *p)
{
	if(p == nil)
		return;
	free(p->protocol);
	free(p->host);
	free(p->username);
	free(p->password);
}
