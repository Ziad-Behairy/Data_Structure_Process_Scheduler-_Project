#include "Schedular.h"
int main()
{
	Schedular Schedul;
	cout << "chose 1 for the interactive mode" << endl;
	cout << "chose 2 for the step_bystep_mode" << endl;
	cout << "chose 3 for the silentmode" << endl;
	int mode;
	cin >> mode;
	if (mode == 1)
	{
		Schedul.Interactive_Mode();
	}
	if (mode == 2)
	{
		// Schedul.step_bystep_mode();
	}
	if (mode == 3)
	{
		// Schedul.silentmode();
	}
	return 0;
}