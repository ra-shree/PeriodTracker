#include <PeriodTracker.h>


namespace PeriodTracker
{
	class monthlyPeriod
	{
		// get the date chosen by the user
		// calculate the next date
		// calculate the ovulation week
		// create a vector of only the length of each period
		// pass the result to the GUI
	private:
		// So the file system can directly enter or get data from here.
		friend class periodFile;
	public:

		monthlyPeriod();
		~monthlyPeriod();
	};

}
