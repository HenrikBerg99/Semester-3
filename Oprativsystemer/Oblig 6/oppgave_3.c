#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Mye kommentarer denne gangen også 😃

#define MAX_WAIT 2000000 // Maks ventetid i microsekunder

pthread_mutex_t mutex; // Lås (gjensidig utelukkelse)
pthread_cond_t cond; // Betinget variabel
int blocked = 0; // Teller hvor mange tråder som er blokkert

void *terningkast(void *nr)
{
    long id = (long) nr; // Tråd-ID (fra parameter)

 
    while (1) { // Uendelig løkke (while(1) {})
        int terning = rand() % 6 + 1; // Kaster et tall fra 1 til 6

        pthread_mutex_lock(&mutex); // Låser mutix får vi skrivet ut eller endrer noe 

        if (terning == 1) {
            blocked++; // Øk teller for hvor mange av 1 som blir blokkert 
            printf("Tråd %3ld -> 1 (%d)\n", id, blocked);
            
            pthread_cond_wait(&cond, &mutex); // pthread_cond_wait slipper mutexen OG setter tråden i ventemodu
            
            blocked--; // Nå reduserer vi blocked, fordi vi ikke er blokkert lenger
        }
        else if (terning == 6) { // Øk teller for hvor mange av 1 som blir blokkert 
        
            int after = (blocked > 0) ? blocked - 1 : 0; // Beregn hvor mange som vil være igjen etter signalet | (betingelse) ? verdi_hvis_sann : verdi_hvis_usann
            printf("Tråd %3ld -> 6 (%d)\n", id, after);

            // Denne sender signalet hvis den har registrert noen
            if (blocked > 0) {
                pthread_cond_signal(&cond);
            }
        }
        else {
            printf("Tråd %3ld -> %d\n", id, terning); // For terning 2 til 5, bare skriv ut resultatet
        }
        
        pthread_mutex_unlock(&mutex); // Låser opp mutex igjen
        usleep(rand() % MAX_WAIT + 1);
    }

    return NULL;
}

int main()
{
    int n; // Antall tråder fra brukerinput
    printf("n? "); // Spør brukeren hvor mange threads han vil kjøre
    scanf("%d", &n);

    // Sett "seed" for rand() så kastene blir forskjellige hver gang
    srand(time(NULL));

    // Initialiser mutex og betinget variabel
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Opprett n tråder
    pthread_t threads[n];
    for (long i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, terningkast, (void *) i);
    }

    // Vent på trådene (disse stopper aldri pga while(1), så programmet kjører evig)
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    // Rydd opp (egentlig aldri nådd her pga evig løkke)
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}