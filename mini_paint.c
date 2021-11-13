#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_zone
{
	int		width;
	int		height;
	char	background;
}	t_zone;

typedef struct s_list
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	color;
}	t_list;

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	if (!str)
		return (len);
	while (str[len])
		len++;
	return (len);
}

int	liberator(FILE *file, char *str)
{
	fclose(file);
	if (str)
		free(str);
	return (1);
}

int	err_msg(char *str)
{
	write(1, str, ft_strlen(str));
	return (1);
}

char	*fill_zone(FILE *file, t_zone *zone)
{
	char	*array;
	int		count;
	int		it;

	if ((count = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
		return (NULL);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (NULL);
	if (!(array = (char *)malloc(sizeof(char) * (zone->width * zone->height))))
		return (NULL);
	it = 0;
	while (it < zone->width * zone->height)
	{
		array[it] = zone->background;
		it++;
	}
	return (array);
}

int	is_in_rad(t_list *list, float x, float y)
{
	float	dist;

	dist = sqrtf(((x - list->x) * (x - list->x)) + ((y - list->y) * (y - list->y)));
	if (dist <= list->radius)
	{
		if ((list->radius - dist) < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}

void	get_fill(t_list *list, t_zone *zone, char *array)
{
	int	x, y, rad;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			rad = is_in_rad(list, x, y);
			if ((rad == 2 && list->type == 'c') || (rad && list->type == 'C'))
				array[(y * zone->width) + x] = list->color;
			x++;
		}
		y++;
	}
}

int	printing_prepare(FILE *file, t_zone *zone, char *array)
{
	t_list	list;
	int		count;

	while ((count = fscanf(file, "%c %f %f %f %c\n", &list.type, &list.x, \
				&list.y, &list.radius, &list.color)) == 5)
	{
		if (list.radius < 0.00000000)
			return (0);
		if (list.type != 'c' && list.type != 'C')
			return (0);
		get_fill(&list, zone, array);
	}
	if (count != -1)
		return (0);
	return (1);
}

void	printing(t_zone *zone, char *array)
{
	int	it = 0;

	while (it < zone->height)
	{
		write(1, array + (it * zone->width), zone->width);
		write(1, "\n", 1);
		it++;
	}

}

int	main(int argc, char **argv)
{
	FILE	*file;
	char	*array;
	t_zone	zone;

	if (argc != 2)
		return (err_msg("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (err_msg("Error: operation file corrupted\n"));
	if (!(array = fill_zone(file, &zone)))
		return (liberator(file, NULL) && err_msg("Error: operation file corrupted\n"));
	if (!printing_prepare(file, &zone, array))
		return (liberator(file, array) && err_msg("Error: operation file corrupted\n"));
	printing(&zone, array);
	liberator(file, array);
	return (0);
}