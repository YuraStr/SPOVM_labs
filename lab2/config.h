#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


class Config
{
	private:
		pid_t pid;	
		int log_level;
		int child_count;
		std::vector<pid_t> child_pids;

	public:
		Config();
		void createProcess();
		void deleteLastProcess();
		void increaseLogLevel();
		void decreaseLogLevel();
		~Config();
};