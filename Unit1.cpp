//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include "Unit1.h"
#include "Unit2.h"
#include "colision.h"
#include "object.h"
#include <vector>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
vector <Rectangl> RecList;
vector <Circle> CirkList;


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button3Click(TObject *Sender)
{
	srand(time(NULL));
	for(int i = 0; i < 5; i++)
	{
		float mass = rand() % 15 + 5;
		float e = (rand() % 50 + 11) / 100;
		int x = rand() % 800 + 150;
		int y = rand() % 400 + 100;
		int r = rand() % 10 + 10;
		int xv = rand() % 50 - 30;
		int yv = rand() % 50 - 30;
		if (rand()%2 )
		{
			xv *= -1;
		}
		if (rand()%2 )
		{
			yv *= -1;
		}
		Circle Example(mass, e ,x, y, r, xv, yv);
		CirkList.push_back(Example);
	}
	for(int i = 0; i < CirkList.size(); i++)
	{
		CirkList[i].hide(Canvas);
		CirkList[i].paint(Canvas, i%10);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{

	for(int i = 0; i < CirkList.size(); i++)
	{
		CirkList[i].f_moov = true;
		if(CirkList[i].position.second <= 635 - (CirkList[i].radius * 1.5))
			CirkList[i].speed.direction.second += 1;
	}
	for(int i = 0; i < RecList.size(); i++)
	{
		RecList[i].f_moov = true;
		if(RecList[i].max.second <= 630 )
			RecList[i].speed.direction.second += 1;
	}
	for(int i = 0; i < CirkList.size(); i++)
	{
		for(int j = i + 1; j < CirkList.size(); j++)
		{
			ResolveCollisionCircle(CirkList[i], CirkList[j], i, j, Canvas);
		}
	}
	for(int i = 0; i < RecList.size(); i++)
	{
		for(int j = i + 1; j < RecList.size(); j++)
		{
			if(RecvsRec(RecList[i], RecList[j]))
				ResolveCollisionRectangl(RecList[i], RecList[j], i, j, Canvas);
		}
	}

	for(int i = 0; i < RecList.size(); i++)
	{
		for(int j = 0; j < CirkList.size(); j++)
		{
			if(RecvsCirk(RecList[i], CirkList[j]))
				ResolveCollisionRectanglvsCircle(RecList[i], CirkList[j], i, j, Canvas);
		}
	}
	for(int i = 0; i < CirkList.size(); i++)
	{
		if (CirkList[i].f_moov)
		{   if(fabs(CirkList[i].speed.direction.first) + fabs(CirkList[i].speed.direction.second) > 0.0)
			{
				CirkList[i].hide(Canvas);
				CirkList[i].borders();
				CirkList[i].move();
			}
		}
	}
	for(int i = 0; i < RecList.size(); i++)
	{
		if (RecList[i].f_moov)
		{   if(fabs(RecList[i].speed.direction.first) + fabs(RecList[i].speed.direction.second) > 0.0)
			{
				RecList[i].hide(Canvas);
				RecList[i].borders();
				RecList[i].move();
			}
		}
	}
	for(int i = 0; i < CirkList.size(); i++)
	{
		if (CirkList[i].f_moov)
			CirkList[i].paint(Canvas, i%10);
	}
	for(int i = 0; i < RecList.size(); i++)
	{
		if (RecList[i].f_moov)
			RecList[i].paint(Canvas, i%10);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{

	for(int i = 0; i < CirkList.size(); i++)
	{
		CirkList[i].f_moov = true;
		if(CirkList[i].position.second <= 635 - (CirkList[i].radius * 1.5))
			CirkList[i].speed.direction.second += 1;
	}
	for(int i = 0; i < RecList.size(); i++)
	{
		RecList[i].f_moov = true;
		if(RecList[i].max.second <= 630 )
			RecList[i].speed.direction.second += 1;
	}
	for(int i = 0; i < CirkList.size(); i++)
	{
		for(int j = i + 1; j < CirkList.size(); j++)
		{
			ResolveCollisionCircle(CirkList[i], CirkList[j], i, j, Canvas);
		}
	}
	for(int i = 0; i < RecList.size(); i++)
	{
		for(int j = i + 1; j < RecList.size(); j++)
		{
			if(RecvsRec(RecList[i], RecList[j]))
				ResolveCollisionRectangl(RecList[i], RecList[j], i, j, Canvas);
		}
	}

	for(int i = 0; i < RecList.size(); i++)
	{
		for(int j = 0; j < CirkList.size(); j++)
		{
			if(RecvsCirk(RecList[i], CirkList[j]))
				ResolveCollisionRectanglvsCircle(RecList[i], CirkList[j], i, j, Canvas);
		}
	}
	for(int i = 0; i < CirkList.size(); i++)
	{
		if (CirkList[i].f_moov)
		{   if(fabs(CirkList[i].speed.direction.first) + fabs(CirkList[i].speed.direction.second) > 0.0)
			{
				CirkList[i].hide(Canvas);
				CirkList[i].borders();
				CirkList[i].move();
			}
		}
	}
	for(int i = 0; i < RecList.size(); i++)
	{
		if (RecList[i].f_moov)
		{   if(fabs(RecList[i].speed.direction.first) + fabs(RecList[i].speed.direction.second) > 0.0)
			{
				RecList[i].hide(Canvas);
				RecList[i].borders();
				RecList[i].move();
			}
		}
	}
	for(int i = 0; i < CirkList.size(); i++)
	{
		if (CirkList[i].f_moov)
			CirkList[i].paint(Canvas, i%10);
	}
	for(int i = 0; i < RecList.size(); i++)
	{
		if (RecList[i].f_moov)
			RecList[i].paint(Canvas, i%10);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
    Canvas->MoveTo(0, 0);
	Canvas->Pen->Color = clBlack;
	Canvas->Pen->Width = 5;
	Canvas->LineTo(1093, 0);
	Canvas->LineTo(1093, 638);
	Canvas->LineTo(0, 638);
	Canvas->LineTo(0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
    Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender)
{
	srand(time(NULL));
	for(int i = 0; i < 5; i++)
	{
		float mass = rand() % 15 + 5;
		float e = (rand() % 50 + 11) / 100;
		int x1 = rand() % 800 + 150;
		int y1 = rand() % 400 + 100;
		int x2 = rand() % 20 + 15 + x1;
		int y2 = rand() % 20 + 15 + y1;
		int xv = rand() % 50 - 30;
		int yv = rand() % 50 - 30;
		if (rand()%2 )
		{
			xv *= -1;
		}
		if (rand()%2 )
		{
			yv *= -1;
		}
		Rectangl Example(mass, e ,x1, y1, x2, y2, xv, yv);
		RecList.push_back(Example);
	}
	for(int i = 0; i < RecList.size(); i++)
	{
		RecList[i].hide(Canvas);
		RecList[i].paint(Canvas, i%10);
	}
}
//---------------------------------------------------------------------------


