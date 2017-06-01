#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char ** argv) {
    if(argc!=2){
        printf(1,"ERROR updating priority \n");
    }
    else {
        priority(atoi(argv[1]));
    }
    exit(0);
}