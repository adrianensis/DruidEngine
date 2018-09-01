#include "Druid.h"
#include <cmath>

DE::Quaternion mul(DE::Quaternion q1, DE::Quaternion q2) {
	float w = (q1.w * q2.w) - (q1.v.dot(q2.v));
	DE::Vector3 v(q2.v*q1.w + q1.v*q1.w + DE::Vector3(q1.v).cross(q2.v));
	return DE::Quaternion(v,w);
}

int main() {

	test(DE::Quaternion);

	DE::Quaternion a(0,1,2,3);
	DE::Quaternion b(4,5,6,7);
	DE::Quaternion c(1,2,2,3);
	DE::Quaternion d(DE::Vector3(30,10,90));
	DE::Quaternion e(DE::Vector3(0,0,0));
	DE::Quaternion f(DE::Vector3(180,10,10));
	DE::Quaternion g(f);
	DE::Quaternion h;

	show(a);
	show(b);
	show(c);
	show(d);
	show(e);
	show(f);
	show(g);
	show(h);

	title("OPERATORS");

	expected(a + a, DE::Quaternion(0+0,1+1,2+2,3+3));
	expected(a - a, DE::Quaternion(0-0,1-1,2-2,3-3));
	expected(b * a, mul(b,a));
	expected(b / a, mul(b,DE::Quaternion(a).inv()));
	expected(DE::Quaternion(a) -= a, DE::Quaternion(0-0,1-1,2-2,3-3));
	expected(DE::Quaternion(a) += a, DE::Quaternion(0+0,1+1,2+2,3+3));
	expected(DE::Quaternion(a) *= a, mul(a,a));
	expected(DE::Quaternion(a) /= a, mul(a,DE::Quaternion(a).inv()));
	expected_bool(a == a, true);
	expected_bool(a == b, false);
	expected_bool(a != a, false);
	expected_bool(a != b, true);

	title("ARRAY ACCESS");

	expected_float(a[0],0);
	expected_float(a[1],1);
	expected_float(a[2],2);
	expected_float(a[3],3);

	title("METHODS");

	expected_float(a.dot(a), a.v.x*a.v.x + a.v.y*a.v.y + a.v.z*a.v.z + a.w*a.w);
	expected_float(a.dot(b), a.v.x*b.v.x + a.v.y*b.v.y + a.v.z*b.v.z + a.w*b.w);
	expected_float(a.sqrlen(), a.dot(a));
	expected_float(a.len(), sqrtf(a.sqrlen()));
	expected_bool(a.eq(b,0),false);
	expected_bool(a.eq(b,10),true);
	expected(DE::Quaternion(a).nor(), DE::Quaternion(a).div(a.len()));
	expected_float_eps(DE::Quaternion(a).nor().len(), 1.0f, 0.00001f);

	expected_bool(e.toEuler().eq(DE::Vector3(0,0,0),0.000001f),true);
	show(f.toEuler());
	expected_bool(f.toEuler().eq(DE::Vector3(-180,10,10),0.1f),true);

	summary();

	return 0;
}
