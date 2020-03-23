#include "all.h"
#include <thread.h>
#include <auth.h>
#include <9pclient.h>

typedef struct Cmd Cmd;
struct Cmd {
	char *name;
	void (*run)(int argc, char **argv);
};

int
dorpc(AuthRpc *rpc, char *verb, char *val)
{
	int ret;

	ret = auth_rpc(rpc, verb, val, strlen(val));
	if(ret < 0)
		sysfatal("auth_rpc: %r");
	return ret;
}

void
cmd_get(int argc, char **argv)
{
	Params *p;
	String *s;
	UserPasswd *up;

	p = parseparams(0);
	if(p == nil)
		sysfatal("parseparams: %r");
	s = makeString(p);
	if(s == nil)
		sysfatal("makeString: %r");
	up = auth_getuserpasswd(nil, s_to_c(s));
	if(up == nil)
		sysfatal("auth_getuserpasswd: %r");
	if(p->username == nil)
		print("username=%s\n", up->user);
	print("password=%s\n", up->passwd);
	free(up);
	s_free(s);
	freeparams(p);
}

int
ctlwrite(char *s)
{
	CFsys *fs;
	CFid *fid;
	int ret;

	ret = -1;
	fs = nsamount("factotum", nil);
	if(fs == nil)
		return -1;
	fid = fsopen(fs, "ctl", OREAD);
	if(fid == nil)
		goto out;
	if(fswrite(fid, s, strlen(s)) < 0)
		goto out;
	ret = 0;

out:
	fsclose(fid);
	fsunmount(fs);
	return ret;
}

void
cmd_store(int argc, char **argv)
{
	Params *p;
	String *s;
	char *q;

	p = parseparams(0);
	if(p == nil)
		sysfatal("parseparams: %r");
	if(p->username == nil || p->password == nil)
		sysfatal("store: need username and password");
	s = makeString(p);
	if(s == nil)
		sysfatal("makeString: %r");
	q = smprint("key %s !password=%q", s_to_c(s), p->password);
	if(q == nil)
		sysfatal("smprint: %r");
	if(ctlwrite(q) < 0)
		sysfatal("crlwrite: %r");
	free(q);
	s_free(s);
	freeparams(p);
}

void
cmd_erase(int argc, char **argv)
{
	Params *p;
	String *s;
	char *q;

	p = parseparams(0);
	if(p == nil)
		sysfatal("parseparams: %r");
	s = makeString(p);
	if(s == nil)
		sysfatal("makeString: %r");
	q = smprint("delkey %s", s_to_c(s));
	if(q == nil)
		sysfatal("smprint: %r");
	if(ctlwrite(q) < 0)
		sysfatal("ctlwrite: %r");
	free(q);
	s_free(s);
	freeparams(p);
}

Cmd commands[] = {
	{ "get", cmd_get },
	{ "store", cmd_store },
	{ "erase", cmd_erase },
	nil,
};

void
usage(void)
{
	fprint(2, "usage: %s [-D] {get|store|erase}\n", argv0);
	threadexits(nil);
}

void
threadmain(int argc, char **argv)
{
	Cmd *p;

	ARGBEGIN{
	case 'D':
		chatty9pclient++;
		break;
	default:
		usage();
	}ARGEND
	if(argc == 0)
		usage();
	quotefmtinstall();
	for(p = commands; p->name; p++)
		if(strcmp(*argv, p->name) == 0){
			p->run(argc, argv+1);
			threadexits(nil);
		}
	usage();
}
