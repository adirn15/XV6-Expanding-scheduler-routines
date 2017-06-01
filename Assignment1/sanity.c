#include "types.h"
#include "stat.h"
#include "user.h"

void sanity(int pl, int* ta, int* wt);
void check();
int fib(int n);

struct perf{
  uint ctime;                   // – process creation time
  uint ttime;                   //– process termination time
  uint stime;                   //– the time the process spent in the SLEEPING state
  uint retime;                  //– the time the process spent in the READY state
  uint rutime;                  //– the time the process spent in the RUNNING state
};

int main(int argc, char * argv[]) {
        
    if(argc!=1){
        printf(1,"ERROR initalizing sanity: insufficent args\n");
        return -1;
    }
    int rounds=3;

    int average_waiting=0;
    int average_turnaround=0;
    int i;

    for (i=1; i<=3; i++){ // 3 POLICYS
        int best_waiting=-1;
        int best_turna=-1;
        int j;
        for (j=0; j<rounds; j++){ 
            sanity(i,&average_turnaround,&average_waiting);
    		if (best_waiting==-1 || (best_waiting>=average_waiting && best_turna>=average_turnaround)){  
                best_waiting=average_waiting;
                best_turna=average_turnaround;
            }
            average_waiting=0;
    		average_turnaround=0;
        }
        printf(1,"********************************************\n\n");
        printf(1, "Total Results for policy %d:\n\n",i);
        printf(1, "Average Turnaround: %d\n\n",best_turna);
        printf(1, "Average Waiting Time: %d\n\n",best_waiting);
        printf(1,"********************************************\n\n");
    }
    return 1;
}

void check(){
    struct perf performance;
    int pid = fork();
    if (pid<0){
        printf(1,"error forking -exiting\n");
        return;
    }
    if(pid==0){
            sleep(10);
            fib(30);
            sleep(50);
            exit(0);
        }
    if(pid>0){
        wait_stat(0,&performance);
        printf(1," %d %d %d %d %d \n",performance.ctime,performance.ttime, performance.stime,performance.retime,performance.rutime);
    }

}
int fib(int n){
    if(n<=0)
        return 0;
    if(n==1)
        return 1;
    else
        return fib(n-1)+fib(n-2);
}


void sanity(int pl, int* ta, int* wt){
    int forks=5;
    struct perf performance;
    int pid;
    int turnaround=0;
    int waiting=0;
    int i;

    policy(pl); //change to current policy 

    for (i = 0; i < forks; ++i){
        pid = fork();
        if (pid<0){
            printf(1,"error forking -exiting\n");
            return;
        }
        if(pid==0){ //son
                int pid2=fork();
                if (pid2<0){
                    printf(1,"error forking -exiting\n");
                    return;
                }
                else if (pid2==0){
                    int k;
                    //for odd process only cpu time
                    if (getpid()%2==1){
                        //computations with blocking
                        if (pl==2)
                          priority(100); //higher priority for odd processes
                        //computations for cpu
                        for (k=0; k<7; k++){
                            fib(30);
                        }
                    }
                    //for even processes blocking time
                    else  { //getpid%2==0

                        for (k=0; k<8; k++){
                            sleep(50);
                            fib(4);
                        }
                    }
                    exit(0); 
                }
                else{
                     int child = wait_stat(0,&performance);
                     turnaround = performance.ttime - performance.ctime;
                     waiting = performance.stime + performance.retime;
                     *wt+=waiting;
                     *ta+=turnaround;
                     printf(1," child %d: ctime: %d ttime: %d stime: %d retime: %d rutime: %d\n",child,performance.ctime,performance.ttime,performance.stime,performance.retime,performance.rutime);
                     exit(0);
                }
    
        }  
    }
    for (i = 0; i < forks; ++i){ //wait for all sons
         int child = wait_stat(0,&performance);
         turnaround = performance.ttime - performance.ctime;
         waiting = performance.stime + performance.retime;
         *wt+=waiting;
         *ta+=turnaround;
         printf(1," child %d: ctime: %d ttime: %d stime: %d retime: %d rutime: %d\n",child,performance.ctime,performance.ttime,performance.stime,performance.retime,performance.rutime );
    }
    *ta/=forks*2;
    *wt/=forks*2;

    return;
}