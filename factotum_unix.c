#include <u.h>
#include <libc.h>
#include <thread.h>
#include <auth.h>
#include <9pclient.h>

static CFsys *(*nsmnt)(char*, char*) = nsamount;

int
threadmaybackground(void)
{
	return 1;
}

void
noauth(void)
{
	nsmnt = nsmount;
}

int
ctlwrite(char *s)
{
	CFsys *fs;
	CFid *fid;
	int ret;

	ret = -1;
	fs = nsmnt("factotum", nil);
	if(fs == nil)
		return -1;
	fid = fsopen(fs, "ctl", OWRITE);
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
