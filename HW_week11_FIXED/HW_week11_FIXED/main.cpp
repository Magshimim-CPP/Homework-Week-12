#include "threads.h"

#define NUM_OF_THREADS 10

int main()
{
	//call_I_Love_Threads();

	std::vector<int> primes1;
	//getPrimes(58, 100, primes1);
	//printVector(primes1);
	//printVector(callGetPrimes(58, 100));

	//callGetPrimes(0, 1000);
	//callGetPrimes(0, 100000);
	//callGetPrimes(0, 1000000);

	std::cout << NUM_OF_THREADS << " threads are running " << std::endl;
	//callWritePrimesMultipleThreads(0, 1000, "primes2.txt", NUM_OF_THREADS);
	//callWritePrimesMultipleThreads(0, 100000, "primes2.txt", NUM_OF_THREADS);
	callWritePrimesMultipleThreads(0, 1000000, "primes2.txt", NUM_OF_THREADS);

	//std::cout << "\nOnly one thread are running " << std::endl;
	//callWritePrimesMultipleThreads(0, 1000, "primes2.txt", 1);
	//callWritePrimesMultipleThreads(0, 100000, "primes2.txt", 1);
	//callWritePrimesMultipleThreads(0, 1000000, "primes2.txt", 1);


	system("pause");
	return 0;
}