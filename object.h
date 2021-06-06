//---------------------------------------------------------------------------

#ifndef objectH
#define objectH
#include <utility>
#include "vcl.h"
#include <math.h>
using namespace std;
//---------------------------------------------------------------------------
struct Velocity
{
	pair <float , float> direction;

	Velocity operator -(Velocity a)
	{
		return Velocity(this->direction.first - a.direction.first, this->direction.second - a.direction.second);
	}

	Velocity operator +(Velocity a)
	{
		return Velocity(this->direction.first + a.direction.first, this->direction.second + a.direction.second);
	}

	Velocity()
		: direction(NULL, NULL) {}
	Velocity(int x, int y)
		: direction(x,y) {}
};

struct Object
{
	float mass;
	float inv_mass;
	float module_speed;
	Velocity speed;
	float restitution;
	bool f_moov;
	Object(float _mass, float _restitution, int x, int y)
	{
		speed.direction.first = x;
		speed.direction.second = y;
		module_speed = sqrt((float)x * x + y * y);
		mass = _mass;
		restitution = _restitution;
		f_moov = false;
		if(_mass)
			inv_mass = 1 / _mass;
		else
			inv_mass = 0;
	}
};

struct Circle: Object
{
  int radius;
  pair <int , int> position;

  borders()
  {
	  if(position.first - this->radius <= 5 )
	  {
		speed.direction.first = -speed.direction.first * 0.8;
		position.first = 5 + this->radius;
	  }
	  if(position.second - this->radius <= 5 )
	  {
		speed.direction.second = -speed.direction.second * 0.8;
		position.second = 5 + this->radius;
	  }
	  if(position.first + this->radius >= 1090)
	  {
		speed.direction.first = -speed.direction.first * 0.8;
		position.first = 1090 - this->radius;
	  }

	  if(position.second <= 635 - (this->radius * 1.5))
		speed.direction.second += 1;
	  else
	  {
		if (speed.direction.second < 0.5)
			speed.direction.second = 0.0;
		else
			speed.direction.second = -speed.direction.second * 0.8 + speed.direction.second * 0.2;
		position.second = 635 - this->radius;
		if (speed.direction.first > 0.5)
			speed.direction.first *= 0.85;
		else
			speed.direction.first = 0.0;
	  }
  }

  move()
  {
	  position.first += speed.direction.first;
	  position.second += speed.direction.second;
  }

  paint(TCanvas* Canvas, int x)
  {
	Canvas->Pen->Width = 1;
	switch (x)
	{
		case 0: Canvas->Pen->Color = clBlue; Canvas->Brush->Color=clBlue; break;
		case 1: Canvas->Pen->Color = clRed; Canvas->Brush->Color=clRed; break;
		case 2: Canvas->Pen->Color = clBlack; Canvas->Brush->Color=clBlack; break;
		case 3: Canvas->Pen->Color = clLime; Canvas->Brush->Color=clLime; break;
		case 4: Canvas->Pen->Color = clYellow; Canvas->Brush->Color=clYellow; break;
		case 5: Canvas->Pen->Color = clGray; Canvas->Brush->Color=clGray; break;
		case 6: Canvas->Pen->Color = clFuchsia; Canvas->Brush->Color=clFuchsia; break;
		case 7: Canvas->Pen->Color = clPurple; Canvas->Brush->Color=clPurple; break;
		case 8: Canvas->Pen->Color = clTeal; Canvas->Brush->Color=clTeal; break;
		case 9: Canvas->Pen->Color = clMaroon; Canvas->Brush->Color=clMaroon; break;
	default:
		break;
	}
	Canvas->Ellipse(position.first - this->radius, position.second - this->radius, position.first + this->radius, position.second + this->radius);
	Canvas->FloodFill(position.first, position.second, clWhite, fsSurface);
  }
  hide(TCanvas* Canvas)
  {
	Canvas->Pen->Width = 1;
	Canvas->Pen->Color = clWhite;
	Canvas->Brush->Color=clWhite;
	Canvas->FloodFill(position.first, position.second, clWhite, fsSurface);
	Canvas->Ellipse(position.first - this->radius, position.second - this->radius, position.first + this->radius, position.second + this->radius);
  }
  Circle(float _mass, float _restitution, int x_center, int y_center, int _radius, int x_speed, int y_speed)
	: Object(_mass, _restitution, x_speed, y_speed),
		 position(x_center, y_center), radius(_radius)  {}
};

struct Rectangl: Object
{
	pair <int , int> min;
	pair <int , int> max;

	move()
   {
	  min.first += speed.direction.first;
	  max.first += speed.direction.first;
	  min.second += speed.direction.second;
	  max.second += speed.direction.second;
   }

   paint(TCanvas* Canvas, int x)
   {
	Canvas->Pen->Width = 1;
	switch (x)
	{
		case 9: Canvas->Pen->Color = clBlue; Canvas->Brush->Color=clBlue; break;
		case 8: Canvas->Pen->Color = clRed; Canvas->Brush->Color=clRed; break;
		case 7: Canvas->Pen->Color = clBlack; Canvas->Brush->Color=clBlack; break;
		case 6: Canvas->Pen->Color = clLime; Canvas->Brush->Color=clLime; break;
		case 5: Canvas->Pen->Color = clYellow; Canvas->Brush->Color=clYellow; break;
		case 4: Canvas->Pen->Color = clGray; Canvas->Brush->Color=clGray; break;
		case 3: Canvas->Pen->Color = clFuchsia; Canvas->Brush->Color=clFuchsia; break;
		case 2: Canvas->Pen->Color = clPurple; Canvas->Brush->Color=clPurple; break;
		case 1: Canvas->Pen->Color = clTeal; Canvas->Brush->Color=clTeal; break;
		case 0: Canvas->Pen->Color = clMaroon; Canvas->Brush->Color=clMaroon; break;
	default:
		break;
	}
	Canvas->Rectangle(min.first, min.second, max.first, max.second);
	Canvas->FloodFill((int)((min.first + max.first) / 2), (int)((min.second + max.second) / 2), clWhite, fsSurface);
   }

   hide(TCanvas* Canvas)
  {
	Canvas->Pen->Width = 1;
	Canvas->Pen->Color = clWhite;
	Canvas->Brush->Color=clWhite;
	Canvas->FloodFill((int)((min.first + max.first) / 2), (int)((min.second + max.second) / 2), clWhite, fsSurface);
	Canvas->Rectangle(min.first, min.second, max.first, max.second);
  }

  borders()
  {
	  if(min.first <= 5 )
	  {
		speed.direction.first = -speed.direction.first * 0.8;
		max.first += 6 - min.first;
		min.first = 6;

	  }
	  if(min.second<= 5 )
	  {
		speed.direction.second = -speed.direction.second * 0.8;
		max.second += 6 - min.second;
		min.second = 6;

	  }
	  if(max.first >= 1090)
	  {
		speed.direction.first = -speed.direction.first * 0.8;
		min.first -= max.first - 1089;
		max.first = 1089;
	  }

	  if(max.second <= 630 )
		speed.direction.second += 1;
	  else
	  {
		if (speed.direction.second < 0.5)
			speed.direction.second = 0.0;
		else
			speed.direction.second = -speed.direction.second * 0.8 + speed.direction.second * 0.2;
		int old = max.second;
		max.second = 635;
		min.second = max.second - (old - min.second);
		if (speed.direction.first > 0.5)
			speed.direction.first *= 0.85;
		else
			speed.direction.first = 0.0;
	  }
  }

	Rectangl(float _mass, float _restitution, int x_min, int y_min, int x_max, int y_max, int x_speed, int y_speed)
	: Object(_mass, _restitution, x_speed, y_speed),
		 min(x_min, y_min), max(x_max, y_max)  {}
};
#endif

