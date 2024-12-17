#ifndef _czytelnia_
#define _czytelnia_

/*** Definicje typow zmiennych ***/
typedef struct {
  int writers; // liczba piszacych
  int readers; // liczba czytajacych
  // <- zasoby czytelni
} cz_t;

/*** Deklaracje procedur interfejsu ***/
void inicjuj(cz_t* czytelnia_p);
void czytam(cz_t* czytelnia_p);
void pisze(cz_t* czytelnia_p);

int my_read_lock_lock(cz_t* czytelnia_p);
int my_read_lock_unlock(cz_t* czytelnia_p);
int my_write_lock_lock(cz_t* czytelnia_p);
int my_write_lock_unlock(cz_t* czytelnia_p);
void debugging(cz_t* obj);

#endif
