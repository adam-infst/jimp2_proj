#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <time.h>

#define max 5

void WypiszGraf(int **T, int n);
char* AskChatbot();
int** ExtractData(char* text, int* n); // deklaracja przed main żeby -Wall nie krzyczał

int main()
{
    printf("Chcesz utworzyć graf samemu czy przy pomocy czatbota?\n[s - samemu, c - czatbot] ");
    int mode = fgetc(stdin);
    printf("\n");

    if(mode == 's')
    {
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
		int **T = malloc(n * sizeof(int*));
		if(T == NULL){
			return 2;
		}
		for (int i = 0; i < n; i++) {
			T[i] = malloc(n * sizeof(int));
			if(T[i] == NULL){
				return 3;
			}
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
        WypiszGraf(T, n);
		//free(c);
		free(T);
    }
    else if(mode == 'c')
    {
        int n = 0;
        int **T = ExtractData(AskChatbot(), &n);
        WypiszGraf(T, n);
    }
    else 
    {
        printf("Podano niewłaściwą opcję.\n");
        return -1;
    }

	return 0;
}

void WypiszGraf(int **T, int n)
{
	printf("wierzcholki w grafie: ");
	for(int i = 0; i < n; i++)
	{
		printf("W%d ",i);
	}
    printf("\npolaczenia w grafie: ");
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
}

char* AskChatbot()
{
    char message[900];
    // printf("\n");
    // int c = fgetc(stdin); // czasami w strumieniu zostaje \n po scanf i psuje fgets, ale to chyba tylko w wsl
    // if (c != '\n') {
    //     ungetc(c, stdin);
    // }
    printf("Napisz wiadomość do czatbota: \n");
	fgets(message, sizeof(message), stdin);
	
    char* url = "http://localhost:1234/api/v0/chat/completions";
	// UWAGA na wsl serwer może mieć problemy ale to już jego sprawa, kod działa tylko serwer nie chce odpowiedzieć
    char jsonInputString[1000];
    sprintf(jsonInputString,
    "{"
        " \"model\": \"granite-3.0-2b-instruct\","
        " \"messages\": [ "
            " { \"role\": \"system\", \"content\": \"Twoim jedynym zadaniem jest pomóc użytkownikowi stworzyć graf."
			" Musisz odpowiedzieć w następującym formacie: # N  W x1 x2 xN  W x1 x2 xN ...  W x1 x2 xN gdzie # to po"
			" prostu #, N to ilość wierzchołków, W to numer wierzchołka od 1 do N, x1 x2 ... xN to cyfry 0 lub 1"
            " w ilości N gdzie 0/1 na miejscu x1 oznacza że wierzchołek W ma/nie ma łuku do wierzchołka nr 1 itd."
            " Maksymalna ilość wierzchołków to 5\" },"
            " { \"role\": \"user\", \"content\": \"%s\" }"
        " ],"
        " \"temperature\": 0.0,"
        " \"max_tokens\": -1,"
        " \"stream\": false"
    "}", message);

    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: application/json");

		char* response = malloc(4000 * sizeof(char)); // inaczej -Wall nie pozwoli na return zm lokalnej
		size_t write_callback;

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonInputString);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // limit czasowy na 15 sekund

        printf("\nOczekiwanie na połączenie...\n");

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            printf("CURL request failed: %s\n", curl_easy_strerror(res));
            char* example = malloc(130 * sizeof(char)); //ExtractData potrzebuje dynamicznego ciągu znaków
            strcpy(example, "Cześć, chętnie pomogę ci utworzyć graf! Oto zapis grafu zgodny z formatem który"
            " otrzymałem: # 3  1 0 1 0  2 0 0 1  3 1 0 0"); // strcpy żeby nie nadpisać wskaźnika
            printf("Zwrócono przykładową odpowiedź od czatbota: \n%s\n\n", example);
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            free(response);
            return example;
        }
        else
        {
            printf("Chatbot: %s\n", response);
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            return response;
        }
    }
    else
    {
        printf("Error: Failed to initialize curl.\n");
        return "error";
    }
    curl_global_cleanup();
}

int** ExtractData(char text[], int* n)
{
    char* znak = "#";
    char* token;

    token = strtok(text, znak); // dotarł do znaku
    
    token = strtok(NULL, " "); // przechodzi do następnego tokena
    *n = atoi(token);
    if(*n > max) {
        printf("Zbyt duża liczba wierzchołków.\n");
        free(text);
        return NULL;
    }
    
	int **T = malloc((*n) * sizeof(int*));
	if(T == NULL){
		return NULL;
	}
	for (int i = 0; i < (*n); i++) {
		T[i] = malloc((*n) * sizeof(int));
		if(T[i] == NULL){
			return NULL;
		}
	}

    token = strtok(NULL, " "); // przechodzimy na liczbę oznaczającą 1szy wierzchołek

    if (token != NULL)
    {
        for(int i = 0; i < *n; i++)
        {
            token = strtok(NULL, " "); // pierwsza liczba z serii to nr wierzchołka
            for(int j = 0; j < *n; j++)
            {
                T[i][j] = atoi(token);
                token = strtok(NULL, " ");
            }
        }
    }
    else {
        printf("Nie znaleziono znaku '%s' w ciągu znaków.\n", znak);
    }
    free(text);
    return T;
}