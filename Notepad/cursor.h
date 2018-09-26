#pragma once
#include <Windows.h>
#include "paragraph.h"
class Cursor
{
private:
	int r;
	int c;
public:
	Cursor() {}
	Cursor(string filename) 
	{
		ifstream in((filename + "cursor").c_str());
		in >> r >> c;
	}
	pair<int, int> cal_cursor_pos(Paragraph* first_paragraph, Paragraph* current_paragraph, int current_paragrah_text_pos, int notepad_width)
	{
		r = 0;
		c = 0;
		Paragraph* parageraph_pointer = first_paragraph;
		while (parageraph_pointer != current_paragraph)
		{
			int para_text_size = int(parageraph_pointer->get_text().size());
			r += (para_text_size + notepad_width) / notepad_width;
			parageraph_pointer = parageraph_pointer->get_next();
		}
		int cur_text_size = current_paragrah_text_pos;
		r += cur_text_size / notepad_width;
		c = cur_text_size % notepad_width;
		return make_pair(r, c);
	}

	void set_cusor(int r, int c)
	{
		//cout <<endl<< r << "," << c << endl;
		COORD pos = { c,r };
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleCursorPosition(output, pos);
	}

	void write_cursor(string filename)
	{
		ofstream out((filename+"cursor").c_str());
		out << r << " " << c;
	}
};