#include <iostream>
#include "config.h"

void sigusr1_handler(int i);
void sigusr2_handler(int i);
void set_signals();

Config *conf;

int main()
{
  int sig;
	printf("Parent pid = %d\n", getpid());

	conf = new Config();

	set_signals();

	sigset_t termset;
	sigemptyset(&termset);
	sigaddset(&termset, SIGTERM);
	sigprocmask(SIG_BLOCK, &termset, NULL);

	while(!sigwait(&termset, &sig)) {
		printf("Terminating\n");
		delete conf;
		return 0;	
	}
}


void sigusr1_handler(int i)
{
	conf->createProcess();
	conf->increaseLogLevel();
}

void sigusr2_handler(int i)
{
	conf->deleteLastProcess();
	conf->decreaseLogLevel();
}

void set_signals() {
	sigset_t set_usr1;
	sigset_t set_usr2;

	sigemptyset(&set_usr1);
	sigemptyset(&set_usr2);
	sigaddset(&set_usr1, SIGUSR1);
	sigaddset(&set_usr2, SIGUSR2);

	struct sigaction sigusr1;
	struct sigaction sigusr2;

	sigusr1.sa_handler = sigusr1_handler;
	sigusr1.sa_mask = set_usr1;
	sigusr1.sa_flags = SA_RESTART | SA_SIGINFO;

	sigusr2.sa_handler = sigusr2_handler;
	sigusr2.sa_mask = set_usr2;
	sigusr2.sa_flags = SA_RESTART | SA_SIGINFO;

	sigaction(SIGUSR1, &sigusr1, NULL);
	sigaction(SIGUSR2, &sigusr2, NULL);
}