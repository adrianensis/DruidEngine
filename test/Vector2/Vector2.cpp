#include "Druid.h"
#include <cmath>

int main() {

	test(DE::Vector2);


	DE::Vector2 a(0,1);
	DE::Vector2 b(4,5);
	DE::Vector2 c(1,2);
	DE::Vector2 d;

	show(a);
	show(b);
	show(c);
	show(d);

	title("OPERATORS");

	expected(a + a, DE::Vector2(0+0,1+1));
	expected(a - a, DE::Vector2(0-0,1-1));
	expected(b * a, DE::Vector2(4*0,5*1));
	expected(b / DE::Vector2(2,2), DE::Vector2(4/2.0f,5/2.0f));
	expected(DE::Vector2(a) -= a, DE::Vector2(0-0,1-1));
	expected(DE::Vector2(a) += a, DE::Vector2(0+0,1+1));
	expected(DE::Vector2(a) *= a, DE::Vector2(0*0,1*1));
	expected(DE::Vector2(a) /= DE::Vector2(2,2), DE::Vector2(0/2.0f,1/2.0f));
	expected_bool(a == a, true);
	expected_bool(a == b, false);
	expected_bool(a != a, false);
	expected_bool(a != b, true);

	title("ARRAY ACCESS");

	expected_num(a[0],0);
	expected_num(a[1],1);

	title("FUNCTIONS");

	expected(DE::Vector2(a).add(a), DE::Vector2(0+0,1+1));
	expected(DE::Vector2(a).sub(a), DE::Vector2(0-0,1-1));
	expected(DE::Vector2(a).mul(DE::Vector2(2,2)), DE::Vector2(0*2,1*2));
	expected(DE::Vector2(a).div(DE::Vector2(2,2)), DE::Vector2(0/2.0f,1/2.0f));
	expected(DE::Vector2(a).add(1), DE::Vector2(0+1,1+1));
	expected(DE::Vector2(a).sub(1), DE::Vector2(0-1,1-1));
	expected(DE::Vector2(a).mul(2), DE::Vector2(0*2,1*2));
	expected(DE::Vector2(a).div(2), DE::Vector2(0/2.0f,1/2.0f));
	// expected(DE::Vector2(a).div(0), DE::Vector2(0/2.0f,1/2.0f));

	show(c.div(2));
	expected(c, DE::Vector2(1/2.0f,2/2.0f));

	expected(DE::Vector2(b).lerp(a, 0.5f),DE::Vector2(b) += (a-b)*0.5f);
	expected(DE::Vector2(b).lerp(a, 0.1f),DE::Vector2(b) += (a-b)*0.1f);
	expected(DE::Vector2(b).lerp(a, 0.823f),DE::Vector2(b) += (a-b)*0.823f);
	expected_num(a.dot(a), a.x*a.x + a.y*a.y);
	expected_num(a.dot(b), a.x*b.x + a.y*b.y);
	expected_num(a.sqrlen(), a.dot(a));
	expected_num(a.len(), sqrtf(a.sqrlen()));
	expected_num(a.max(),1);
	expected_num(a.min(),0);
	expected_num(a.sqrdst(b),(a-b).dot(a-b));
	expected_num(a.dst(b), sqrtf(a.sqrdst(b)));
	expected_bool(a.eq(b,0),false);
	expected_bool(a.eq(b,10),true);
	expected(DE::Vector2(a).nor(), DE::Vector2(a).div(a.len()));

	expected_num(a.angle(a), 0.0);
	expected_num(DE::Vector2(1,0).angle(DE::Vector2(0,1))*(180/M_PI), 90);
	expected_num(DE::Vector2(1,0).angle(DE::Vector2(1,1))*(180/M_PI), 45);
	expected_num(DE::Vector2(1,0).angle(DE::Vector2(1,-1))*(180/M_PI), 315);
	expected_num(DE::Vector2(1,0).angle(DE::Vector2(-1,0))*(180/M_PI), 180);

	expected_num_eps(DE::Vector2(a).clamp(0.1f).len(),0.1f,0.1f);
	expected_num_eps(DE::Vector2(a).clamp(0.2f).len(),0.2f,0.1f);
	expected_num_eps(DE::Vector2(a).clamp(0.3f).len(),0.3f,0.1f);
	expected_num_eps(DE::Vector2(a).clamp(0.4f).len(),0.4f,0.1f);
	expected_num_eps(DE::Vector2(a).clamp(0.5f).len(),0.5f,0.1f);

	summary();

	return 0;
}
