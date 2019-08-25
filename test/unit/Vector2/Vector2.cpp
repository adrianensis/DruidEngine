#include "Test.h"
#include "Memory.h"
#include "Vector2.h"
#include <cmath>

int main() {

	DE_test(DE::Vector2);


	DE::Vector2 a(0,1);
	DE::Vector2 b(4,5);
	DE::Vector2 c(1,2);
	DE::Vector2 d;

	DE_test_show(a);
	DE_test_show(b);
	DE_test_show(c);
	DE_test_show(d);

	DE_test_title("OPERATORS");

	DE_test_expected(a + a, DE::Vector2(0+0,1+1));
	DE_test_expected(a - a, DE::Vector2(0-0,1-1));
	DE_test_expected(b * a, DE::Vector2(4*0,5*1));
	DE_test_expected(b / DE::Vector2(2,2), DE::Vector2(4/2.0f,5/2.0f));
	DE_test_expected(DE::Vector2(a) -= a, DE::Vector2(0-0,1-1));
	DE_test_expected(DE::Vector2(a) += a, DE::Vector2(0+0,1+1));
	DE_test_expected(DE::Vector2(a) *= a, DE::Vector2(0*0,1*1));
	DE_test_expected(DE::Vector2(a) /= DE::Vector2(2,2), DE::Vector2(0/2.0f,1/2.0f));
	DE_test_expected_bool(a == a, true);
	DE_test_expected_bool(a == b, false);
	DE_test_expected_bool(a != a, false);
	DE_test_expected_bool(a != b, true);

	DE_test_title("ARRAY ACCESS");

	DE_test_expected_float(a[0],0);
	DE_test_expected_float(a[1],1);

	DE_test_title("FUNCTIONS");

	DE_test_expected(DE::Vector2(a).add(a), DE::Vector2(0+0,1+1));
	DE_test_expected(DE::Vector2(a).sub(a), DE::Vector2(0-0,1-1));
	DE_test_expected(DE::Vector2(a).mul(DE::Vector2(2,2)), DE::Vector2(0*2,1*2));
	DE_test_expected(DE::Vector2(a).div(DE::Vector2(2,2)), DE::Vector2(0/2.0f,1/2.0f));
	DE_test_expected(DE::Vector2(a).add(1), DE::Vector2(0+1,1+1));
	DE_test_expected(DE::Vector2(a).sub(1), DE::Vector2(0-1,1-1));
	DE_test_expected(DE::Vector2(a).mul(2), DE::Vector2(0*2,1*2));
	DE_test_expected(DE::Vector2(a).div(2), DE::Vector2(0/2.0f,1/2.0f));
	// DE_test_expected(DE::Vector2(a).div(0), DE::Vector2(0/2.0f,1/2.0f));

	DE_test_show(c.div(2));
	DE_test_expected(c, DE::Vector2(1/2.0f,2/2.0f));

	DE_test_expected(DE::Vector2(b).lerp(a, 0.5f),DE::Vector2(b) += (a-b)*0.5f);
	DE_test_expected(DE::Vector2(b).lerp(a, 0.1f),DE::Vector2(b) += (a-b)*0.1f);
	DE_test_expected(DE::Vector2(b).lerp(a, 0.823f),DE::Vector2(b) += (a-b)*0.823f);
	DE_test_expected_float(a.dot(a), a.x*a.x + a.y*a.y);
	DE_test_expected_float(a.dot(b), a.x*b.x + a.y*b.y);
	DE_test_expected_float(a.sqrlen(), a.dot(a));
	DE_test_expected_float(a.len(), sqrtf(a.sqrlen()));
	DE_test_expected_float(a.max(),1);
	DE_test_expected_float(a.min(),0);
	DE_test_expected_float(a.sqrdst(b),(a-b).dot(a-b));
	DE_test_expected_float(a.dst(b), sqrtf(a.sqrdst(b)));
	DE_test_expected_bool(a.eq(b,0),false);
	DE_test_expected_bool(a.eq(b,10),true);
	DE_test_expected(DE::Vector2(a).nor(), DE::Vector2(a).div(a.len()));

	DE_test_expected_float(a.angle(a), 0.0);
	DE_test_expected_float(DE::Vector2(1,0).angle(DE::Vector2(0,1))*(180/M_PI), 90);
	DE_test_expected_float(DE::Vector2(1,0).angle(DE::Vector2(1,1))*(180/M_PI), 45);
	DE_test_expected_float(DE::Vector2(1,0).angle(DE::Vector2(1,-1))*(180/M_PI), 315);
	DE_test_expected_float(DE::Vector2(1,0).angle(DE::Vector2(-1,0))*(180/M_PI), 180);

	DE_test_expected_float_eps(DE::Vector2(a).clamp(0.1f).len(),0.1f,0.1f);
	DE_test_expected_float_eps(DE::Vector2(a).clamp(0.2f).len(),0.2f,0.1f);
	DE_test_expected_float_eps(DE::Vector2(a).clamp(0.3f).len(),0.3f,0.1f);
	DE_test_expected_float_eps(DE::Vector2(a).clamp(0.4f).len(),0.4f,0.1f);
	DE_test_expected_float_eps(DE::Vector2(a).clamp(0.5f).len(),0.5f,0.1f);

	summary();

	return 0;
}
