// This was a fun one!
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for wait(NULL);
#include <signal.h>
 
void ChildBehavior(int iChildID){
    printf("I am number %d\n", iChildID);
}
 
void SpawnFork(int iDontRemember){
    static int iSpawnID = 1;
    if(iSpawnID < 3) // only call 3x
        alarm(1);
 
    if(!fork()){ // No need to track PIDs for implementation
        ChildBehavior(iSpawnID); // Zero -> I'm the child
        exit(iSpawnID);
    } else { // Parent
        iSpawnID++;
    }
}
 
int main(){
    int iChildReturn;
    // Set up the alarm
    signal(SIGALRM, SpawnFork);
    alarm(1); // One second for first spawn
   
    do{ // Loop until child process returns 2
        iChildReturn = wait(NULL);
    } while(iChildReturn != 2);
   
    // We're done!
    printf("Go Tigers!\n");
    return 0;
}