#include "Core/Time/TimeUtils.hpp"
#include "Test/Test.hpp"

int main()
{
	test(Time);

	/*Time::init();

	test_expected_float(Time::getDeltaTimeMillis(), 0.0f);

	Time::tick();

	test_expected_float(Time::getDeltaTimeMillis(), 0.0f);

	Time::tick();

	// waste some time in this loop
	int a = 0;
	while (Time::getElapsedTime() < 300){
		a++;
	}

	Time::tick();

	test_show(Time::getDeltaTimeMillis());

	test_expected_bool(Time::getDeltaTimeMillis() > 0.0f, true);

	Time::tick();

	test_tick(); // Starts measuring time for test

	f32 msCount = 200.0f;
	while (Time::getElapsedTime() < msCount){
		//test_show(Time::getElapsedTime());
	}

	test_tock(); // Ends measuring time for test

	test_show(Time::getElapsedTime());
	test_expected_bool(Time::getElapsedTime() > 0.0f, true);*/

	summary();

	return 0;
}
