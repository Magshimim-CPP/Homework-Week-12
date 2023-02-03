#include "MessageSender.h"

//setting up global mutexes and a condition variable.
mutex dataMutex;
mutex usersMutex;
condition_variable cond;
bool keepLoop = true;

void checkInput();

/*
Menu navigation function for the message sender.
Input: none.
Output: none.
*/
void MessageSender::showMenu()
{
	string username;
	int option = 0;
	

	//looping the menu until user chooses to exit.
	while (keepLoop)
	{
		//taking input for an option from the menu.
		cout << MENU << TAKING_INPUT;
		cin >> option;
		checkInput(); //(checking the input is valid, and that the cin is clear).

		switch (option)
		{
			case (SIGN_IN):
			{
				//taking input for a name to add.
				cout << NAME_INPUT;
				cin >> username;

				if (this->_users.count(username)) //if the name already exist's, printing error.
				{
					cout << RED << NAME_TAKEN_ERROR << RESET_COLOR << endl;
				}

				else //if the name is new, adding to the users set.
				{
					lock_guard<mutex> LockUsersWriting(usersMutex);
					this->_users.insert(username);
					cout << GREEN << ADDING_SUCCESS << RESET_COLOR << endl;
				}

				break;
			}

			case (SIGN_OUT):
			{
				//taking input for a name to remove.
				cout << NAME_INPUT;
				cin >> username;

				if (this->_users.count(username)) //if the name exist's, removing him from _users.
				{
					lock_guard<mutex> LockUsersWriting(usersMutex);
					this->_users.erase(username);
					cout << GREEN << REMOVING_SUCCESS << RESET_COLOR << endl;
				}

				else //if the name dosen't exist, printing error.
				{
					cout << RED << NAME_EXIST_ERROR << RESET_COLOR << endl;
				}

				break;
			}

			case (CONNECTED_USERS): //listing all connected users.
			{
				//looping through all connected users (_users).
				cout << "\nConnected Users:" << endl;
				for (auto it = this->_users.begin(); it != this->_users.end(); it++)
				{
					cout << CYAN << *it << RESET_COLOR << endl;
				}
				break;
			}

			case (EXIT): //Exit the program.
			{
				cout << "\nPlease wait...\n";
				keepLoop = false;
				break;
			}

			default: //Invalid input.
			{
				checkInput();
				cout << RED << CHOICE_ERROR << RESET_COLOR << endl;
				break;
			}
		}
	}
}

/*
Function reads the content of data.txt to the _data variable line by line (as strings).
Input: none.
Output: none.
*/
void MessageSender::readAdminFile()
{
	fstream dataFile;
	string lineFromFile;
	ofstream clearFile;

	//running as long as the program is active, to read data.txt every 60 seconds.
	while (keepLoop)
	{
		sleep_for(seconds(60)); //waiting 60 seconds before each reading.

		dataFile.open(DATA_PATH); //opening the data file.

		unique_lock<mutex> LockDataWriting(dataMutex); //setting a unique lock for the data writing section (locking dataMutex).

		while (!dataFile.eof()) //writing the file's content to _data until we reach end of file.
		{
			getline(dataFile, lineFromFile);
			_data.push_back(lineFromFile);
		}

		LockDataWriting.unlock(); //unlocking dataMutex.

		cond.notify_one(); //notifying for one thread who waited for this condition variable.

		clearFile.open(DATA_PATH); //opening the file in writing mode, to clear it's content after reading.

		//closing the files.
		dataFile.close();
		clearFile.close();
		
	}
}

/*
Function is writing the content from _data to the output.txt file, with the username that recived the message.
Input: none.
Output: none.
*/
void MessageSender::writeMessagesToUsersFile()
{
	ofstream outputFile(OUTPUT_PATH); //opening the output.txt file.

	while (keepLoop) //running as long as the program is active, to write to output.txt every 60 seconds.
	{
		unique_lock<mutex> LockDataReading(dataMutex); //locking dataMutex for data reading.

		cond.wait(LockDataReading, [&]() {return !(_data.empty()); }); //waiting for LockDataReading, and for data not to be empety (lambda function)

		lock_guard<mutex> LockUsersReading(usersMutex); //locking the usersMutex before reading info from _users.

		for (int i = 0; i < _data.size(); i++) //looping through all the lines of data.txt (each index of _data).
		{
			for (auto it = _users.begin(); it != _users.end(); ++it) //writing a message to each user with the currently looped message.
			{
				if (_data[i] != "") //if the content is not just a blank line, writing to outputFile.
				{
					outputFile << (*it) << " : " << _data[i] << endl;
				}
			}
		}

		_data.clear(); //clearing the current content of data after writing to all users.

		LockDataReading.unlock(); //unlocking dataMutex.

		sleep_for(seconds(60)); //waiting 60 seconds before each writing.
	}
	outputFile.close(); //colsing the output file.
}

/*
Static function to check input errors (cin fail), in case user enterd a char to int etc..
Input: none
Output: none
*/
void checkInput()
{
	if (cin.fail()) //checking if there is a cin fail (input error)
	{
		cin.clear(); //clearing the input buffer
		cin.ignore(CHAR_MAX, '\n'); //ignoring cin input until '\n' (new line).
	}
}
