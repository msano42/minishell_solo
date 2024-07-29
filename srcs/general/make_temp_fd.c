#include <stddef.h>
#include <fcntl.h>
#include <errno.h>

#include "general.h"
#include "wrappers.h" // memory

#include <string.h> // ft_memmove()

#define BUFFER_SIZE 20
#define RANDOM_SUFFIX_LEN 15 // why isn't this passed as argument??

static char				*_gen_random_filename(const char *root, \
						enum e_status *status);
static enum e_status	_fill_with_random_alnum(char *dst, size_t size);

int	make_temp_fd(const char *root, char **filename, enum e_status *status)
{
	int		fd;

	while (1)
	{
		*filename = _gen_random_filename(root, status);
		if (!*filename)
			return (-1);
		fd = xopen(*filename, (O_CREAT | O_EXCL | O_RDWR), (S_IRUSR | S_IWUSR));
		if (fd >= 0 || errno != EEXIST)
			break ;
		free(*filename);
	}
	if (fd < 0)
	{
		*status = E_OPEN;
		free(*filename);
		*filename = NULL;
	}
	return (fd);
}

/* **************************************************************** */
/*	                      Static Functions	                        */
/* **************************************************************** */

static char	*_gen_random_filename(const char *root, enum e_status *status)
{
	char			*filename;
	const size_t	rootlen = ft_strlen(root);

	filename = xmalloc(rootlen + RANDOM_SUFFIX_LEN + 1);
	if (!filename)
	{
		*status = E_MALLOC;
		return (NULL);
	}
	ft_memmove(filename, root, rootlen);
	filename[rootlen + RANDOM_SUFFIX_LEN] = '\0';
	*status = _fill_with_random_alnum(&filename[rootlen], RANDOM_SUFFIX_LEN);
	if (*status != SUCCESS)
	{
		free(filename);
		return (NULL);
	}
	return (filename);
}

static enum e_status	_fill_with_random_alnum(char *dst, size_t size)
{
	size_t	i;
	ssize_t	j;
	int		fd;
	ssize_t	bytes_read;
	char	buf[BUFFER_SIZE];

	fd = xopen("/dev/random", O_RDONLY, NO_MODE);
	if (fd < 0)
		return (E_OPEN);
	i = 0;
	while (i < size)
	{
		if (!i || j >= bytes_read)
		{
			j = 0;
			bytes_read = read(fd, buf, BUFFER_SIZE);
			if (bytes_read < 0)
				return (close(fd), E_READ);
		}
		if (bytes_read && ft_isalnum(buf[j]))
			dst[i++] = buf[j];
		j++;
	}
	close(fd);
	return (SUCCESS);
}

// #include <stdio.h>
// int	main(void)
// {
// 	char			*filename;
// 	enum e_status	status;
// 	size_t			i;

// 	filename = NULL;
// 	status = SUCCESS;

// 	i = 0;
// 	while (i < 10)
// 	{
// 		filename = _gen_random_filename("sh-thd_", &status);
// 		printf("%s\n", filename);
// 		free(filename);
// 		i++;
// 	}
// }

/* **************************************************************** */
/*	                 Static Functions Documentation	            	*/
/* **************************************************************** */

/**
	@fn static char	*_gen_random_filename(const char *root, 
	enum e_status *status)

	@brief Generates a random filename prefixed by \ar root.

	@li The length of the random portion is defined by \ar RANDOM_SUFFIX_LEN.

	@param root The root path to which the random filename will be appended.
	@param status A pointer to an enum e_status representing the status
	of the operation.

	@return Pointer to the generated filename, or NULL on error.

	\ar *status is set according to the execution status:
	@li \ar SUCCESS
	@li \ar E_MALLOC memory allocation failure
	@li \ar E_OPEN: open(2) error
	@li \ar E_READ: read(2) error

	@showrefby
*/

/**
	@fn static enum e_status	_fill_with_random_alnum(char *dst, size_t size)

	@brief Fills \ar dst with \ar size bytes of random alphanumeric
	characters.

	@li Uses /dev/random to fill \ar dst.

	@param dst Pointer to the buffer to fill.
	@param size Number of bytes to fill.

	@return Execution status:
		@li \ar SUCCESS
		@li \ar E_OPEN: Failed to open /dev/random for reading.
		@li \ar E_READ: Failed to read random data from /dev/random.

	@showrefby
*/