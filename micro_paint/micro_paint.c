#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_draw {
    int width;
    int height;
    char *matrice;
} t_draw;

typedef struct s_rect {
    char type;
    float x;
    float y;
    float width;
    float height;
    char color;
} t_rect;

int get_info(FILE *file, t_draw *draw)
{
	char 	*temp;
	int 	i;
	char	background;
	if (fscanf(file, "%d %d %c\n", &draw->width, &draw->height, &background) == 3)
	{
		if (!(draw->width > 0 && draw->width <= 300) || !(draw->height > 0 && draw->height <= 300))
			return (1);
		temp = (char *)malloc(draw->width * draw->height);
		draw->matrice = temp;
		if (!draw->matrice)
			return (1);
		i = 0;
		while (i < draw->width * draw->height)
			draw->matrice[i++] = background;
		return (0);
	}
	return (1);
}

int is_in_rect(float x, float y, t_rect *rect)
{
	if ((((x < rect->x) || (rect->x + rect->width < x)) || (y < rect->y)) || (rect->y + rect->height < y))
		return (0);
	if (((x - rect->x < 1.00000000) || ((rect->x + rect->width) - x < 1.00000000)) ||
		((y - rect->y < 1.00000000 || ((rect->y + rect->height) - y < 1.00000000))))
		return (2); // Border
	return (1); // Inside
}

int apply_op(t_rect *rect,t_draw *draw)
{
	int j, i = 0, is_in;
	if (((rect->width <= 0.00000000) || (rect->height <= 0.00000000)) || ((rect->type != 'R' && (rect->type != 'r'))))
		return (1);
	while (i < draw->height)
	{
		j = 0;
		while (j < draw->width){
			is_in = is_in_rect((float)j, (float)i, rect);
			if ((is_in == 2) || ((is_in == 1 && (rect->type == 'R'))))
				draw->matrice[j + i * draw->width] = rect->color;
			j++;
		}
		i++;
	}
	return (0);
}

void print_info(t_draw *draw)
{
	int i = 0;
	while (i < draw->height)
		printf("%.*s\n", draw->width, draw->matrice + i++ * draw->width);
}

int execute(FILE *file)
{
	int 		scan_ret;
	t_rect	rect;
	t_draw		draw;
	if (!get_info(file, &draw))
	{
		scan_ret = fscanf(file,"%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color);
		while (scan_ret == 6)
		{
			if (apply_op(&rect, &draw))
				return (1);
			scan_ret = fscanf(file,"%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color);
		}
		if (scan_ret == -1)
		{
			print_info(&draw);
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
