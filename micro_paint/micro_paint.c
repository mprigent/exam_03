#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_drawing {
    int width;
    int height;
    char *matrice;
} t_drawing;

typedef struct s_rectangle {
    char type;
    float x;
    float y;
    float width;
    float height;
    char color;
} t_rectangle;

int get_info(FILE *file, t_drawing *drawing)
{
	char 	*tmp;
	int 	i;
	char	background;
	if (fscanf(file, "%d %d %c\n", &drawing->width, &drawing->height, &background) == 3)
	{
		if (!(drawing->width > 0 && drawing->width <= 300) || !(drawing->height > 0 && drawing->height <= 300))
			return (1);
		tmp = (char *)malloc(drawing->width * drawing->height);
		drawing->matrice = tmp;
		if (!drawing->matrice)
			return (1);
		i = 0;
		while (i < drawing->width * drawing->height)
			drawing->matrice[i++] = background;
		return (0);
	}
	return (1);
}

int is_in_rectangle(float x, float y, t_rectangle *rectangle)
{
	if ((((x < rectangle->x) || (rectangle->x + rectangle->width < x)) || (y < rectangle->y)) || (rectangle->y + rectangle->height < y))
		return (0);
	if (((x - rectangle->x < 1.00000000) || ((rectangle->x + rectangle->width) - x < 1.00000000)) ||
		((y - rectangle->y < 1.00000000 || ((rectangle->y + rectangle->height) - y < 1.00000000))))
		return (2); // Border
	return (1); // Inside
}

int apply_op(t_rectangle *rect,t_drawing *drawing)
{
	int j, i = 0, is_in;
	if (((rect->width <= 0.00000000) || (rect->height <= 0.00000000)) || ((rect->type != 'R' && (rect->type != 'r'))))
		return (1);
	while (i < drawing->height)
	{
		j = 0;
		while (j < drawing->width){
			is_in = is_in_rectangle((float)j, (float)i, rect);
			if ((is_in == 2) || ((is_in == 1 && (rect->type == 'R'))))
				drawing->matrice[j + i * drawing->width] = rect->color;
			j++;
		}
		i++;
	}
	return (0);
}

void print_info(t_drawing *zone)
{
	int i = 0;
	while (i < zone->height)
		printf("%.*s\n", zone->width, zone->matrice + i++ * zone->width);
}

int execute(FILE *file)
{
	int 		scan_ret;
	t_rectangle	rect;
	t_drawing		drawing;
	if (!get_info(file, &drawing))
	{
		scan_ret = fscanf(file,"%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color);
		while (scan_ret == 6)
		{
			if (apply_op(&rect, &drawing))
				return (1);
			scan_ret = fscanf(file,"%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color);
		}
		if (scan_ret == -1)
		{
			print_info(&drawing);
			return (0);
		}
	}
	return (1);
}

int main(int argc, char **argv)
{
	FILE	*file;
	if (argc == 2)
	{
		file = fopen(argv[1],"r");
		if (file && !execute(file))
			return 0;
		write(1, "Error: Operation file corrupted\n", 32);
	}
	else
		write(1, "Error: argument\n", 16);
	return (1);
}
