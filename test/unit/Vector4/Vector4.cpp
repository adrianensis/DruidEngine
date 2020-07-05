#include "Test.hpp"
#include "Memory.hpp"
#include "Vector4.hpp"
#include <cmath>

int main() {

	DE_test(DE::Vector4);

	DE::Vector4 a(0, 1, 2, 3);
	DE::Vector4 b(4, 5, 6, 7);
	DE::Vector4 c(1, 2, 2, 3);
	DE::Vector4 d;

	DE_test_show(a);
	DE_test_show(b);
	DE_test_show(c);
	DE_test_show(d);

	DE_test_title("OPERATORS");

	DE_test_expected(a + a, DE::Vector4(0 + 0, 1 + 1, 2 + 2, 3 + 3));
	DE_test_expected(a - a, DE::Vector4(0 - 0, 1 - 1, 2 - 2, 3 - 3));
	DE_test_expected(b * a, DE::Vector4(4 * 0, 5 * 1, 6 * 2, 7 * 3));
	DE_test_expected(b / DE::Vector4(1, 1, 2, 3), DE::Vector4(4 / 1.0f, 5 / 1.0f, 6 / 2.0f, 7 / 3.0f));
	DE_test_expected(DE::Vector4(a) -= a, DE::Vector4(0 - 0, 1 - 1, 2 - 2, 3 - 3));
	DE_test_expected(DE::Vector4(a) += a, DE::Vector4(0 + 0, 1 + 1, 2 + 2, 3 + 3));
	DE_test_expected(DE::Vector4(a) *= a, DE::Vector4(0 * 0, 1 * 1, 2 * 2, 3 * 3));
	DE_test_expected(DE::Vector4(a) /= DE::Vector4(1, 1, 2, 3), DE::Vector4(0 / 1.0f, 1 / 1.0f, 2 / 2.0f, 3 / 3.0f));
	DE_test_expected_bool(a == a, true);
	DE_test_expected_bool(a == b, false);
	DE_test_expected_bool(a != a, false);
	DE_test_expected_bool(a != b, true);

	DE_test_title("ARRAY ACCESS");

	DE_test_expected_float(a[0], 0);
	DE_test_expected_float(a[1], 1);
	DE_test_expected_float(a[2], 2);
	DE_test_expected_float(a[3], 3);

	DE_test_title("FUNCTIONS");

	DE_test_expected(DE::Vector4(a).add(a), DE::Vector4(0 + 0, 1 + 1, 2 + 2, 3 + 3));
	DE_test_expected(DE::Vector4(a).sub(a), DE::Vector4(0 - 0, 1 - 1, 2 - 2, 3 - 3));
	DE_test_expected(DE::Vector4(a).mul(DE::Vector4(2, 2, 2, 2)), DE::Vector4(0 * 2, 1 * 2, 2 * 2, 3 * 2));
	DE_test_expected(DE::Vector4(a).div(DE::Vector4(2, 2, 2, 2)), DE::Vector4(0 / 2.0f, 1 / 2.0f, 2 / 2.0f, 3 / 2.0f));
	DE_test_expected(DE::Vector4(a).add(1), DE::Vector4(0 + 1, 1 + 1, 2 + 1, 3 + 1));
	DE_test_expected(DE::Vector4(a).sub(1), DE::Vector4(0 - 1, 1 - 1, 2 - 1, 3 - 1));
	DE_test_expected(DE::Vector4(a).mul(2), DE::Vector4(0 * 2, 1 * 2, 2 * 2, 3 * 2));
	DE_test_expected(DE::Vector4(a).div(2), DE::Vector4(0 / 2.0f, 1 / 2.0f, 2 / 2.0f, 3 / 2.0f));

	DE_test_show(c.div(2));
	DE_test_expected(c, DE::Vector4(1 / 2.0f, 2 / 2.0f, 2 / 2.0f, 3 / 2.0f));

	DE_test_expected(DE::Vector4(b).lerp(a, 0.5f), DE::Vector4(b) += (a - b) * 0.5f);
	DE_test_expected(DE::Vector4(b).lerp(a, 0.1f), DE::Vector4(b) += (a - b) * 0.1f);
	DE_test_expected(DE::Vector4(b).lerp(a, 0.823f), DE::Vector4(b) += (a - b) * 0.823f);
	DE_test_expected_float(a.dot(a), a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	DE_test_expected_float(a.dot(b), a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
	DE_test_expected_float(a.sqrlen(), a.dot(a));
	DE_test_expected_float(a.len(), sqrtf(a.sqrlen()));
	DE_test_expected_float(a.max(), 3);
	DE_test_expected_float(a.min(), 0);
	DE_test_expected_float(a.sqrdst(b), (a - b).dot(a - b));
	DE_test_expected_float(a.dst(b), sqrtf(a.sqrdst(b)));
	DE_test_expected_bool(a.eq(b, 0), false);
	DE_test_expected_bool(a.eq(b, 10), true);
	DE_test_expected(DE::Vector4(a).nor(), DE::Vector4(a).div(a.len()));

	DE_test_expected_float_eps(DE::Vector4(a).clamp(0.1f).len(), 0.1f, 0.1f);
	DE_test_expected_float_eps(DE::Vector4(a).clamp(0.2f).len(), 0.2f, 0.1f);
	DE_test_expected_float_eps(DE::Vector4(a).clamp(0.3f).len(), 0.3f, 0.1f);
	DE_test_expected_float_eps(DE::Vector4(a).clamp(0.4f).len(), 0.4f, 0.1f);
	DE_test_expected_float_eps(DE::Vector4(a).clamp(0.5f).len(), 0.5f, 0.1f);

	summary();

	return 0;
}
