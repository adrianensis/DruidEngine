#include "Core/TimeUtils.hpp"
#include "Test/Test.hpp"
#include "Memory/Memory.hpp"

int main() {

	test(DE::Time);

	/*DE::Time::init();

	test_expected_float(DE::Time::getDeltaTimeMillis(), 0.0f);

	DE::Time::tick();

	test_expected_float(DE::Time::getDeltaTimeMillis(), 0.0f);

	DE::Time::tick();

	// waste some time in this loop
	int a = 0;
	while (DE::Time::getElapsedTime() < 300) {
		a++;
	}

	DE::Time::tick();

	test_show(DE::Time::getDeltaTimeMillis());

	test_expected_bool(DE::Time::getDeltaTimeMillis() > 0.0f, true);

	DE::Time::tick();

	test_tick(); // Starts measuring time for test

	f32 msCount = 200.0f;
	while (DE::Time::getElapsedTime() < msCount) {
		//test_show(DE::Time::getElapsedTime());
	}

	test_tock(); // Ends measuring time for test

	test_show(DE::Time::getElapsedTime());
	test_expected_bool(DE::Time::getElapsedTime() > 0.0f, true);*/

	summary();

	return 0;
}
