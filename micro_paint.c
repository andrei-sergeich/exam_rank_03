#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
	float	width;
	float	height;
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
	char	*array;
	int		count;
	int		it;

	it = 0;
	if ((count = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
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

int	is_rec(float y, float x, t_list *tmp)
{
	float	check = 1.00000000;

	if ((x < tmp->x) || (y < tmp->y) || (x > (tmp->x + tmp->width)) || (y > (tmp->y + tmp->height)))
		return (0);
	if ((x - tmp->x < check) || (y - tmp->y < check) || \
			(((tmp->x + tmp->width) - x) < check) || (((tmp->y + tmp->height) - y) < check))
		return (2);
	return (1);
}

void	get_fill(t_list *tmp, t_zone *zone, char *array)
{
	int	x, y, rec;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			rec = is_rec(y, x, tmp);
			if ((tmp->type == 'r' && rec == 2) || (tmp->type == 'R' && rec))
				array[(y * zone->width) + x] = tmp->color;
			x++;
		}
		y++;
	}
}

int	filling(FILE *file, char *array, t_zone *zone)
{
	int		count;
	t_list	tmp;

	while ((count = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y,
							&tmp.width, &tmp.height, &tmp.color)) == 6)
	{
		if (tmp.height <= 0.00000000 || tmp.width <= 0.00000000)
			return (0);
		if (tmp.type != 'r' && tmp.type !='R')
			return (0);
		get_fill(&tmp, zone, array);
	}
	if (count != -1)
		return (0);
	return (1);
}

void	painting(char *array, t_zone *zone)
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
	t_zone	zone;
	char	*array;

	if (argc != 2)
		return (err_msg("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (err_msg("Error: operation file corrupted\n"));
	if (!(array = get_zone(file, &zone)))
		return (liberator(file, NULL) && err_msg("Error: operation file corrupted\n"));
	if (!(filling(file, array, &zone)))
		return (liberator(file, array) && err_msg("Error: operation file corrupted\n"));
	painting(array, &zone);
	liberator(file, array);
	return (0);
}