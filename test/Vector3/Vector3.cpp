#include "Druid.h"
#include <cmath>

int main() {

	test(DE::Vector3);

	DE::Vector3 a(0,1,2);
	DE::Vector3 b(4,5,6);
	DE::Vector3 c(1,2,2);
	DE::Vector3 d;

	show(a);
	show(b);
	show(c);
	show(d);

	title("OPERATORS");

	expected(a + a, DE::Vector3(0+0,1+1,2+2));
	expected(a - a, DE::Vector3(0-0,1-1,2-2));
	expected(b * a, DE::Vector3(4*0,5*1,6*2));
	expected(b / DE::Vector3(1,1,2), DE::Vector3(4/1.0f,5/1.0f,6/2.0f));
	expected(DE::Vector3(a) -= a, DE::Vector3(0-0,1-1,2-2));
	expected(DE::Vector3(a) += a, DE::Vector3(0+0,1+1,2+2));
	expected(DE::Vector3(a) *= a, DE::Vector3(0*0,1*1,2*2));
	expected(DE::Vector3(a) /= DE::Vector3(1,1,2), DE::Vector3(0/1.0f,1/1.0f,2/2.0f));
	expected_bool(a == a, true);
	expected_bool(a == b, false);
	expected_bool(a != a, false);
	expected_bool(a != b, true);

	title("ARRAY ACCESS");

	expected_num(a[0],0);
	expected_num(a[1],1);
	expected_num(a[2],2);

	title("FUNCTIONS");

	expected(DE::Vector3(a).add(a), DE::Vector3(0+0,1+1,2+2));
	expected(DE::Vector3(a).sub(a), DE::Vector3(0-0,1-1,2-2));
	expected(DE::Vector3(a).mul(DE::Vector3(2,2,2)), DE::Vector3(0*2,1*2,2*2));
	expected(DE::Vector3(a).div(DE::Vector3(2,2,2)), DE::Vector3(0/2.0f,1/2.0f,2/2.0f));
	expected(DE::Vector3(a).add(1), DE::Vector3(0+1,1+1,2+1));
	expected(DE::Vector3(a).sub(1), DE::Vector3(0-1,1-1,2-1));
	expected(DE::Vector3(a).mul(2), DE::Vector3(0*2,1*2,2*2));
	expected(DE::Vector3(a).div(2), DE::Vector3(0/2.0f,1/2.0f,2/2.0f));

	show(c.div(2));
	expected(c, DE::Vector3(1/2.0f,2/2.0f,2/2.0f));

	expected(DE::Vector3(b).lerp(a, 0.5f),DE::Vector3(b) += (a-b)*0.5f);
	expected(DE::Vector3(b).lerp(a, 0.1f),DE::Vector3(b) += (a-b)*0.1f);
	expected(DE::Vector3(b).lerp(a, 0.823f),DE::Vector3(b) += (a-b)*0.823f);
	expected_num(a.dot(a), a.x*a.x + a.y*a.y + a.z*a.z);
	expected_num(a.dot(b), a.x*b.x + a.y*b.y + a.z*b.z);
	expected_num(a.sqrlen(), a.dot(a));
	expected_num(a.len(), sqrtf(a.sqrlen()));
	expected_num(a.max(),2);
	expected_num(a.min(),0);
	expected_num(a.sqrdst(b),(a-b).dot(a-b));
	expected_num(a.dst(b), sqrtf(a.sqrdst(b)));
	expected_bool(a.eq(b,0),false);
	expected_bool(a.eq(b,10),true);
	expected(DE::Vector3(a).nor(), DE::Vector3(a).div(a.len()));
	expected(DE::Vector3(1,0,0).cross(DE::Vector3(0,1,0)), DE::Vector3(0,0,1));
	expected(DE::Vector3(1,0,0).cross(DE::Vector3(0,-1,0)), DE::Vector3(0,0,-1));
	expected(DE::Vector3(-1,0,0).cross(DE::Vector3(0,1,0)), DE::Vector3(0,0,-1));

	expected_num(a.angle(a), 0.0);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(0,1,0))*(180/M_PI), 90);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(0,-1,0))*(180/M_PI), 90);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(1,1,0))*(180/M_PI), 45);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(1,-1,0))*(180/M_PI), 45);
	expected_num(DE::Vector3(-1,0,0).angle(DE::Vector3(-1,-1,0))*(180/M_PI), 45);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(-1,0,0))*(180/M_PI), 180);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(-1,0,1))*(180/M_PI), 135);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(-1,0,-1))*(180/M_PI), 135);

	expected_num(a.angle(a, DE::Vector3(0,1,0)), 0.0);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(0,1,0), DE::Vector3(0,0,1))*(180/M_PI), 90);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(0,-1,0), DE::Vector3(0,0,1))*(180/M_PI), 270);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(1,1,0), DE::Vector3(0,0,1))*(180/M_PI), 45);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(1,-1,0), DE::Vector3(0,0,1))*(180/M_PI), 315);
	expected_num(DE::Vector3(-1,0,0).angle(DE::Vector3(-1,-1,0), DE::Vector3(0,0,1))*(180/M_PI), 45);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(-1,0,0), DE::Vector3(0,1,0))*(180/M_PI), 180);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(-1,0,1), DE::Vector3(0,1,0))*(180/M_PI), 225);
	expected_num(DE::Vector3(1,0,0).angle(DE::Vector3(-1,0,-1), DE::Vector3(0,1,0))*(180/M_PI), 135);

	expected_num_eps(DE::Vector3(a).clamp(0.1f).len(),0.1f,0.00001f);
	expected_num_eps(DE::Vector3(a).clamp(0.2f).len(),0.2f,0.00001f);
	expected_num_eps(DE::Vector3(a).clamp(0.3f).len(),0.3f,0.00001f);
	expected_num_eps(DE::Vector3(a).clamp(0.4f).len(),0.4f,0.00001f);
	expected_num_eps(DE::Vector3(a).clamp(0.5f).len(),0.5f,0.00001f);

	summary();

	return 0;
}
