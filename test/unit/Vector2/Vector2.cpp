#include "Test/Test.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/MathUtils.hpp"
#include <cmath>

int main()
{
	test(Vector2);

	Vector2 a(0, 1);
	Vector2 b(4, 5);
	Vector2 c(1, 2);
	Vector2 d;

	test_show(a);
	test_show(b);
	test_show(c);
	test_show(d);

	test_title("OPERATORS");

	test_expected(a + a, Vector2(0 + 0, 1 + 1));
	test_expected(a - a, Vector2(0 - 0, 1 - 1));
	test_expected(b * a, Vector2(4 * 0, 5 * 1));
	test_expected(b / Vector2(2, 2), Vector2(4 / 2.0f, 5 / 2.0f));
	test_expected(Vector2(a) -= a, Vector2(0 - 0, 1 - 1));
	test_expected(Vector2(a) += a, Vector2(0 + 0, 1 + 1));
	test_expected(Vector2(a) *= a, Vector2(0 * 0, 1 * 1));
	test_expected(Vector2(a) /= Vector2(2, 2), Vector2(0 / 2.0f, 1 / 2.0f));
	test_expected_bool(a == a, true);
	test_expected_bool(a == b, false);
	test_expected_bool(a != a, false);
	test_expected_bool(a != b, true);

	test_title("ARRAY ACCESS");

	test_expected_float(a[0], 0);
	test_expected_float(a[1], 1);

	test_title("FUNCTIONS");

	test_expected(Vector2(a).add(a), Vector2(0 + 0, 1 + 1));
	test_expected(Vector2(a).sub(a), Vector2(0 - 0, 1 - 1));
	test_expected(Vector2(a).mul(Vector2(2, 2)), Vector2(0 * 2, 1 * 2));
	test_expected(Vector2(a).div(Vector2(2, 2)), Vector2(0 / 2.0f, 1 / 2.0f));
	test_expected(Vector2(a).add(1), Vector2(0 + 1, 1 + 1));
	test_expected(Vector2(a).sub(1), Vector2(0 - 1, 1 - 1));
	test_expected(Vector2(a).mul(2), Vector2(0 * 2, 1 * 2));
	test_expected(Vector2(a).div(2), Vector2(0 / 2.0f, 1 / 2.0f));
	// test_expected(Vector2(a).div(0), Vector2(0/2.0f,1/2.0f));

	test_show(c.div(2));
	test_expected(c, Vector2(1 / 2.0f, 2 / 2.0f));

	test_expected(Vector2(b).lerp(a, 0.5f), Vector2(b) += (a - b) * 0.5f);
	test_expected(Vector2(b).lerp(a, 0.1f), Vector2(b) += (a - b) * 0.1f);
	test_expected(Vector2(b).lerp(a, 0.823f), Vector2(b) += (a - b) * 0.823f);
	test_expected_float(a.dot(a), a.x * a.x + a.y * a.y);
	test_expected_float(a.dot(b), a.x * b.x + a.y * b.y);
	test_expected_float(a.sqrlen(), a.dot(a));
	test_expected_float(a.len(), sqrtf(a.sqrlen()));
	test_expected_float(a.max(), 1);
	test_expected_float(a.min(), 0);
	test_expected_float(a.sqrdst(b), (a - b).dot(a - b));
	test_expected_float(a.dst(b), sqrtf(a.sqrdst(b)));
	test_expected_bool(a.eq(b, 0), false);
	test_expected_bool(a.eq(b, 10), true);
	test_expected(Vector2(a).nor(), Vector2(a).div(a.len()));

	test_expected_float(a.angle(a), 0.0);
	test_expected_float(Vector2(1, 0).angle(Vector2(0, 1)) * (180 / MathUtils::PI), 90);
	test_expected_float(Vector2(1, 0).angle(Vector2(1, 1)) * (180 / MathUtils::PI), 45);
	test_expected_float(Vector2(1, 0).angle(Vector2(1, -1)) * (180 / MathUtils::PI), 315);
	test_expected_float(Vector2(1, 0).angle(Vector2(-1, 0)) * (180 / MathUtils::PI), 180);

	test_expected_float_eps(Vector2(a).clamp(0.1f).len(), 0.1f, 0.1f);
	test_expected_float_eps(Vector2(a).clamp(0.2f).len(), 0.2f, 0.1f);
	test_expected_float_eps(Vector2(a).clamp(0.3f).len(), 0.3f, 0.1f);
	test_expected_float_eps(Vector2(a).clamp(0.4f).len(), 0.4f, 0.1f);
	test_expected_float_eps(Vector2(a).clamp(0.5f).len(), 0.5f, 0.1f);

	summary();

	return 0;
}
