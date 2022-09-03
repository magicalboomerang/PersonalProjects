#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void ParentBehavior(int iChildPID);
void ChildBehavior(int iPassedValue);

int main(int iArgumentCount, char *pArgumentList[]){
	
	int iChildPID;;
	iChildPID = fork(); // 0 - child, >= 0 Created process ID
	
	if(iChildPID){
		// Parent function
		ParentBehavior(iChildPID);
		// KILL CHILD 0_0
		kill(iChildPID, SIGKILL);
	} else {
		// Child behavior
		signal(SIGUSR1, &ChildBehavior);
		while (1){};
	}
	
	return 0;
}

void ParentBehavior(int iChildPID){
	for(int iSignalIndex = 1; iSignalIndex < 10; iSignalIndex++){
		printf("Child, behave!\n");
		sleep(1);
		kill(iChildPID, SIGUSR1);
		sleep(1)
		kill(iChildPID, SIGUSR2);
	}
	printf("Goodnight, child!\n");
}

void ChildBehavior(int iPassedValue){
	printf("\tI am a child. See me behave as a child. I am going down a slide now. Wee! Thank you for bearing witness.\n");
	printf("\n\t\tOh... and I found this under the slide... *You aquired the number %d*\n", iPassedValue);
}