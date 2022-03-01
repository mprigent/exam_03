#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

typedef struct s_drawing {
    int width;
    int height;
    char *matrice;
} t_drawing;

typedef struct s_circle {
    char type;
    float x;
    float y;
    float radius;
    char color;
} t_circle;

int get_info(FILE *file, t_drawing *drawing)

{
	char	background;
	int scan_ret = fscanf(file,"%d %d %c\n", &drawing->width, &drawing->height, &background);
	if (scan_ret == 3)
	{
		if (!(drawing->width > 0 && drawing->width <= 300) || !(drawing->height > 0 && drawing->height <= 300))
			return (1);
		drawing->matrice = (char *)malloc(drawing->width * drawing->height);
		if (!drawing->matrice)
			return (1);
		scan_ret = 0;
		while (scan_ret < drawing->width * drawing->height)
			drawing->matrice[scan_ret++] = background;
		return (0);
	}
	return (1);
}

int is_in_circle(float x, float y, t_circle *circle)
{
	float distance = sqrtf((powf((x - circle->x),2))+(powf((y - circle->y), 2))) - circle->radius;
	if (distance <= 0.00000000)
	{
		if (distance <= -1.00000000)
			return (1); // Inside
		return (2); // Border
	}
	return (0);
}

int apply_op(t_circle *circle, t_drawing *drawing)
{
	int j, i = 0, is_in;

	if ((circle->radius <= 0.00000000) || ((circle->type != 'C' && (circle->type != 'c'))))
		return (1);
	while (i < drawing->height)
	{
		j = 0;
		while (j < drawing->width){
			is_in = is_in_circle((float)j, (float)i, circle);
			if ((is_in == 2) || ((is_in == 1 && (circle->type == 'C'))))
				drawing->matrice[j + i * drawing->width] = circle->color;
			j++;
		}
		i++;
	}
	return (0);
}

int print_info(t_drawing *drawing)
{
	int i = 0;
	while (i < drawing->height)
	{
		printf("%.*s\n", drawing->width, drawing->matrice + i * drawing->width);
		i = i + 1;
	}
	return i;
}

int execute(FILE *file)
{
	int 	scan_ret;
	t_circle 	circle;
	t_drawing	drawing;

	if (!get_info(file, &drawing))
	{
		scan_ret = fscanf(file,"%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.color);
		while (scan_ret == 5)
		{
			if (apply_op(&circle, &drawing))
				return (1);
			scan_ret = fscanf(file,"%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.color);
		}
		if (scan_ret == -1)
		{
			print_info(&drawing);
			return (0);
		}
		return (1);
	}
	return (1);
}

int main(int argc,char **argv)
{
	FILE *file;

	if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (file && !execute(file))
			return (0);
		write(1, "Error: Operation file corrupted\n", 32);
	}
	else
		write(1, "Error: argument\n", 16);
	return (1);
}
