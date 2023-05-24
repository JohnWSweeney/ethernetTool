#include "start.h"

int main()
{
	std::cout << "ethernetTool v1.0.1" << '\n';
	bool running = true;
	std::vector<std::string> tokens;
	while (running)
	{
		getInput(tokens);
		selectThread(running, tokens);
	}
}