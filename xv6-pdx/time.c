#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int curr_time = uptime();

  if (argc == 1){
      int temp_left = 0;
      int now = uptime() - curr_time;
      if(now > 1000){
        temp_left = now % 1000;
        now /= 1000;
    }
      // saya capek pak
      if(temp_left != 0){
        printf(1, "(null) ran in %d.%d seconds\n", now, temp_left);
    } else{
        printf(1, "(null) ran in 0.%d seconds\n", now);
     }
    } else {
        if (fork() == 0){
            exec(argv[1], &argv[1]);
        }else{
            wait();
             int temp_left = 0;
             int now = uptime() - curr_time;
            
            if(now > 1000){
            temp_left = now % 1000;
            now /= 1000;
            }
            if(temp_left != 0){
            printf(1, "%s ran in %d.%d seconds\n", argv[1], now, temp_left);
            } else{
            printf(1, "%s ran in 0.%d seconds\n", argv[1], now);
            }
        }
    }

  exit();
}
