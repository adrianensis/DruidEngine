#include "Test/Test.hpp"
#include "Memory/Memory.hpp"
#include "Maths/Vector3.hpp"
#include <cmath>

int main() {

	test(DE::Vector3);

	DE::Vector3 a(0, 1, 2);
	DE::Vector3 b(4, 5, 6);
	DE::Vector3 c(1, 2, 2);
	DE::Vector3 d;

	test_show(a);
	test_show(b);
	test_show(c);
	test_show(d);

	test_title("OPERATORS");

	test_expected(a + a, DE::Vector3(0 + 0, 1 + 1, 2 + 2));
	test_expected(a - a, DE::Vector3(0 - 0, 1 - 1, 2 - 2));
	test_expected(b * a, DE::Vector3(4 * 0, 5 * 1, 6 * 2));
	test_expected(b / DE::Vector3(1, 1, 2), DE::Vector3(4 / 1.0f, 5 / 1.0f, 6 / 2.0f));
	test_expected(DE::Vector3(a) -= a, DE::Vector3(0 - 0, 1 - 1, 2 - 2));
	test_expected(DE::Vector3(a) += a, DE::Vector3(0 + 0, 1 + 1, 2 + 2));
	test_expected(DE::Vector3(a) *= a, DE::Vector3(0 * 0, 1 * 1, 2 * 2));
	test_expected(DE::Vector3(a) /= DE::Vector3(1, 1, 2), DE::Vector3(0 / 1.0f, 1 / 1.0f, 2 / 2.0f));
	test_expected_bool(a == a, true);
	test_expected_bool(a == b, false);
	test_expected_bool(a != a, false);
	test_expected_bool(a != b, true);

	test_title("ARRAY ACCESS");

	test_expected_float(a[0], 0);
	test_expected_float(a[1], 1);
	test_expected_float(a[2], 2);

	test_title("FUNCTIONS");

	test_expected(DE::Vector3(a).add(a), DE::Vector3(0 + 0, 1 + 1, 2 + 2));
	test_expected(DE::Vector3(a).sub(a), DE::Vector3(0 - 0, 1 - 1, 2 - 2));
	test_expected(DE::Vector3(a).mul(DE::Vector3(2, 2, 2)), DE::Vector3(0 * 2, 1 * 2, 2 * 2));
	test_expected(DE::Vector3(a).div(DE::Vector3(2, 2, 2)), DE::Vector3(0 / 2.0f, 1 / 2.0f, 2 / 2.0f));
	test_expected(DE::Vector3(a).add(1), DE::Vector3(0 + 1, 1 + 1, 2 + 1));
	test_expected(DE::Vector3(a).sub(1), DE::Vector3(0 - 1, 1 - 1, 2 - 1));
	test_expected(DE::Vector3(a).mul(2), DE::Vector3(0 * 2, 1 * 2, 2 * 2));
	test_expected(DE::Vector3(a).div(2), DE::Vector3(0 / 2.0f, 1 / 2.0f, 2 / 2.0f));

	test_show(c.div(2));
	test_expected(c, DE::Vector3(1 / 2.0f, 2 / 2.0f, 2 / 2.0f));

	test_expected(DE::Vector3(b).lerp(a, 0.5f), DE::Vector3(b) += (a - b) * 0.5f);
	test_expected(DE::Vector3(b).lerp(a, 0.1f), DE::Vector3(b) += (a - b) * 0.1f);
	test_expected(DE::Vector3(b).lerp(a, 0.823f), DE::Vector3(b) += (a - b) * 0.823f);
	test_expected_float(a.dot(a), a.x * a.x + a.y * a.y + a.z * a.z);
	test_expected_float(a.dot(b), a.x * b.x + a.y * b.y + a.z * b.z);
	test_expected_float(a.sqrlen(), a.dot(a));
	test_expected_float(a.len(), sqrtf(a.sqrlen()));
	test_expected_float(a.max(), 2);
	test_expected_float(a.min(), 0);
	test_expected_float(a.sqrdst(b), (a - b).dot(a - b));
	test_expected_float(a.dst(b), sqrtf(a.sqrdst(b)));
	test_expected_bool(a.eq(b, 0), false);
	test_expected_bool(a.eq(b, 10), true);
	test_expected(DE::Vector3(a).nor(), DE::Vector3(a).div(a.len()));
	test_expected(DE::Vector3(1, 0, 0).cross(DE::Vector3(0, 1, 0)), DE::Vector3(0, 0, 1));
	test_expected(DE::Vector3(1, 0, 0).cross(DE::Vector3(0, -1, 0)), DE::Vector3(0, 0, -1));
	test_expected(DE::Vector3(-1, 0, 0).cross(DE::Vector3(0, 1, 0)), DE::Vector3(0, 0, -1));

	test_expected_float(a.angle(a), 0.0);
	test_expected_float(DE::Vector3(1, 0, 0).angle(DE::Vector3(0, 1, 0)) * (180 / MathUtils::PI), 90);
	test_expected_float(DE::Vector3(1, 0, 0).angle(DE::Vector3(0, -1, 0)) * (180 / MathUtils::PI), 90);
	test_expected_float_eps(DE::Vector3(1, 0, 0).angle(DE::Vector3(1, 1, 0)) * (180 / MathUtils::PI), 45, 0.00001f);
	test_expected_float_eps(DE::Vector3(1, 0, 0).angle(DE::Vector3(1, -1, 0)) * (180 / MathUtils::PI), 45, 0.00001f);
	test_expected_float_eps(DE::Vector3(-1, 0, 0).angle(DE::Vector3(-1, -1, 0)) * (180 / MathUtils::PI), 45,
			0.00001f);
	test_expected_float_eps(DE::Vector3(1, 0, 0).angle(DE::Vector3(-1, 0, 0)) * (180 / MathUtils::PI), 180, 0.00001f);
	test_expected_float(DE::Vector3(1, 0, 0).angle(DE::Vector3(-1, 0, 1)) * (180 / MathUtils::PI), 135);
	test_expected_float(DE::Vector3(1, 0, 0).angle(DE::Vector3(-1, 0, -1)) * (180 / MathUtils::PI), 135);

	test_expected_float(a.angle(a, DE::Vector3(0, 0, 1)), 0.0);
	test_expected_float(
			DE::Vector3(1, 0, 0).angle(DE::Vector3(0, 1, 0), DE::Vector3(0, 0, 1)) * (180 / MathUtils::PI), 90);
	test_expected_float(
			DE::Vector3(1, 0, 0).angle(DE::Vector3(0, -1, 0), DE::Vector3(0, 0, 1)) * (180 / MathUtils::PI), 270);
	test_expected_float(
			DE::Vector3(1, 0, 0).angle(DE::Vector3(1, 1, 0), DE::Vector3(0, 0, 1)) * (180 / MathUtils::PI), 45);
	test_expected_float(
			DE::Vector3(1, 0, 0).angle(DE::Vector3(1, -1, 0), DE::Vector3(0, 0, 1)) * (180 / MathUtils::PI), 315);
	test_expected_float(
			DE::Vector3(-1, 0, 0).angle(DE::Vector3(-1, -1, 0), DE::Vector3(0, 0, 1)) * (180 / MathUtils::PI), 45);
	test_expected_float(
			DE::Vector3(1, 0, 0).angle(DE::Vector3(-1, 0, 0), DE::Vector3(0, 1, 0)) * (180 / MathUtils::PI), 180);
	test_expected_float(
			DE::Vector3(1, 0, 0).angle(DE::Vector3(-1, 0, 1), DE::Vector3(0, 1, 0)) * (180 / MathUtils::PI), 225);
	test_expected_float(
			DE::Vector3(1, 0, 0).angle(DE::Vector3(-1, 0, -1), DE::Vector3(0, 1, 0)) * (180 / MathUtils::PI), 135);

	test_expected_float_eps(DE::Vector3(a).clamp(0.1f).len(), 0.1f, 0.00001f);
	test_expected_float_eps(DE::Vector3(a).clamp(0.2f).len(), 0.2f, 0.00001f);
	test_expected_float_eps(DE::Vector3(a).clamp(0.3f).len(), 0.3f, 0.00001f);
	test_expected_float_eps(DE::Vector3(a).clamp(0.4f).len(), 0.4f, 0.00001f);
	test_expected_float_eps(DE::Vector3(a).clamp(0.5f).len(), 0.5f, 0.00001f);

	summary();

	return 0;
}
