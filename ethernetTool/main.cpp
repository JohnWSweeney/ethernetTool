#include "start.h"

int main()
{
	std::cout << "ethernetTool v1.2.1\n\n";
	bool running = true;
	std::vector<std::string> tokens;
	while (running)
	{
		getInput(tokens);
		startMenu(running, tokens);
	}
}