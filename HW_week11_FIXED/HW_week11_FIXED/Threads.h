#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using std::ref;
using std::cout;
using std::endl;
using std::thread;
using std::vector;
using std::string;
using std::mutex;

void I_Love_Threads();
void call_I_Love_Threads();

void printVector(vector<int> primes);

void getPrimes(int begin, int end, vector<int>& primes);
vector<int> callGetPrimes(int begin, int end);


void writePrimesToFile(int begin, int end, std::ofstream& file);
void callWritePrimesMultipleThreads(int begin, int end, string filePath, int N);
