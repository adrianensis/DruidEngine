#include "Test/Test.hpp"
#include "Memory/Memory.hpp"
#include "Maths/Quaternion.hpp"
#include <cmath>

DE::Quaternion mul(DE::Quaternion q1, DE::Quaternion q2) {
	float w = (q1.w * q2.w) - (q1.v.dot(q2.v));
	DE::Vector3 v(q2.v * q1.w + q1.v * q1.w + DE::Vector3(q1.v).cross(q2.v));
	return DE::Quaternion(v, w);
}

int main() {

	DE::Memory::init();

	test(DE::Quaternion);

	DE::Quaternion a(0, 1, 2, 3);
	DE::Quaternion b(4, 5, 6, 7);
	DE::Quaternion c(1, 2, 2, 3);
	DE::Quaternion d(DE::Vector3(30, 10, 90));
	DE::Quaternion e(DE::Vector3(0, 0, 0));
	DE::Quaternion f(DE::Vector3(180, 10, 10));
	DE::Quaternion g(f);
	DE::Quaternion h;

	test_show(a);
	test_show(b);
	test_show(c);
	test_show(d);
	test_show(e);
	test_show(f);
	test_show(g);
	test_show(h);

	test_title("OPERATORS");

	test_expected(a + a, DE::Quaternion(0 + 0, 1 + 1, 2 + 2, 3 + 3));
	test_expected(a - a, DE::Quaternion(0 - 0, 1 - 1, 2 - 2, 3 - 3));
	test_expected(b * a, mul(b, a));
	test_expected(b / a, mul(b, DE::Quaternion(a).inv()));
	test_expected(DE::Quaternion(a) -= a, DE::Quaternion(0 - 0, 1 - 1, 2 - 2, 3 - 3));
	test_expected(DE::Quaternion(a) += a, DE::Quaternion(0 + 0, 1 + 1, 2 + 2, 3 + 3));
	test_expected(DE::Quaternion(a) *= a, mul(a, a));
	test_expected(DE::Quaternion(a) /= a, mul(a, DE::Quaternion(a).inv()));
	test_expected_bool(a == a, true);
	test_expected_bool(a == b, false);
	test_expected_bool(a != a, false);
	test_expected_bool(a != b, true);

	test_title("ARRAY ACCESS");

	test_expected_float(a[0], 0);
	test_expected_float(a[1], 1);
	test_expected_float(a[2], 2);
	test_expected_float(a[3], 3);

	test_title("METHODS");

	test_expected_float(a.dot(a), a.v.x * a.v.x + a.v.y * a.v.y + a.v.z * a.v.z + a.w * a.w);
	test_expected_float(a.dot(b), a.v.x * b.v.x + a.v.y * b.v.y + a.v.z * b.v.z + a.w * b.w);
	test_expected_float(a.sqrlen(), a.dot(a));
	test_expected_float(a.len(), sqrtf(a.sqrlen()));
	test_expected_bool(a.eq(b, 0), false);
	test_expected_bool(a.eq(b, 10), true);
	test_expected(DE::Quaternion(a).nor(), DE::Quaternion(a).div(a.len()));
	test_expected_float_eps(DE::Quaternion(a).nor().len(), 1.0f, 0.00001f);

	/*test_expected_bool(e.toEuler().eq(DE::Vector3(0, 0, 0), 0.000001f), true);
	test_show(f.toEuler());
	test_expected_bool(f.toEuler().eq(DE::Vector3(-180, 10, 10), 0.1f), true);*/

	DE::Matrix4* m = DE::Memory::allocate<Matrix4>();
	m->zeros();

	DE::Quaternion i(0, 0, 0, 1);
	i.toMatrix(m);

	test_expected_float_eps(m->get(0, 0), 1.0f, 0.00001f);
	test_expected_float_eps(m->get(0, 1), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(0, 2), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(0, 3), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(1, 0), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(1, 1), 1.0f, 0.00001f);
	test_expected_float_eps(m->get(1, 2), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(1, 3), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(2, 0), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(2, 1), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(2, 2), 1.0f, 0.00001f);
	test_expected_float_eps(m->get(2, 3), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(3, 1), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(3, 2), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(3, 0), 0.0f, 0.00001f);
	test_expected_float_eps(m->get(3, 3), 1.0f, 0.00001f);

	DE::Quaternion ii(0, 0, 0, 1);

	/*DE::Matrix4* mRot = DE::Memory::allocate<Matrix4>();
	mRot->rotation(DE::Vector3(0, 0, 0));

	ii.fromMatrix(*mRot);

	test_expected_float_eps(ii[0], 0, 0.00001f);
	test_expected_float_eps(ii[1], 0, 0.00001f);
	test_expected_float_eps(ii[2], 0, 0.00001f);
	test_expected_float_eps(ii[3], 1, 0.00001f);

	mRot->rotation(DE::Vector3(92.0f, 0, 0));

	ii.fromMatrix(*mRot);*/

	/*test_show(ii.toEuler().x);
	test_show(ii.toEuler().y);
	test_show(ii.toEuler().z);*/

	//test_expected_bool(ii.toEuler().eq(DE::Vector3(92.0f, 0, 0), 0.001f), true);

	/*test_show(ii.v.x);

	test_expected_float_eps(ii.v.x, -0.719f, 0.001f);
	test_expected_float_eps(ii.v.y, 0, 0.00001f);
	test_expected_float_eps(ii.v.z, 0, 0.00001f);
	test_expected_float_eps(ii.w, 0.695f, 0.00001f);*/

	//DE::Quaternion z(Vector3(60, 60, 10));

	/*test_expected_float_eps(z.v.x, 0.469f, 0.00001f);
	test_expected_float_eps(z.v.y, 0.394f, 0.00001f);
	test_expected_float_eps(z.v.z, 0.314f, 0.00001f);
	test_expected_float_eps(z.w, 0.725f, 0.00001f);*/

	/*Vector3 euler(z.toEuler());

	test_expected_float_eps(euler.x, 60.0f, 0.00001f);
	test_expected_float_eps(euler.y, 60.0f, 0.00001f);
	test_expected_float_eps(euler.z, 10.0f, 0.00001f);*/

	DE::Memory::free();

	summary();

	return 0;
}
