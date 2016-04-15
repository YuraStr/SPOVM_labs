#include "config.h"


Config::Config()
{
	std::fstream fstr;
	fstr.open("config.conf", std::ios_base::in);
	if (!fstr.is_open()) {
		std::cout << "File cannot be open!" << std::endl;
		exit(1);
	}

	char buf[40];
	// skip first line
	fstr.getline(buf, 40);		

	pid = getpid();

	int child;
	do {
		fstr.getline(buf, 40, '=');

		if (!strcmp(buf, "LOG_LEVEL")) {
			fstr.getline(buf, 40, '\n');
			if (!strcmp(buf, "DEBUG")) log_level = 0;
			else if (!strcmp(buf, "ERROR")) log_level = 1;
			else if (!strcmp(buf, "CRITICAL")) log_level = 2;
		}

		if (!strcmp(buf, "CHILD_COUNT")) {
			fstr.getline(buf, 40, '\n');
			child = atoi(buf);
		}
	} while(!fstr.eof());

	for (int i = 0; i < child; i++)
		createProcess();

	child_count = child;

	fstr.close();
}


void Config::createProcess()
{
	pid_t pid = fork();
	switch (pid) {
		case -1: {
				std::cout << "Cannot create process!" << std::endl;
				exit(1);
		}
		case 0: {
			execl("child", "child", NULL);
		}
		default:
			child_pids.push_back(pid);
			++child_count;	
			break;
	}
	printf("Child count: %d\n", child_count);
	printf("Last pid: %d\n", child_pids.back());
}

void Config::deleteLastProcess() 
{
	if (child_pids.size() != 0) {
		printf("Process %d was terminated\n", child_pids.back());
		kill(child_pids.back(), SIGKILL);
		child_pids.pop_back();
		--child_count;
	}
}

void Config::increaseLogLevel()
{
	if (log_level != 2) {
		++log_level;
	}
}

void Config::decreaseLogLevel()
{
	if (log_level != 0) {
		--log_level;
	}
}

Config::~Config()
{
	std::fstream fstr;
	fstr.open("config.conf", std::ios_base::out | std::ios_base::trunc);
	if (!fstr.is_open()) {
		std::cout << "File cannot be open!" << std::endl;
		exit(1);
	}

	fstr << "PID=" << pid << std::endl;

	if (log_level == 0) fstr << "LOG_LEVEL=DEBUG\n";
	else if (log_level == 1) fstr << "LOG_LEVEL=ERROR\n";
	else if (log_level == 2) fstr << "LOG_LEVEL=CRITICAL\n";

	fstr << "CHILD_COUNT=" << child_pids.size() << std::endl;

	fstr.close();

	for (int i = child_pids.size() - 1; i >= 0; i--) {
		deleteLastProcess();
	}

	child_pids.clear();
}