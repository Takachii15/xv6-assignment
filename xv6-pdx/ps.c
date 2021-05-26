#ifdef CS333_P2

#define MAX_ENTRIES 16

#include "types.h"
#include "user.h"
#include "uproc.h"

int
main(int argc, char *argv[])
{
  struct uproc* temp;

  temp = malloc(sizeof(struct uproc) * MAX_ENTRIES);

  if (getprocs(MAX_ENTRIES, temp) < 0) {
    printf(2,"Error: getprocs call failed. %s at line %d\n",
        __FILE__, __LINE__);
    exit();
  }

  printf(1, "\nPID\tName\t\tUID\tGID\tPPID\tElapsed\tCPU\tState\tSize\n");

  for (int i = 0; i < MAX_ENTRIES; i++){
    if (temp[i].pid == NULL)
      break;
    printf(1, "%d\t%s\t\t%d\t%d\t%d\t", 
      temp[i].pid,
      temp[i].name,
      temp[i].uid,
      temp[i].gid,
      temp[i].ppid
    );

    // Special case for floating point
    // Elapsed Ticks
    if (temp[i].elapsed_ticks < 10){
      printf(1, "0.00%d\t", temp[i].elapsed_ticks);
    } else if (temp[i].elapsed_ticks < 100){
      printf(1, "0.0%d\t", temp[i].elapsed_ticks);
    } else if (temp[i].elapsed_ticks < 1000){
      printf(1, "0.%d\t", temp[i].elapsed_ticks);
    }else{
      printf(1, "%d.%d\t", temp[i].elapsed_ticks/1000, temp[i].elapsed_ticks%1000);
    }

    // CPU Total Ticks
    if (temp[i].CPU_total_ticks < 10){
      printf(1, "0.00%d", temp[i].CPU_total_ticks);
    } else if (temp[i].CPU_total_ticks < 100){
      printf(1, "0.0%d", temp[i].CPU_total_ticks);
    } else if (temp[i].CPU_total_ticks < 1000){
      printf(1, "0.%d", temp[i].CPU_total_ticks);
    }else{
      printf(1, "%d.%d", temp[i].CPU_total_ticks/1000, temp[i].CPU_total_ticks%1000);
    }

    printf(1, "\t%s\t%d\n",
      temp[i].state,
      temp[i].size
    );
  }

  printf(1, "\n");
  exit();
}

#endif // CS333_P2
