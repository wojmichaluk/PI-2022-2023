#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#define N_MAX 100000   // maksymalna liczba prób
#define TEST 0         // 1 - ulatwia testowanie programu, 0 - dla automatycznej oceny

// Oblicza wartoœæ œredniej arytmetycznej oraz wariancji próby losowej
void aver_varian(const double tab[], size_t n, double *arith_average, double *variance) {
    for (size_t i=0; i<n; i++)
        *arith_average+=tab[i]/n;
    for (size_t i=0; i<n; i++)
        *variance+=(tab[i]-*arith_average)*(tab[i]-*arith_average)/n;
}

// Wype³nia tablicê tab wynikami n prób Bernoulliego
// 1 - sukces; 0 - pora¿ka
void bernoulli_gen(int tab[], size_t n, double probab) {
    double prob;
    for (size_t i=0; i<n; i++){
        prob=rand();
        if (prob<probab*(RAND_MAX+1)) tab[i]=1;
        else tab[i]=0;
    }
}

// Dyskretny rozk³ad prawdopodobieñstwa zmiennej losowej - sumy oczek dwóch kostek do gry.
// Wartoœci zmiennej losowej s¹ liczbami naturalnymi nale¿¹cymi do przedzia³u [2-12].
// throws_num - liczba rzutów (jeden rzut to rzut dwoma kostkami).
// Funkcja zapisuje obliczony rozk³ad w pocz¹tkowych elementach tablicy tab.
void two_dice_probab_distrib(double distrib[], int throws_num) {
    int a;
    for (int i=0; i<11; i++) distrib[i]=0.0;
    for (int i=0; i<throws_num; i++){
        a=rand()%6;
        a+=rand()%6;
        distrib[a]+=1.0/throws_num;
    }
}

// Oblicza i zwraca Dystrybuantê (Cumulative Distribution Function)
void cum_discret_distrib(double distrib[], size_t n) {
    for (size_t i=1; i<n; i++) distrib[i]+=distrib[i-1];
}

// Histogram - wykres "s³upkowy" wartoœci funkcji zapisanych w tablicy tab o d³ugoœci n,
// x_start - wartoœæ pierwszej danej (za³o¿enie: przyrost zmiennej niezale¿nej=1),
// y_scale - wartoœæ zmiennej zale¿nej odpowiadaj¹cej szerokoœci jednego znaku na wykresie,
void print_histogram(const double tab[], size_t n, int x_start, double y_scale, char mark) {
    for (size_t i=0; i<n; i++){
        if (x_start+i<10) printf(" %d|",x_start+i);
        else printf("%d|",x_start+i);
        for (int j=0; j<round(tab[i]/y_scale); j++) printf("%c",mark);
        printf(" %.3f\n",tab[i]);
    }
}


// Symuluje problem Monty'ego-Hall'a dla przypadku 3-bramkowego
// Wejœcie: adres licznika wygranych przy zmianie wyboru,
//          adres licznika wygranych bez zamiany,
//          liczba gier (prób).
// Funkcja oblicza ile razy wygrywaj¹cym wyborem by³aby zmiana decyzji
// i ile razy wygrywa³aby opcja przeciwna.
// Za³o¿enie: Funkcja wywo³uje generator rand dok³adnie 2 razy.
//            Pierwsze losowanie: nr drzwi, za którymi jest nagroda.
//            Drugie losowanie: nr drzwi w pierwszym wyborze gracza.
void monty_hall(int *p_switch_wins, int *p_nonswitch_wins, int n) {
    int reward,chosen;
    *p_switch_wins=*p_nonswitch_wins=0;
    for (int i=0; i<n; i++){
        reward=rand()%3;
        chosen=rand()%3;
        if (chosen==reward) *p_nonswitch_wins+=1;
        else *p_switch_wins+=1;
    }
}

// read double vector of size n
void read_vector_dbl(double tab[], size_t n) {
    for (size_t i = 0; i < n; ++i) {
        scanf("%lf", tab + i);
    }
}

// print double vector of size n (with 2 figures after the decimal point)
void print_vector_dbl(const double tab[], size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        printf("%.3f ", tab[i]);
    }
    printf("\n");
}

// print integer vector
void print_vector_int(const int tab[], size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        printf("%d ", tab[i]);
    }
    printf("\n");
}

// get from input stream one character ignoring leading whitespace
char get_char_ignore_w_sp(void) {
    int c;
    do  c = getchar();
    while(isspace(c));
    return c;
}

int main(void) {
    int to_do, seed, number, itab[N_MAX];
    size_t n;
    double dtab[N_MAX], two_dice_tab[11];
    char mark;
    if (TEST) printf("Numer testu [1, 7]: ");
    scanf("%d", &to_do);
    if(to_do > 1) {
        if(TEST) printf("seed = ");
        scanf("%d",&seed);
        srand(seed);        // ustawienie ziarna dla powterzalnosci wynikow
    }

    switch(to_do) {
    case 1: // aver_varian
        if (TEST) printf("aver_varian: Wpisz dlugosc tablicy: ");
        scanf("%d", &n);
        if(n < 1 || n > N_MAX) {
            printf("n < 1 lub n > N_MAX\n");
            break;
        }
        { double arith_aver, varian;
            if (TEST) printf("Wpisz elementy tablicy: ");
            read_vector_dbl(dtab, n);
            aver_varian(dtab, n, &arith_aver, &varian);
            if (TEST) printf("œrednia arith_aver = ");
            printf("%.3f ", arith_aver);
            if (TEST) printf(" wariancja = ");
            printf("%.3f\n ", varian);
        }
        break;
    case 2: // bernoulli_gen
        if (TEST) printf("Liczba prób: ");
        scanf("%d", &n);
        if(n < 0 || n > N_MAX) {
            printf("n < 0 lub n>%d\n",N_MAX);
            break;
        }
        { double probab;
            if (TEST) printf("Prawdopodobieñstwo sukcesu: ");
            scanf("%lf", &probab);
            if(probab < 0 || probab > 1) {
                printf("p<0 lub p>1\n");
                break;
            }
            bernoulli_gen(itab, n, probab);
            print_vector_int(itab, n);
        }
        break;
    case 3:  // probab_distrib i tablica
    case 5:  // probab_distrib i histogram
        if (TEST) printf("Liczba prób: ");
        scanf("%d", &number);
        if(number <= 0 || number > N_MAX) {
            printf("number <= 0 lub number > N_MAX\n");
            break;
        }
        two_dice_probab_distrib(two_dice_tab, number);
        if(to_do==3)  print_vector_dbl(two_dice_tab, 11);
        else {
            if(TEST) printf("Wpisz znak: ");
            mark = get_char_ignore_w_sp();
            print_histogram(two_dice_tab,11,2,0.005,mark);
        }
        break;
    case 4: case 6:  // var_dystryb i hist
        if (TEST) printf("Liczba rzutów dwiema kostkami: ");
        scanf("%d", &number);
        if(number < 0 || number > N_MAX) {
            printf("number <= 0 lub number > N_MAX\n");
            break;
        }
        two_dice_probab_distrib(two_dice_tab, number);
        cum_discret_distrib(two_dice_tab, 11);
        if(to_do==4)  print_vector_dbl(two_dice_tab, 11);
        else {
            if(TEST) printf("Wpisz znak: ");
            mark = get_char_ignore_w_sp();
            print_histogram(two_dice_tab,11,2,0.02,mark);
        }
        break;
    case 7: // monty_hall
        if (TEST) printf("Wpisz liczbê prób: ");
        scanf("%d", &number);
        if(number <= 0) {
            printf("number<=0\n");
            break;
        }
        { int switch_wins, nonswitch_wins;
            monty_hall(&switch_wins, &nonswitch_wins, number);
            if (TEST) printf("W %d grach zwyciêstwo dawa³a zmiana wyboru drzwi ", number);
            printf("%d ", switch_wins);
            if (TEST) printf("razy,\na po pozostawienie wyboru drzwi bez zmiany  ");
            printf("%d ", nonswitch_wins);
            if (TEST) printf("razy.");
        }
        break;
    default:
        printf("Numer testu spoza zakresu [1, 7] %d", to_do);
        break;
    }
    return 0;
}
