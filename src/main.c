#include <stdio.h>
#include <stdlib.h>

const int max=5;

int main() {

	int n = 0;
	char *c = NULL;


	c = malloc(max*sizeof(char));
	printf("podaj liczbe wierzcholkow: ");
	fgets(c, max, stdin);
	if(c == NULL){
		return 1;
	}
	n = atoi(c);
        int (*T)[n] = malloc(sizeof(int[n][n]));
	if(T == NULL){
		return 2;
	}
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			T[i][j] = 0;
		}
	}
	int a = 0, b = 0;
	for(;;)
	{
		a = 0;
		b = 0;
		printf("podaj wierzcholek z ktorego chcesz zrobic polaczenie, lub q jesli chcesz przestac pisac wierzcholki:");
		fgets(c, max, stdin);
		if(c[0] == 'q')
		{
			break;
		}
		a = atoi(c);
		printf("podaj wierzcholek do ktorego chcesz zrobic polaczenie:");
		fgets(c, max, stdin);
		b = atoi(c);
		if(a>0 && b>0 && a!=b && a<=n && b<=n)
		{
			T[a-1][b-1]=1;
		}
	}
	printf("wierzcholki w grafie: ");
	for(int i = 0; i < n; i++)
	{
		printf("\n");
		for(int j = 0; j < n; j++)
		{
			if(T[i][j] == 1){
				printf("W%d->W%d ", i+1, j+1);
			}			
		}
	}
	printf("\n");
	return 0;
}
