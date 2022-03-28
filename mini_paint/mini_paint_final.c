#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define ARG "argument"
#define CORRUPT "Operation file corrupted"

int ft_error(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	write(1, "Error: ", 7);
	write(1, str, i);
	write(1, "\n", 1);
	return (1);
}

int ft_draw(char *buff, int width, int height)
{
	for (int Y = 0; Y < height; Y++)
	{
		write(1, buff + (Y * width), width);
		write(1, "\n", 1);
	}
	return (0);
}

int main(int argc, char **argv)
{
	FILE *file;
	int width, height, ret;
	char background, c, drawchar;
	float Xa, Ya, Xb, Yb, radius, distance;

	if (argc != 2)
		return (ft_error(ARG));
	if (!(file = fopen(argv[1], "r")))
		return (ft_error(CORRUPT));
	if ((fscanf(file, "%d %d %c\n", &width, &height, &background)) != 3)
		return (ft_error(CORRUPT));
	if (!(width > 0 && width <= 300) || !(height > 0 && height <= 300))
		return (ft_error(CORRUPT));
	char buff[(width * height)];
	for (int XY = 0; XY < (width * height); XY++)
		buff[XY] = background;
	while ((ret = fscanf(file, "%c %f %f %f %c\n", &c, &Xa, &Ya, &radius, &drawchar)) == 5)
	{
		if ((c != 'c' && c != 'C') || !(radius > 0.00000000))
			return (ft_error(CORRUPT));
		for(int Y = 0; Y < height; Y++)
		{
			Yb = (float)Y;
			for(int X = 0; X < width; X++)
			{
				Xb = (float)X;
				distance = sqrtf((Xa - Xb) * (Xa - Xb) + (Ya - Yb) * (Ya - Yb));
				if (distance <= radius)
				{
					if ((c == 'c' && radius - distance < 1.00000000) || (c == 'C'))
						buff[(Y * width) + X] = drawchar;
				}
			}
		}
	}
	if (ret != -1)
		return (ft_error(CORRUPT));
	return (ft_draw(buff, width, height));
}
