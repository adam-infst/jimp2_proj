#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define max 5

int main() {

	int n = 0;
	char c[max];
	//char* c = malloc((max+1)*sizeof(char));
	printf("podaj liczbe wierzcholkow: ");
	fgets(c, max, stdin);
	printf("work");
	if(c == NULL){
		return 1;
	}
	n = atoi(c);
	printf("%d",n);
	int **T = malloc(sizeof(int[n][n]));
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
	int r=n*n*2+1;
	srand(time(NULL));
	char *los;
	printf("jesli chcesz by graf byl losowany, wpisz y, by w przeciwnym wypadku kliknij enter");
	fgets(los,1,stdin);
	for(;;)
	{
		if(los[0] == 'y')
		{
		a=rand()%n+1;
		b=rand()%n+1;
		}else
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
		}
		if(los[0] == 'y'  &&  rand()%r == 0 || c[0]=='q')
		{
			break;
		}
		if(a>0 && b>0 && a<=n && b<=n)
		{
			T[a-1][b-1]=1;
		}
	}
	printf("wierzcholki w grafie: ");
	for(int i = 0; i < n; i++)
	{
		printf("W%d ",i);
	}
	printf("\npolaczenia w grafie:");
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
	//free(c);
	free(T);
	return 0;
}
