#include "threads.h"

std::condition_variable cond;
mutex insertToFileLock;
bool firstFinish = false;

void wait_for_the_other_thread();

/*
Function print's "I love threads"
Input: none
Output: none
*/
void I_Love_Threads()
{ 
    cout << "I Love Threads" << endl;
}

/*
Function uses a thread to call the function 'I_Love_Threads'.
Input: none
Output: none
*/
void call_I_Love_Threads()
{
    thread t(I_Love_Threads);
    t.join();
}

/*
Function print's the content of a given vector.
Input: vector<int> primes
Output: none
*/
void printVector(vector<int> primes)
{
    for (auto it = primes.begin(); it != primes.end(); it++)
    {
        cout << *it << endl;
    }
}

/*
Function checks for prime numbers in a given range, and adds the found ones to the given vector.
Input: int begin, int end, vector<int>& primes
Output: none
*/
void getPrimes(int begin, int end, vector<int>& primes)
{
    bool primeCheck = true;

    //looping all the numbers between 'begin' to 'end'.
    for (int i = begin; i <= end; i++)
    {
        //reseting isPrime each loop.
        primeCheck = true;

        if (i == 0 || i == 1)
        {
            primeCheck = false;
        }

        else
        {
            //checking if the number is not prime. if it is not, changing isPrime to false.
            for (int j = 2; j <= sqrt(i); ++j)
            {
                if (i % j == 0)
                {
                    primeCheck = false;
                    break;
                }
            }
        }

        //if the number is prime, adding it to the primes vector.
        if (primeCheck)
        {
            primes.push_back(i);
        }
    }
}

/*
Function uses a thread to call the function 'getPrimes'.
Input: int begin, int end
Output: vector<int> primes (with the results)
*/
vector<int> callGetPrimes(int begin, int end)
{
    vector<int> primes; //Creating the prime numbers vector.

    time_t start, finish; //setting time_t variables.

    time(&start); //Starting the timer (getting the current time).

    thread t(getPrimes, ref(begin), ref(end), ref(primes)); //Activating a thread with the 'getPrimes' function.

    t.join(); //waiting for the thread to finish his action (blocking).

    time(&finish); //Stopping the timer (getting the current time).

    double duration = finish - start; //Calculating the duration of the program's running time (ending time - starting time)

    cout << "Time (in seconds) for the program to finish:  " << duration << endl; //(printing the runing time of the function using a thread).

    return primes; //Returning the vector with the results.
}

/*
Function write's all of the numbers in a given range to a file.
Input: int begin, int end, std::ofstream& file
Output: none
*/
void writePrimesToFile(int begin, int end, std::ofstream& file)
{
    bool isLocked;
    //setting the primes vector with all primes in range.
    vector<int> primes = callGetPrimes(begin, end);

    //if the file is open, writing the vector's content to the file.
    if (file.is_open())
    {
        wait_for_the_other_thread();
        for (int i = 0; i < primes.size(); i++)
        {
            insertToFileLock.lock();
            file << primes[i] << "\n";
            insertToFileLock.unlock();
        }
    }
}

/*
Function calls 'writePrimesToFile' using a selected amount of threads.
Input: int begin, int end, string filePath, int N
Output: none
*/
void callWritePrimesMultipleThreads(int begin, int end, string filePath, int N)
{
    //setting time_t variables.
    time_t start, finish;

    //opening the file in the given path.
    std::ofstream file;
    file.open(filePath);

    //creating an array of N threads.
    thread* threadsArr = new std::thread[N]; 

    //deviding the work between the threads so that each of them will check a different range.
    int equalWork = (end - begin) / N;
    int reminder = (end - begin) % N;


    time(&start); //Starting the timer (getting the current time).

    //looping to activate each thread with a different range for the task.
    for (int i = 0; i < N; i++)
    {
        //(last thread will check the reminder of the numbers)
        if (i == N - 1)
        {
            threadsArr[i] = thread(writePrimesToFile, begin, end + reminder, ref(file));
        }
        //each tread besides the last one will check equal amount of numbers.
        else
        {
            threadsArr[i] = thread(writePrimesToFile, begin, begin + equalWork, ref(file));
        }
        
        begin =+ equalWork;
    }

    //using join on each thread of the array (blocking).
    for (int i = 0; i < N; i++)
    {
        threadsArr[i].join();
    }

    time(&finish); //Stopping the timer (getting the current time).

    double duration = finish - start; //Calculating the duration of the program's running time (ending time - starting time)

    cout << "Time (in seconds) for all threads to finish:  " << duration << endl;

    file.close(); //closing the file.
}

/*
Function make's a thread wait for another thread to finish his action, before letting this thread continue, using a condition variable.
Input: none
Output: none
*/
void wait_for_the_other_thread()
{
    std::unique_lock<std::mutex> lock(insertToFileLock);
    if (firstFinish)
    {
        firstFinish = false;
        cond.notify_all();
    }
    else
    {
        firstFinish = true;
        cond.wait(lock);
    }
}