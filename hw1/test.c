#include <stdlib.h>
#include <stdio.h>
#include <complex.h>

int min(int a, int b){
	if (a <= b){
		return a;
	}
	if (b < a){
		return b;
	}
}

//takes two integers
//returns the one that is greater
int max(int a, int b){
	if (a >= b){
		return a;
	}
	if (b > a){
		return b;
	}
}

double complex m_seq(double complex z_n, double complex c)
{
	double complex r = c + z_n * z_n;
	return r;
}

int main(){
	double complex c = 1 + 1 * I;
	double complex z_n = c;
	int iters = 1000;
	int i;
	for (i = 0; i < iters; i++){
		z_n = m_seq(z_n, c);
		if (abs(z_n) > 2){
			printf("true");
			return 0;
		}
	}
	printf("false");
	return 0;
}