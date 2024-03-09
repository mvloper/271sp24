#include <stdlib.h>
#include <stdio.h>
#include <complex.h>

// rip from https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C
// try "convert x.ppm x.png" and follow the install instructions to get a png
 


int min(int a, int b){
	if (a <= b){
		return a;
	}
	if (b < a){
		return b;
	}
}
int max(int a, int b){
	if (a >= b){
		return a;
	}
	if (b > a){
		return b;
	}
}



unsigned char ***create_base(int size){
	// In C, we make a 3 dimension unsigned char array of size * size * 3
	// Hint - how many times should loop? How many times should you call malloc?
	unsigned char ***base = malloc(sizeof(unsigned char**) * size);
	for (int i = 0; i < size; i++){
		unsigned char **col = malloc(sizeof(unsigned char*) * size);
		for (int j = 0; j < size; j++){
			unsigned char *color = malloc(sizeof(unsigned char) * 3);
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;
			col[j] = color;
		}
		base[i] = col;
	}
	return base;
}



double complex m_seq(double complex z_n, double complex c){
	// Calculate z_(n+1) = z_n^2 + c and return the result
	// in C, we accept two complex values and produce a complex output
	double complex r = z_n * z_n + c;
	return r;
}



void c2b(double complex c, int size, int *x, int *y){
	// in C we accept a complex value and an integer size and two integer pointers, and populate the integer points with the x and y results
	// I've included sample code to zero out x and y.
	*x = (int) ((creall(c) + 2.0) *  size) / 4.0;
	*y = (int) ((cimagl(c) + 2.0) * size) / 4.0;
	*x = min(*x, size - 1);
	*y = min(*y, size - 1);
	*x = max(*x, 0);
	*y = max(*y, 0);
	return;
}



double complex b2c(int size, int x, int y){
	// in C, we use b2c to loop over all pixels rather than relying on randomization
	// return the complex associated with a location x, y
	double a = x * 4.0 / size - 2.0;
	double b = y * 4.0 / size - 2.0;
	double complex r = a + b * I;
	return r;
}



int escapes(double complex c, int iters){
	// in C we accept a complex value, and integer number of iterations, and returns with an int that represents whether c escapes, or exceeds absolute value 2 in iters applications of m_seq.
	// I included the absolute value sample code
	double complex z_n = c;
	int i;
	for (i = 0; i < iters; i++){
		z_n = m_seq(z_n, c);
		if (abs(z_n) > 2){
			return 1;
		}
	}
	return 0;
}



void one_val(unsigned char ***base, int size, int iters, int color, double complex c){
	// in C, we accept a 3d array base, an integer for size and for iterations, a color channel of 0,1,2, and a complex value c
	double complex z_n = c;
	int esc = escapes(c, iters);
	if (!esc){
		return;
	}
	int i;
	for (i = 0; i < iters; i++){
		if (abs(z_n) > 2){
			return;
		}
		int x = 0;
		int y = 0;
		c2b(z_n, size, &x, &y);
		x = min(x, size - 1);
		y = min(y, size - 1);
		unsigned char v = base[x][y][color];
		v += 25;
		if (v > 255){
			v = 255;
		}
		base[x][y][color] = v;
		z_n = m_seq(z_n, c);
	}
	return;
}



void get_colors(unsigned char ***base, int size, int iters){
	// in C, we accept a 3d array base, an integer for size and for iterations
	int i_list[3] = {iters, iters * 10, iters * 100};
	int x, y, i;
	for (x = 0; x < size; x++){
		for (y = 0; y < size; y++){
			for (i = 0; i < 3; i++){
				one_val(base, size, i_list[i], i, b2c(size, x, y));
			}
		}
	}
	return;
}

void sort(int *arr, int len) // bubble sort :|
{
	int i, j;
	for ( i = 0; i < len - 1; i++ )
	{
		for ( j = 0; j < len - i - 1; j++ )
		{
			if (arr[j] > arr[j + 1])
			{
				int temp = arr[j+1];
				arr[j+1] = arr[j];
				arr[j] = temp;
			}
		}
	}
	return;
}

void median(unsigned char ***base, int size, int colr){
	// broken :(
	int winw = 2;//CHANGE WIN SIZE IF YOU CHANGE THIS
	int win[2*2] = {0};
	int x, y, fx, fy, i, temp, edge = winw/2;
	for (x = edge; x < (size-edge); x++){
		// printf("%d\n", x);
		for (y = edge; y < (size-edge); y++){
			i = 0;
			for (fx = 0; fx < winw; fx++){
				for (fy = 0; fy < winw; fy++){
					win[i] = base[x + fx - edge][y + fy - edge][colr];
					temp = i;
					i = temp + 1;
				}
			}
			sort(win, winw*winw);
			base[x][y][colr] = win[(winw * winw) / 2];
		}
	}
}

void equalize(unsigned char ***base, int size){
	// OPTIONAL
	// That said, you images will look bad without this.
	// The Python sample had a hacky solution.
	// We accept a base, and equalize values to percentiles rather than counts
	// You equalized images in CS 151 ImageShop.
	int a;
	for (a = 0; a < 3; a++){
		median(base, size, a);
		printf("equalized %d\n", a);
	}
	return;
}
 


void make_brot(int size, int iters){
	// Given an edge size and starting iteration count, make a buddhabrot.
	// I'm leaving the ppm code
	FILE *fp = fopen("brot.ppm", "wb"); /* b - binary mode */
	fprintf(fp, "P6\n%d %d\n255\n", size, size);
	static unsigned char color[3];
	unsigned char ***base = create_base(size);
	get_colors(base, size, iters);
	printf("finished get_colors");
	// equalize(base, size);
	fflush(stdout);
	for ( int x = 0 ; x < size ; x++ ){
		for ( int y = 0 ; y < size ; y++ ){
			color[0] = base[x][y][0];  /* red */
			color[1] = base[x][y][1];  /* green */
			color[2] = base[x][y][2];  /* blue */
			fwrite(color, 1, 3, fp);
			free(base[x][y]);
		}
	}
	fclose(fp);
	return;
}

 
int main()
{
	make_brot(4000,50);
	return 0;
}
