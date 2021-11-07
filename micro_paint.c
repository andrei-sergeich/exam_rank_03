#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typdef struct s_zone
{
	int		width;
	int		height;
	char	background;
}	t_zone

typdef struct s_list
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	color;
}	t_list

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	if (!str)
		return(len);
	while (str[len])
		len++;
	return (len);
}

int	err_msg(char *str)
{
	write(1, str, ft_strlen(str));
	return (1);
}

int	liberator(FILE *file, char *str)
{
	fclose(file);
	if (str)
		free(str);
	return (1);
}

char	*get_zone(FILE *file, t_zone *zone)
{
	char	array;
	int		count;
	int		it;

	it = 0;
	if ((count = fscanf(file, "%d %d %c", zone->width, zone->height, zone->background)) != 3)
		return (NULL);
	if (count == -1)
		return (NULL);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (NULL);
	if (!(array = (char *)malloc(sizeof(char) * (zone->width * zone->height))))
		return (NULL);
	while (it < zone->width * zone->height)
	{
		array[it] = zone->background;
		it++;
	}
	return (array);
}

int	main(int argc, char **argv)
{
	FILE	*file;
	t_zone	zone;
	char	*array;

	if (argc != 2)
		return (err_msg("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (err_msg("Error: operation file corrupted\n"));
	if (!(array = get_zone(file, &zone)))
		return (liberator(file, NULL) err_msg("Error: operation file corrupted\n"));

	liberator(file, array);
	return (0);
}