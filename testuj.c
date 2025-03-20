#include <stdio.h>
#include <string.h>
#include <windows.h> //dla Sleep() bo standardowy sleep przyjmuje tylko całkowite sekundy


void safe_fputs(FILE* out, char* mess);
void WykonajProgramLosowe(char* programPath, char* wierzcholki);
void WykonajProgramInterakcja(char* programPath);
int SprawdzNapis(FILE* in, char* slowo, int errMess);
char* SprawdzWynik(char* outputPath);

int main()
{
    char* programPath = "C:\\Users\\awiel\\Desktop\\abcd\\program.exe";
    char* outputPath = "C:\\Users\\awiel\\Desktop\\abcd\\graph_output.txt";

    char wynikiTestow[3][200];

    WykonajProgramLosowe(programPath, "3\n");
    strcpy(wynikiTestow[0], SprawdzWynik(outputPath));

    WykonajProgramLosowe(programPath, "7\n");
    strcpy(wynikiTestow[1], SprawdzWynik(outputPath));

    WykonajProgramInterakcja(programPath);
    strcpy(wynikiTestow[2], SprawdzWynik(outputPath));

    //czatbota nie testujemy bo w dokumentacji nie było opisu formatu oczekiwanej odp zwrotnej

    FILE* out = fopen("WynikiTestow.txt", "w");
    if (out == NULL) {
        printf("Nie udalo sie otworzyc pliku do zapisywania.\n");
        return 1;
    }
    fprintf(out, "Test 1 (Losowe krawedzie): \n%s\n", wynikiTestow[0]);
    fprintf(out, "Test 2 (Losowe krawedzie): \n%s\n", wynikiTestow[1]);
    fprintf(out, "Test 3 (Samodzielnie podane krawedzie): \n%s\n", wynikiTestow[2]);

    fclose(out);
    printf("Wyniki testow zapisano w 'WynikiTestow.txt'\n");
    return 0;
}

void safe_fputs(FILE* out, char* mess)
{
    fputs(mess, out);
    fflush(out); // inaczej nie ma pewności czy dane zostały przesłane w odpowiednim momencie
    Sleep(100);  //czas w ms
    //pauza jest potrzebna bo jeśli fputs są od razu po sobie, czasem zewnętrzny program nie przetwarza ich wystarczająco szybko i się nakładają
}

void WykonajProgramLosowe(char* programPath, char* wierzcholki)
{
    printf("<> Rozpoczynam testowanie..\n");
    FILE *fp;
    fp = popen(programPath, "w"); // otwieramy proces i uruchamiamy program
    if (fp == NULL) {
        printf("Blad przy otwieraniu procesu.\n");
        return 1;
    }

    // Przekazywanie danych do stdin zewnętrznego programu
    safe_fputs(fp, "1\n"); // wybranie graf.exe

    safe_fputs(fp, wierzcholki);  // ilość wierzchołków

    safe_fputs(fp, "tak\n");  // losowe krawędzie

    safe_fputs(fp, "tak\n");  // zapisanie do pliku

    pclose(fp);
    printf("<> Test zakonczony.\n\n");
}

void WykonajProgramInterakcja(char* programPath)
{
    printf("<> Rozpoczynam testowanie..\n");
    FILE *fp;
    fp = popen(programPath, "w");
    if (fp == NULL) {
        printf("Blad przy otwieraniu procesu.\n");
        return 1;
    }

    safe_fputs(fp, "1\n"); // wybranie graf.exe

    safe_fputs(fp, "4\n"); // 4 wierzchołki

    safe_fputs(fp, "nie\n"); // samodzielnie podane krawędzie

    safe_fputs(fp, "2\n"); // 2 krawędzie

    safe_fputs(fp, "0 1\n"); // 1sza krawędź

    safe_fputs(fp, "1 2\n"); // 2ga krawędź

    safe_fputs(fp, "tak\n"); // zapisanie do pliku

    pclose(fp);
    printf("<> Test zakonczony.\n\n");
}

int SprawdzNapis(FILE* in, char* slowo, int errMess)
{
    char bufor[50];
    fscanf(in, "%s", bufor);
    if(strcmp(bufor, slowo) != 0) {
        if(errMess == 1) 
            printf("Blad: Spodziewano sie napisu 'Graf z X wierzcholkami i Y krawedziami:', nie znaleziono slowa '%s'\n", slowo);
        else 
            printf("Blad: Spodziewano sie napisu w1 -> w2, w3  ,nie znaleziono '->'\n");
        return 0;
    }
}

char* SprawdzWynik(char* outputPath)
{
    FILE* in = fopen(outputPath, "r");
    if(in == NULL) {
        static char m[200]; //static bo inaczej ją usuwa jako zmienną lokalną a chcemy ją przekazać dalej
        snprintf(m, sizeof(m), "Nie znaleziono pliku '%s'\n", outputPath);
        fclose(in);
        return m;
    }
    // spodziewamy się linii "Graf z X wierzcholkami i Y krawedziami:"
    int wierzcholki = 0;
    int krawedzie = 0;

    SprawdzNapis(in, "Graf", 1);
    SprawdzNapis(in, "z", 1);

    if(fscanf(in, "%d", &wierzcholki) != 1) {
        static char m[200];
        snprintf(m, sizeof(m), "Blad: Spodziewano sie napisu 'Graf z X wierzcholkami i Y krawedziami:', nie znaleziono liczby 'X'\n");
        fclose(in);
        return m;
    }
    SprawdzNapis(in, "wierzcholkami", 1);
    SprawdzNapis(in, "i", 1);

    if(fscanf(in, "%d", &krawedzie) != 1) {
        static char m[200];
        snprintf(m, sizeof(m), "Blad: Spodziewano sie napisu 'Graf z X wierzcholkami i Y krawedziami:', nie znaleziono liczby 'Y'\n");
        fclose(in);
        return m;
    }
    SprawdzNapis(in, "krawedziami:", 1);

    // sprawdzanie formatu dla losowych i czata:

    int kCounter = 0;
    int tmp;
    int koniecOpisu = 0;
    // spr jednej linii
    if(fscanf(in, "%d", &tmp) == 1) //pierwszy numer wierzchołka
    {
        while(!koniecOpisu)
        {
            SprawdzNapis(in, "->", 2);
            while(1){
                if(fscanf(in, "%d", &tmp) == 1)
                {
                    kCounter++;
                    if(fscanf(in, "%d", &tmp) == 1) //jeśli trafimy na dwie liczby pod rząd to dotarliśmy do połączeń kolejnego wierzchołka
                    {
                        break;
                    }
                    else
                    {
                        int c;
                        c = fgetc(in);
                        if(c == EOF) {
                            koniecOpisu = 1;
                            break;
                        }
                        else if(c != ',') { //jeśli trafimy na [,] to kontynuujemu pętlę
                            static char m[200];
                            snprintf(m, sizeof(m), "Blad: Zly format\n");
                            fclose(in);
                            return m;
                        }
                    }
                }
            }
        }
    }
    kCounter /= 2;
    static char m[200];
    if(kCounter == krawedzie)
        snprintf(m, sizeof(m), "Wynik jest poprawny.\n", kCounter);
    else
        snprintf(m, sizeof(m), "Nie zgadza sie ilosc krawedzi! Oczekiwano %d, naliczono %d.\n", krawedzie, kCounter);

    fclose(in);
    return m;

}
