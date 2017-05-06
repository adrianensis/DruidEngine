#include "Druid.h"
#include <cmath>

int main() {

	test(DE::Vector4);

	DE::Vector4 a(0,1,2,3);
	DE::Vector4 b(4,5,6,7);
	DE::Vector4 c(1,2,2,3);
	DE::Vector4 d;



	show(a);
	show(b);
	show(c);
	show(d);

	title("OPERATORS");

	expected(a + a, DE::Vector4(0+0,1+1,2+2,3+3));
	expected(a - a, DE::Vector4(0-0,1-1,2-2,3-3));
	expected(b * a, DE::Vector4(4*0,5*1,6*2,7*3));
	expected(b / DE::Vector4(1,1,2,3), DE::Vector4(4/1.0f,5/1.0f,6/2.0f,7/3.0f));
	expected(DE::Vector4(a) -= a, DE::Vector4(0-0,1-1,2-2,3-3));
	expected(DE::Vector4(a) += a, DE::Vector4(0+0,1+1,2+2,3+3));
	expected(DE::Vector4(a) *= a, DE::Vector4(0*0,1*1,2*2,3*3));
	expected(DE::Vector4(a) /= DE::Vector4(1,1,2,3), DE::Vector4(0/1.0f,1/1.0f,2/2.0f,3/3.0f));
	expected_bool(a == a, true);
	expected_bool(a == b, false);
	expected_bool(a != a, false);
	expected_bool(a != b, true);

	title("ARRAY ACCESS");

	expected_num(a[0],0);
	expected_num(a[1],1);
	expected_num(a[2],2);
	expected_num(a[3],3);

	title("FUNCTIONS");

	expected(DE::Vector4(a).add(a), DE::Vector4(0+0,1+1,2+2,3+3));
	expected(DE::Vector4(a).sub(a), DE::Vector4(0-0,1-1,2-2,3-3));
	expected(DE::Vector4(a).mul(DE::Vector4(2,2,2,2)), DE::Vector4(0*2,1*2,2*2,3*2));
	expected(DE::Vector4(a).div(DE::Vector4(2,2,2,2)), DE::Vector4(0/2.0f,1/2.0f,2/2.0f,3/2.0f));
	expected(DE::Vector4(a).add(1), DE::Vector4(0+1,1+1,2+1,3+1));
	expected(DE::Vector4(a).sub(1), DE::Vector4(0-1,1-1,2-1,3-1));
	expected(DE::Vector4(a).mul(2), DE::Vector4(0*2,1*2,2*2,3*2));
	expected(DE::Vector4(a).div(2), DE::Vector4(0/2.0f,1/2.0f,2/2.0f,3/2.0f));

	show(c.div(2));
	expected(c, DE::Vector4(1/2.0f,2/2.0f,2/2.0f,3/2.0f));

	expected(DE::Vector4(b).lerp(a, 0.5f),DE::Vector4(b) += (a-b)*0.5f);
	expected(DE::Vector4(b).lerp(a, 0.1f),DE::Vector4(b) += (a-b)*0.1f);
	expected(DE::Vector4(b).lerp(a, 0.823f),DE::Vector4(b) += (a-b)*0.823f);
	expected_num(a.dot(a), a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w);
	expected_num(a.dot(b), a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w);
	expected_num(a.sqrlen(), a.dot(a));
	expected_num(a.len(), sqrtf(a.sqrlen()));
	expected_num(a.max(),3);
	expected_num(a.min(),0);
	expected_num(a.sqrdst(b),(a-b).dot(a-b));
	expected_num(a.dst(b), sqrtf(a.sqrdst(b)));
	expected_bool(a.eq(b,0),false);
	expected_bool(a.eq(b,10),true);
	expected(DE::Vector4(a).nor(), DE::Vector4(a).div(a.len()));

	expected_num_eps(DE::Vector4(a).clamp(0.1f).len(),0.1f,0.1f);
	expected_num_eps(DE::Vector4(a).clamp(0.2f).len(),0.2f,0.1f);
	expected_num_eps(DE::Vector4(a).clamp(0.3f).len(),0.3f,0.1f);
	expected_num_eps(DE::Vector4(a).clamp(0.4f).len(),0.4f,0.1f);
	expected_num_eps(DE::Vector4(a).clamp(0.5f).len(),0.5f,0.1f);

	summary();

	return 0;
}
