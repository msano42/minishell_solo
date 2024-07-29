#ifndef HASHLIB_H
# define HASHLIB_H

# include <stdint.h>

/* **************************************************************** */
/*	                          Defines	                            */
/* **************************************************************** */

# define HASH_NOSRCH 0x01

/* **************************************************************** */
/*	                           Structs	                            */
/* **************************************************************** */

typedef struct s_bucket
{
	char			*key;
	void			*data;
	unsigned int	key_hash;
	int				times_found;
	struct s_bucket	*next;
}	t_bucket;

typedef struct s_hash_table
{
	t_bucket	**bucket_array;
	int			nbuckets;
	int			nentries;
}	t_hash_table;

/* **************************************************************** */
/*	                     Function Definitions	                    */
/* **************************************************************** */

/* hash_create.c */
t_hash_table	*hash_create(int num_buckets);

/* hash_destroy.c */
void			hash_destroy(t_hash_table *table);

/* hash_flush.c */
void			hash_flush(t_hash_table *table, void (*free_func)(void *));

/* hash_search.c */
t_bucket		*hash_search(const char *str, t_hash_table *table);

/* hash_insert.c */
t_bucket		*hash_insert(char *str, t_hash_table *table, uint8_t flags);

/* hash_remove.c */
t_bucket		*hash_remove(const char *str, t_hash_table *table);

/* hash_string.c */
unsigned int	hash_string(const char *str);

/* hash_size.c */
int				hash_size(t_hash_table *table);

/* hash_bucket.c */
int				hash_bucket(const char *str, t_hash_table *table);

#endif /* !HASHLIB_H */
