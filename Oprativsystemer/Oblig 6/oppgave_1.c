
#include <stdio.h>
#include <pthread.h> // For pthread funksjoner (threads and mutex)

int count = 0; 
pthread_mutex_t lock; // Dette er mutexen (eller låsen). Den må initieres før bruk.

void *increase()
{ 
   int i; 
   for (i = 0; i < 1e8; ++i) {
      pthread_mutex_lock(&lock); // Her låser vi mutexen før vi går inn i kritisk område. Dette gjør jeg for å unngå at to tråder skal endre count samtidig.
      count++;                   
      pthread_mutex_unlock(&lock); // Låser de  n opp igjen etter Count++ er ferdig.
   }
   return NULL;
}

void *decrease()
{  
   int i;
   for (i = 0; i < 1e8; ++i) {
      pthread_mutex_lock(&lock); // Samme som ovenfor. Låser mutexen før vi går inn i kritisk område.
      count--;
      pthread_mutex_unlock(&lock); // Låser den opp igjen etter Count-- er ferdig.
   }
   return NULL;
}

int main()
{ 
   pthread_t thread1, thread2; 
    
   pthread_mutex_init(&lock, NULL); // Initialiserer mutexen før bruk.

   pthread_create(&thread1, NULL, increase, NULL); // Lager to tråder som kjører increase og decrease funksjonene.
   pthread_create(&thread2, NULL, decrease, NULL);
   
   pthread_join(thread1, NULL);  // Venter på at begge trådene skal bli ferdige før vi går videre i main.
   pthread_join(thread2, NULL); 

   pthread_mutex_destroy(&lock); // Ødelegger mutexen når vi er ferdige med den.
  
   printf("count = %d\n", count);
   return 0;
}