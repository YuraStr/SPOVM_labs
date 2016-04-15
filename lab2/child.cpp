#include <unistd.h>
#include <iostream>

int main() 
{
	while (1) {
		std::cout << "Child " << getpid() << std::endl;
		sleep(1);
	}
}