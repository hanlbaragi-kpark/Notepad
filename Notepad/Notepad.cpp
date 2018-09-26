#include <iostream>
#include <vector>
#include <sstream>
#include "paragraph.h"
#include "cursor.h"
#include"conio.h"
using namespace std;

Paragraph_controller* paragraph_controller;
Cursor* cursor;
int width;
void print_info();
void print_paper();

void init()
{
	width = 50;
	paragraph_controller = new Paragraph_controller(width, new Paragraph());
	cursor = new Cursor();
}

void save()
{
	string filename = "save.txt";
	paragraph_controller->write_paragraph(filename);
	cursor->write_cursor(filename);
}

void open()
{
	string filename = "save.txt";
	Paragraph_controller* del_paragraph_controller = paragraph_controller;
	Cursor* del_cursor = cursor;
	width = 50;
	paragraph_controller = new Paragraph_controller(width, filename);
	cursor = new Cursor(filename);
	delete del_paragraph_controller;
	delete del_cursor;
	print_paper();
	print_info();
}


void set_char_extended(char ch)
{
	if (ch == char(0x48))//up
	{
		paragraph_controller->up();
	}
	else if (ch == char(0x50))//down
	{
		paragraph_controller->down();
	}
	else if (ch == char(0x4b))//left
	{
		paragraph_controller->left();
	}
	else if (ch == char(0x4d))//right
	{
		paragraph_controller->right();
	}
	else if (ch == char(0x53))//del
	{
		paragraph_controller->del();
	}
	
}

void set_char(char ch)
{
	if (ch == char(0x13))//ctrl + s(ave)
	{
		save();
	}
	else if (ch == char(0x0f))////ctrl + o(pen)
	{
		open();
	}
	else if (ch == char(0x08))//back space
	{
		paragraph_controller->back_space();
	}
	else if (ch == '\r')
	{
		string cut = paragraph_controller->cut_tail_of_text();
		paragraph_controller->create_new_paragraph(cut);
	}
	else
	{
		paragraph_controller->insert_char(ch);
	}
	int debug = 1;
}



void print_paper()
{
	system("CLS");
	for (Paragraph* p = paragraph_controller->get_first_paragraph(); p != NULL; p = p->get_next())
	{
		string text = p->get_text();
		for (int cur = 0 ; cur < text.size() ; cur++)
		{
			char ch = text[cur];
			if (ch != '\r') cout << ch;
			if (((cur + 1)% width) == 0)
			{
				cout << endl;
			}
		}
		cout << endl;
	}
}

void print_info()
{
	int margin = width + 10;
	pair<int, int> pos = cursor->cal_cursor_pos(paragraph_controller->get_first_paragraph(), paragraph_controller->get_current_paragraph(), paragraph_controller->get_text_pos(), width);
	cursor->set_cusor(0, margin);
	vector<string> info =
	{
		{ "Width  :   " + to_string(width) },
		{ "Cursor :   " + to_string(pos.first) + "," + to_string(pos.second)},
	};
	for (int i = 0; i < info.size(); i++)
	{
		cursor->set_cusor(i, margin);
		cout <<  info[i].c_str()<< endl;
	}
	cursor->set_cusor(pos.first, pos.second);
}

int main()
{
	init();
	while (true)
	{
		
		char ch = _getch();
		if (ch == char(0xE0) || ch == 0)
			set_char_extended(ch=_getch());
		else set_char(ch);
		print_paper();
		print_info();
		
	}
	
	return 0;
}
