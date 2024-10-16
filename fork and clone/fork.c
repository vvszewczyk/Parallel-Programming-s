#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include"pomiar_czasu.h"

int zmienna_globalna=0;

int main()
{
  
  int pid, wynik, i;

  inicjuj_czas();
  
  for(i = 0; i < 10; i++)
  {
    pid = fork(); 			
    
    if(pid == 0)
    { 			
      
      zmienna_globalna++;
      char thread_id_str[10];
      snprintf(thread_id_str, sizeof(thread_id_str), "%d", i); //konwersja id na string: bufor, ilosc znakow, typ, numer
      char* args[] = {"./program", thread_id_str, NULL};
      wynik = execv(args[0], args);

      if(wynik == -1)
      {
        printf("Proces potomny nie wykonal programu\n");
      }

      exit(0);
      
    } 
    else 
    {					
      wait(NULL);
    }
    
  }
  drukuj_czas();
}

  
