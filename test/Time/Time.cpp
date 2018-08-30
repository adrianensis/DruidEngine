#include "Druid.h"

int main() {

	test(DE::Time);

	DE::Time::init();

	expected_float(DE::Time::getDeltaTime(), 0.0f);

	// expected_float_eps(DE::Vector2(a).clamp(0.1f).len(),0.1f,0.1f);
	// expected_float_eps(DE::Vector2(a).clamp(0.2f).len(),0.2f,0.1f);
	// expected_float_eps(DE::Vector2(a).clamp(0.3f).len(),0.3f,0.1f);
	// expected_float_eps(DE::Vector2(a).clamp(0.4f).len(),0.4f,0.1f);
	// expected_float_eps(DE::Vector2(a).clamp(0.5f).len(),0.5f,0.1f);

	DE::Time::tick();

	expected_bool(DE::Time::getDeltaTime() > 0.0f, true);

	DE::Time::tick();

	f32 msCount = 200.0f;
	while (DE::Time::getElapsedTime() < msCount) {
		expected_bool(DE::Time::getElapsedTime() >= 0.0f, true);
	}

	summary();

	return 0;
}
