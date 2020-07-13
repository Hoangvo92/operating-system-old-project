#include <iostream>

using namespace std;

int main()
{
	int quantum,pid,exectime,priority;
	pid = 0;
	cin >> quantum;
	cout << "Quantum = " << quantum << endl;
	while (cin >> exectime >> priority)
	{
		cout << "PID = " << pid << ", Exectime = " << exectime << ", Priority = " << priority << endl;
		pid++; 
	} 
	return 0;
}
