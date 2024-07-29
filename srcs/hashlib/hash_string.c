#define FNV_OFFSET 2166136261

unsigned int	hash_string(const char *str)
{
	unsigned int	h;

	h = FNV_OFFSET;
	while (*str)
	{
		h += (h << 1) + (h << 4) + (h << 7) + (h << 8) + (h << 24);
		h ^= *str;
		str++;
	}
	return (h);
}
