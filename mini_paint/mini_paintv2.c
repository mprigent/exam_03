#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#define ARG "argument"
#define CORRUPT "Operation file corrupted"

int	throw_error(char *error)
{
	int i = 0;
	while (error[i])
		i++;
	write(1, "Error: ", 7);
	write(1, error, i);
	write(1, "\n", 1);
	return (1);
}

int	draw_buff(char *buff, int width, int height)
{
	//draw buff from top to bottom for the entire width
	for (int Y = 0;Y < height;Y++) 	{
		write(1, buff + (Y * width), width);
		write(1, "\n", 1);
	}
	return (0); //important to return 0 here
}

int	main(int argc, char **argv)
{
	FILE	*F;
	int		width, height, ret; //ret variable for fscanf
	char	bc, c, drawchar; //bc = BACKGROUND_CHAR, drawchar = CHAR
	float	Xa, Ya, Xb, Yb, radius, distance;

	// --- Error check before doing anything ---
	if (argc != 2)
		return (throw_error(ARG));
	if (!(F = fopen(argv[1], "r"))) //try and open argv[1] for reading
		return (throw_error(CORRUPT));
	if ((fscanf(F, "%d %d %c\n", &width, &height, &bc)) != 3) //try and get the first line, fscanf must return 3
		return (throw_error(CORRUPT));
	if (!(width > 0 && width <= 300) || !(height > 0 && height <= 300))
		return (throw_error(CORRUPT));

	// --- Init the buffer ---
	char	buff[(width * height)]; //insert bc and drawchar in this buffer
	for (int XY = 0;XY < (width * height);XY++) // fill the buffer with bc
		buff[XY] = bc;

	// --- fill the buffer ---
	while ((ret = fscanf(F, "%c %f %f %f %c\n", &c, &Xa, &Ya, &radius, &drawchar)) == 5)
	{
		if ((c != 'c' && c != 'C') || !(radius > 0.00000000))
			return (throw_error(CORRUPT));
		for (int Y = 0;Y < height;Y++)
		{
			Yb = (float)Y; //important to cast here
			for (int X = 0;X < width;X++)
			{
				Xb = (float)X; //important to cast here
				distance = sqrtf((Xa - Xb) * (Xa - Xb) + (Ya - Yb) * (Ya - Yb));
				if (distance <= radius)
				{
					if ((c == 'c' && radius - distance < 1.00000000) //important: radius - distance and not distance - radius
						|| (c == 'C'))
						buff[(Y * width) + X] = drawchar;
				}
			}
		}
	}
	if (ret != -1) // if ret is not -1, fscanf did not finish reading hence: error
		return (throw_error(CORRUPT));
	return (draw_buff(buff, width, height));
}
