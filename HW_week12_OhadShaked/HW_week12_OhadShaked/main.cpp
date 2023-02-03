#include "MessageSender.h"

int main()
{
	//creating a MessageSender variable.
	MessageSender program;

	//setting threads for the reading and writing of the files.
	thread readData(&MessageSender::readAdminFile, &program);
	thread writeOutput(&MessageSender::writeMessagesToUsersFile, &program);

	//entering the program menu.
	program.showMenu();

	//waiting for threads to finish (blocking)
	readData.join();
	writeOutput.join();

	//godbye message to the user.
	cout << GREEN << EXIT_MESSAGE << RESET_COLOR << endl;

	return 0;
}