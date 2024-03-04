#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
 
//takes two integers
//returns the one that is less
int min(int a, int b){
	if (a <= b){
		return a;
	}
	if (b < a){
		return b
	}
}

//takes two integers
//returns the one that is greater
int max(int a, int b){
	if (a >= b){
		return a;
	}
	if (b > a){
		return b
	}
}
 
// rip from https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C
// try "convert x.ppm x.png" and follow the install instructions to get a png

// In C, we make a 3 dimension unsigned char array of size * size * 3
// Hint - how many times should loop? How many times should you call malloc?
unsigned char ***create_base(int size)
{
	unsigned char ***base = malloc(sizeof(unsigned char**) * size);
	for (int i = 0; i < size; i++){
		unsigned char **col = malloc(sizeof(unsigned char*) * size);
		for (int j = 0; j < size; j++){
			unsigned char *thing = malloc(sizeof(unsigned char) * 3);
			thing[0] = 0;
			thing[1] = 0;
			thing[2] = 0;
			col[j] = thing;
		}
		base[i] = col;
	}
	return base;
}

// Calculate z_(n+1) = z_n^2 + c and return the result
// in C, we accept two complex values and produce a complex output
// I've included sample code to work with complex values.
// Hint - don't use exponentiation
double complex m_seq(double complex z_n, double complex c)
{
	double complex r = c + z_n * z_n;
	return r;
}

// in C we accept a complex value and an integer size and two integer pointers, and populate the integer points with the x and y results
// I've included sample code to zero out x and y.
void c2b(double complex c, int size, int *x, int *y)
{
	*x = (int) (creal(c) + 2.0) * ((double) size) / 4.0;
	*y = (int) (cimag(c) + 2.0) * ((double) size) / 4.0;
*x = min(*x, size - 1);
	*y = min(*y, size - 1);
	*x = max(*x, 0);
	*y = max(*y, 0);
	return;
}


// in C, we use b2c to loop over all pixels rather than relying on randomization
// return the complex associated with a location x, y
// I've included sample code to work with complex values.
double complex b2c(int size, int x, int y)
{
	double a = x * 4.0 / size - 2.0;
	double b = y * 4.0 / size - 2.0;
	double complex r = a + b * I;
	return r;
}

// in C we accept a complex value, and integer number of iterations, and returns with an int that represents whether c escapes, or exceeds absolute value 2 in iters applications of m_seq.
// I included the absolute value sample code
int escapes(double complex c, int iters)
{
	int i;
	for (i = 0; i < iters; i++){
		z_n = m_seq(z_n, c);
		if (abs(z_n) > 2){
			return 1;
		}
	}
	return 0;
}

// in C, we accept a 3d array base, an integer for size and for iterations, a color channel of 0,1,2, and a complex value c
void one_val(unsigned char ***base, int size, int iters, int color, double complex c)
{
double complex z_n = c;
	if (0 == int escapes(c, iters)){
		return;
	}
	int i;
	for (i = 0; i < iters; i++){
		if (abs(z_n) > 2){
			return;
		}
		int a = 0, b = 0;
		int *x = a;
		int *y = b;
		c2b(z_n, size, *x, *y);
		*x = min(*x, size - 1);
		*y = min(*y, size - 1);
		/*idk how arrays work*/
	}
	return;
/*py
def one_val(base, iters, color, c):
	size = len(base)
	z_n = c # we save the starting value
	if not escapes(c, iters):
		return
	for i in range(iters):
		if abs(z_n) > 2:
			return base
		(x,y) = c2b(z_n, size)
		x = min(x, size - 1) # prevent overflow
		y = min(y, size - 1) # prevent overflow
		v = base[x][y][color]
		v += 25
		if v > 255:
			v = 255
		base[x][y][color] = v
		z_n = m_seq(z_n, c)
	return
*/
}

// in C, we accept a 3d array base, an integer for size and for iterations
void get_colors(unsigned char ***base, int size, int iters)
{
int i_list[3] = {iters, iters * 10, iters * 100};
	int x, y, i;
	for (x = 0; x < size; x++){
		for (y = 0; y < size; y++){
			for (i = 0; i < 3; i++){
				one_val(base, i_list[i], i, b2c(x, y, size));
			}
		}
	}
	return;
}

// OPTIONAL
// That said, you images will look bad without this.
// The Python sample had a hacky solution.
// We accept a base, and equalize values to percentiles rather than counts
// You equalized images in CS 151 ImageShop.
void equalize(unsigned char ***base, int size)
{
	return;
}
 
// Given an edge size and starting iteration count, make a buddhabrot.
// I'm leaving the ppm code
void make_brot(int size, int iters)
{

	FILE *fp = fopen("brot.ppm", "wb"); /* b - binary mode */
	fprintf(fp, "P6\n%d %d\n255\n", size, size);
	static unsigned char color[3];

	fflush(stdout);
	for ( int x = 0 ; x < size ; x++ )
	{
		for ( int y = 0 ; y < size ; y++ )
		{
			color[0] = 0;  /* red */
			color[1] = 0;  /* green */
			color[2] = 0;  /* blue */
			fwrite(color, 1, 3, fp);
		}
	}
	fclose(fp);
	return;
}

 
int main()
{
	//make_brot(4000,50);
	return 0;
}
