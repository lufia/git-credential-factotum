#include <u.h>
#include <libc.h>

void
noauth(void)
{
}

int
ctlwrite(char *s)
{
	int fd;

	fd = open("/mnt/factotum/ctl", OWRITE);
	if(fd < 0)
		return -1;
	if(write(fd, s, strlen(s)) < 0){
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}
