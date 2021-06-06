//---------------------------------------------------------------------------

#ifndef colisionH
#define colisionH
#include <utility>
#include "vcl.h"
#include "object.h"
#include <math.h>
#include <iostream>
using namespace std;
//---------------------------------------------------------------------------
float DotProduct(float a1, float a2, float b1, float b2)
{
	return (a1 * b1 + a2 * b2);
}

bool RecvsRec(Rectangl& a, Rectangl& b)
{
	if(a.max.first < b.min.first || a.min.first > b.max.first) return false;
	if(a.max.second < b.min.second || a.min.second > b.max.second) return false;
	return true;
}


bool RecvsCirk(Rectangl& A, Circle& B)
{
	float dis;
	float dig;
	pair <float , float> Acenter;
	Acenter.first = (A.max.first - A.min.first) / 2 + A.min.first;
	Acenter.second = (A.max.second - A.min.second) / 2 + A.min.second;
	dis = sqrt((float)(B.position.first - Acenter.first) * (B.position.first - Acenter.first)
	 + (B.position.second - Acenter.second) * (B.position.second - Acenter.second));
	dig = sqrt((float)(A.max.first - A.min.first) * (A.max.first - A.min.first)
	 + (A.max.second - A.min.second) * (A.max.second - A.min.second));
	if(dig + B.radius < dis)
		return false;
	return true;
}

float Distance( pair <int , int> a, pair <int , int> b )
{
	return sqrt((float)((a.first - b.first)^2 + (a.second - b.second)^2));
}


void ResolveCollisionCircle( Circle& A, Circle& B, int i, int j, TCanvas* Canvas)
{
	pair <float , float> normal;
	normal.first = B.position.first - A.position.first;
	normal.second = B.position.second - A.position.second;
	float length = sqrt(normal.first * normal.first + normal.second * normal.second);
	if(length <= A.radius + B.radius && length != 0)
	{

		normal.first /= length;
		normal.second /= length;
		pair <float , float> tang;
		tang.first = -normal.second;
		tang.second = normal.first;


		float vait = DotProduct(A.speed.direction.first,A.speed.direction.second, tang.first, tang.second);
		float vain = DotProduct(A.speed.direction.first,A.speed.direction.second, normal.first, normal.second);
		float vbit = DotProduct(B.speed.direction.first,B.speed.direction.second, tang.first, tang.second);
		float vbin = DotProduct(B.speed.direction.first,B.speed.direction.second, normal.first, normal.second);

		float ma = A.mass;
		float mb = B.mass;
		float vafn = (mb*vbin*(B.restitution+1) + vain*(ma - A.restitution*mb)) / (ma + mb);
		float vbfn = (ma*vain*(A.restitution+1) - vbin*(ma - B.restitution*mb)) / (ma + mb);

		float vaft = vait;
		float vbft = vbit;

		float xfa = vafn*normal.first + vaft*tang.first;
		float yfa = vafn*normal.second + vaft*tang.second;
		float xfb = vbfn*normal.first + vbft*tang.first;
		float yfb = vbfn*normal.second + vbft*tang.second;

		A.speed.direction.first = xfa;
		A.speed.direction.second = yfa;
		B.speed.direction.first = xfb;
		B.speed.direction.second = yfb;

		A.hide(Canvas);
		B.hide(Canvas);

		A.position.first += A.speed.direction.first;
		A.position.second += A.speed.direction.second;
		B.position.first += B.speed.direction.first;
		B.position.second += B.speed.direction.second;


		A.f_moov = false;
		B.f_moov = false;


		const float percent = 0.5;
		const float slop = 0.05;
		float penetrationDepth = length;
		pair <float, float> correction;

		correction.first = max(penetrationDepth - slop, 0.0f) / (A.inv_mass + B.inv_mass) * percent * normal.first;
		correction.second = max(penetrationDepth - slop, 0.0f) / (A.inv_mass + B.inv_mass) * percent * normal.second;


		A.position.first -= A.inv_mass * correction.first;
		A.position.second -= A.inv_mass * correction.second;
		B.position.first += B.inv_mass * correction.first;
		B.position.second += B.inv_mass * correction.second;

		A.borders();
		B.borders();

		A.paint(Canvas, i%10);
		B.paint(Canvas, j%10);

	}
}


void ResolveCollisionRectangl( Rectangl& A, Rectangl& B, int i, int j, TCanvas* Canvas)
{
	pair <float , float> normal;
	pair <float , float> normaldir;
	pair <float , float> Acenter;
	pair <float , float> Bcenter;

	float penetrationDepth;
	Acenter.first = (A.max.first - A.min.first) / 2 + A.min.first;
	Acenter.second = (A.max.second - A.min.second) / 2 + A.min.second;
	Bcenter.first = (B.max.first - B.min.first) / 2 + B.min.first;
	Bcenter.second = (B.max.second - B.min.second) / 2 + B.min.second;

	normal.first = Bcenter.first - Acenter.first;
	normal.second = Bcenter.second - Acenter.second;

	pair <float , float> tang;
	tang.first = -normaldir.second;
	tang.second = normaldir.first;
	float length = sqrt(normal.first * normal.first + normal.second * normal.second);

	normaldir.first = normal.first / length;
	normaldir.second = normal.second / length;


	float vait = DotProduct(A.speed.direction.first,A.speed.direction.second, tang.first, tang.second);
	float vain = DotProduct(A.speed.direction.first,A.speed.direction.second, normaldir.first, normaldir.second);
	float vbit = DotProduct(B.speed.direction.first,B.speed.direction.second, tang.first, tang.second);
	float vbin = DotProduct(B.speed.direction.first,B.speed.direction.second, normaldir.first, normaldir.second);

	float ma = A.mass;
	float mb = B.mass;
	float vafn = (mb*vbin*(B.restitution+1) + vain*(ma - A.restitution*mb)) / (ma + mb);
	float vbfn = (ma*vain*(A.restitution+1) - vbin*(ma - B.restitution*mb)) / (ma + mb);

	float vaft = vait;
	float vbft = vbit;

	float xfa = vafn*normaldir.first + vaft*tang.first;
	float yfa = vafn*normaldir.second + vaft*tang.second;
	float xfb = vbfn*normaldir.first + vbft*tang.first;
	float yfb = vbfn*normaldir.second + vbft*tang.second;

	A.speed.direction.first = xfa;
	A.speed.direction.second = yfa;
	B.speed.direction.first = xfb;
	B.speed.direction.second = yfb;

	A.hide(Canvas);
	B.hide(Canvas);

	A.max.first += A.speed.direction.first;
	A.min.first += A.speed.direction.first;
	A.max.second += A.speed.direction.second;
	A.min.second += A.speed.direction.second;
	B.max.first += B.speed.direction.first;
	B.min.first += B.speed.direction.first;
	B.max.second += B.speed.direction.second;
	B.min.second += B.speed.direction.second;


	A.f_moov = false;
	B.f_moov = false;


	const float percent = 0.4;
	const float slop = 0.01;
	penetrationDepth = length;
	pair <float, float> correction;

	correction.first = max(penetrationDepth - slop, 0.0f) / (A.inv_mass + B.inv_mass) * percent * normaldir.first;
	correction.second = max(penetrationDepth - slop, 0.0f) / (A.inv_mass + B.inv_mass) * percent * normaldir.second;

	if(correction.first == 0 && correction.second == 0)
	{
		correction.first = 10;
		correction.second = 10;
	}
	A.max.first -= A.inv_mass * correction.first;
	A.min.first -= A.inv_mass * correction.first;
	A.max.second -= A.inv_mass * correction.second;
	A.min.second -= A.inv_mass * correction.second;
	B.max.first += B.inv_mass * correction.first;
	B.min.first += B.inv_mass * correction.first;
	B.max.second += B.inv_mass * correction.second;
	B.min.second += B.inv_mass * correction.second;

	A.borders();
	B.borders();

	A.paint(Canvas, i%10);
	B.paint(Canvas, j%10);
}


void ResolveCollisionRectanglvsCircle( Rectangl& A, Circle& B, int i, int j, TCanvas* Canvas)
{
	pair<float , float> Acenter;
	pair <float , float> Bcenter;
	pair <float , float> normal;
	int x_min = 100000;
	int y_min = 100000;
	int xdis = 100000;
	int ydis = 100000;
	int l;
	Acenter.first = (A.max.first - A.min.first) / 2 + A.min.first;
	Acenter.second = (A.max.second - A.min.second) / 2 + A.min.second;
	if (Acenter.first <= B.position.first && Acenter.second >= B.position.second)
	{
		for (l = Acenter.first; l <= A.max.first; l++)
		{
			if(B.position.first - B.radius < l)
				if(xdis > B.position.first - l && B.position.first - l != 0 && B.position.first - l >= 0)
				{
					xdis = B.position.first - l;
					x_min = l;
				}
		}
		for (l = Acenter.second; l <= A.max.second; l++)
		{
			if(B.position.second + B.radius > l)
				if(ydis > l - B.position.second && l - B.position.first != 0 && l - B.position.second >= 0)
				{
					ydis = l - B.position.second ;
					y_min = l;
				}
		}
	}
	else if (Acenter.first > B.position.first && Acenter.second > B.position.second)
	{
		for (l = A.min.first; l < Acenter.first; l++)
		{
			if(B.position.first + B.radius > l)
				if(xdis > l - B.position.first && l - B.position.first != 0 && l - B.position.first >= 0)
				{
					xdis = l - B.position.first;
					x_min = l;
				}
		}
		for (l = A.min.second; l < Acenter.second; l++)
		{
			if(B.position.second + B.radius > l)
				if(ydis > l - B.position.second && l -B.position.first != 0 && l - B.position.second >= 0)
				{
					ydis = l - B.position.second;
					y_min = l;
				}
		}
	}
	else if (Acenter.first >= B.position.first && Acenter.second <= B.position.second)
	{
		for (l = A.min.first; l <= Acenter.first; l++)
		{
			if(B.position.first + B.radius > l)
				if(xdis > l - B.position.first && l - B.position.first != 0 && l - B.position.first >= 0)
				{
					xdis = l - B.position.first;
					x_min = l;
				}
		}
		for (l = Acenter.second; l <= A.max.second; l++)
		{
			if(B.position.second - B.radius < l)
				if(ydis > B.position.second - l && B.position.first - l != 0 && B.position.second - l >= 0)
				{
					ydis = B.position.second - l;
					y_min = l;
				}
		}
	}
	else if (Acenter.first < B.position.first && Acenter.second < B.position.second)
	{
		for (l = Acenter.first; l < A.max.first; l++)
		{
			if(B.position.first - B.radius < l)
				if(xdis > B.position.first - l && B.position.first - l != 0 && B.position.first - l >= 0)
				{
					xdis = B.position.first - l;
					x_min = l;
				}
		}
		for (l = Acenter.second; l < A.max.second; l++)
		{
			if(B.position.second - B.radius < l)
				if(ydis > B.position.second - l && B.position.first - l != 0 && B.position.second - l >= 0)
				{
					ydis = B.position.second - l;
					y_min = l;
				}
		}
	}
	else
		return;

	if(x_min != 100000 && y_min != 100000)
	{
		float penetrationDepth;

		normal.first =  B.position.first - x_min;
		normal.second = B.position.second - y_min;

		float length = sqrt(normal.first * normal.first + normal.second * normal.second);
		normal.first /= length;
		normal.second /= length;

		pair <float , float> tang;
		tang.first = -normal.second;
		tang.second = normal.first;


		float vait = DotProduct(A.speed.direction.first,A.speed.direction.second, tang.first, tang.second);
		float vain = DotProduct(A.speed.direction.first,A.speed.direction.second, normal.first, normal.second);
		float vbit = DotProduct(B.speed.direction.first,B.speed.direction.second, tang.first, tang.second);
		float vbin = DotProduct(B.speed.direction.first,B.speed.direction.second, normal.first, normal.second);

		float ma = A.mass;
		float mb = B.mass;
		float vafn = (mb*vbin*(B.restitution+1) + vain*(ma - A.restitution*mb)) / (ma + mb);
		float vbfn = (ma*vain*(A.restitution+1) - vbin*(ma - B.restitution*mb)) / (ma + mb);

		float vaft = vait;
		float vbft = vbit;

		float xfa = vafn*normal.first + vaft*tang.first;
		float yfa = vafn*normal.second + vaft*tang.second;
		float xfb = vbfn*normal.first + vbft*tang.first;
		float yfb = vbfn*normal.second + vbft*tang.second;

		A.speed.direction.first = xfa;
		A.speed.direction.second = yfa;
		B.speed.direction.first = xfb;
		B.speed.direction.second = yfb;

		A.hide(Canvas);
		B.hide(Canvas);

		A.max.first += A.speed.direction.first;
		A.min.first += A.speed.direction.first;
		A.max.second += A.speed.direction.second;
		A.min.second += A.speed.direction.second;
		B.position.first += B.speed.direction.first;
		B.position.second += B.speed.direction.second;


		A.f_moov = false;
		B.f_moov = false;


		const float percent = 0.6;
		const float slop = 0.06;
		float help = sqrt((float)(B.position.first - x_min) * (B.position.first - x_min)
		 + (B.position.second - y_min) * (B.position.second - y_min));
		penetrationDepth = B.radius - help;
		pair <float, float> correction;

		correction.first = max(penetrationDepth - slop, 0.0f) / (A.inv_mass + B.inv_mass) * percent * normal.first;
		correction.second = max(penetrationDepth - slop, 0.0f) / (A.inv_mass + B.inv_mass) * percent * normal.second;

		if(correction.first == 0 && correction.second == 0)
		{
			correction.first = 10;
			correction.second = 10;
		}
		A.max.first -= A.inv_mass * correction.first;
		A.min.first -= A.inv_mass * correction.first;
		A.max.second -= A.inv_mass * correction.second;
		A.min.second -= A.inv_mass * correction.second;
		B.position.first += B.inv_mass * correction.first;
		B.position.second += B.inv_mass * correction.second;

		A.borders();
		B.borders();

		A.paint(Canvas, i%11);
		B.paint(Canvas, j%11);
	}
}
#endif

