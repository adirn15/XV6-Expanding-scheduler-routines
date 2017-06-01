#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char * argv[]) {
    if(argc!=2){
        printf(1,"ERROR updating policy \n");
    }
    else {
    	int p=atoi(argv[1]);
        policy(p);
    }
    exit(0);
}