#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <chrono>

//Using specific object types and functions from the std namespace.
using std::string;
using std::vector;
using std::set;
using std::mutex;
using std::condition_variable;
using std::lock_guard;
using std::unique_lock;
using std::ofstream;
using std::fstream;
using std::thread;
using std::chrono::seconds;
using std::this_thread::sleep_for;
using std::getline;
using std::cout;
using std::cin;
using std::endl;

//setting define values for menu options and strings.
#define SIGN_IN 1
#define SIGN_OUT 2
#define CONNECTED_USERS 3
#define EXIT 4
#define MENU "\n1. Sign in\n2. Sign out\n3. Connected users\n4. Exit\n"
#define NAME_TAKEN_ERROR "ERROR: This name is already taken."
#define NAME_INPUT "Please enter your name:  "
#define NAME_EXIST_ERROR "ERROR: This name dosen't exist."
#define TAKING_INPUT "\nYour choice:  "
#define REMOVING_SUCCESS "\nUser Signed out successfully"
#define ADDING_SUCCESS "\nUser Signed in successfully"
#define CHOICE_ERROR "ERROR: Invalid choice. please try again."
#define EXIT_MESSAGE "\nThank You For Using MessageSender!"
#define DATA_PATH "data.txt"
#define OUTPUT_PATH "output.txt"

//Colors for program look's
#define CYAN "\033[1;1;36m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m" 
#define RESET_COLOR "\033[0m"

class MessageSender
{
public:
	void showMenu();
	void readAdminFile();
	void writeMessagesToUsersFile();

private:
	vector<string> _data;
	set<string> _users;
};