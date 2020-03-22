#include <u.h>
#include <libc.h>
#include <thread.h>
#include <auth.h>

typedef struct Cmd Cmd;
struct Cmd {
	char *name;
	void (*run)(int argc, char **argv);
};

void
run_get(int argc, char **argv)
{
	print("get\n");
}

void
run_store(int argc, char **argv)
{
	print("store\n");
}

void
run_erase(int argc, char **argv)
{
	print("rease\n");
}

Cmd commands[] = {
	{ "get", run_get },
	{ "store", run_store },
	{ "erase", run_erase },
	nil,
};

void
usage(void)
{
	fprint(2, "usage: %s {get|store|erase}\n", argv0);
	threadexits(nil);
}

void
threadmain(int argc, char **argv)
{
	Cmd *p;

	ARGBEGIN{
	default:
		usage();
	}ARGEND
	if(argc == 0)
		usage();
	for(p = commands; p->name; p++)
		if(strcmp(*argv, p->name) == 0){
			p->run(argc, argv+1);
			threadexits(nil);
		}
	usage();
}