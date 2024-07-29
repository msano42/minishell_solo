#ifndef WRAPPERS_H
# define WRAPPERS_H

# ifdef UNIT_TESTING
#  include "unity_memory.h"
# else
#  include <stdlib.h>
# endif /* UNIT_TESTING */

# include <stddef.h>
# include <unistd.h>
# include <dirent.h>
# include <termios.h>
# include <sys/wait.h>
# include <sys/stat.h>

# define NO_MODE 0 /* for third xopen() flag */

void			*xmalloc(size_t size);
void			xfree(void *ptr);
ssize_t			xwrite(int fd, const void *buf, size_t nbyte);
int				xaccess(const char *path, int mode);
int				xopen(const char *path, int oflag, mode_t mode);
ssize_t			xread(int fd, void *buf, size_t nbyte);
int				xclose(int fd);
pid_t			xfork(void);
pid_t			xwait(int *stat_loc);
pid_t			xwaitpid(pid_t pid, int *stat_loc, int options);
pid_t			xwait3(int *stat_loc, int options, struct rusage *rusage);
pid_t			xwait4(pid_t pid, int *stat_loc, int options, \
				struct rusage *rusage);
sig_t			xsignal(int sig, sig_t func);
int				xsigaction(int sig, const struct sigaction *act, \
				struct sigaction *oact);
int				xsigemptyset(sigset_t *set);
int				xsigaddset(sigset_t *set, int signo);
int				xkill(pid_t pid, int sig);
void			xexit(int status);
char			*xgetcwd(char *buf, size_t size);
int				xchdir(const char *path);
int				xstat(const char *path, struct stat *buf);
int				xlstat(const char *path, struct stat *buf);
int				xfstat(int fd, struct stat *buf);
int				xunlink(const char *path);
int				xexecve(const char *path, char *const argv[], \
				char *const envp[]);
int				xdup(int fd);
int				xdup2(int fd, int fd2);
int				xpipe(int fd[2]);
DIR				*xopendir(const char *filename);
struct dirent	*xreaddir(DIR *dirp);
int				xclosedir(DIR *dirp);
char			*xstrerror(int errnum);
void			xperror(const char *s);
int				xisatty(int fd);
char			*xttyname(int fd);
int				xioctl(int fd, unsigned long request, void *ptr);
char			*xgetenv(const char *name);
int				xtcsetattr(int fd, int optional_actions, \
							const struct termios *termios_p);
int				xtcgetattr(int fd, struct termios *termios_p);

#endif /* !WRAPPERS_H */

// readline
// rl_clear_history
// rl_on_new_line
// rl_replace_line
// rl_redisplay
// add_history
// printf

// ttyslot
// tgetent
// tgetflag
// tgetnum
// tgetstr
// tgoto
// tputs
