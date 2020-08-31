#include <TimeUtils.hpp>
#include "Test.hpp"
#include "Memory.hpp"

int main() {

	DE_test(DE::Time);

	DE::Time::init();

	DE_test_expected_float(DE::Time::getDeltaTimeMillis(), 0.0f);

	DE::Time::tick();

	DE_test_expected_float(DE::Time::getDeltaTimeMillis(), 0.0f);

	DE::Time::tick();

	// waste some time in this loop
	int a = 0;
	while (DE::Time::getElapsedTime() < 300) {
		a++;
	}

	DE::Time::tick();

	DE_test_show(DE::Time::getDeltaTimeMillis());

	DE_test_expected_bool(DE::Time::getDeltaTimeMillis() > 0.0f, true);

	DE::Time::tick();

	DE_test_tick(); // Starts measuring time for test

	f32 msCount = 200.0f;
	while (DE::Time::getElapsedTime() < msCount) {
		//DE_test_show(DE::Time::getElapsedTime());
	}

	DE_test_tock(); // Ends measuring time for test

	DE_test_show(DE::Time::getElapsedTime());
	DE_test_expected_bool(DE::Time::getElapsedTime() > 0.0f, true);

	summary();

	return 0;
}
