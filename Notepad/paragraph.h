#pragma once
#include<iostream>
#include<list>
#include<algorithm>
#include <fstream>
using namespace std;

class Paragraph
{
	string text;
	Paragraph* next;
	Paragraph* prev;
public:
	Paragraph(string text = "") :text(text)
	{
		next = NULL;
		prev = NULL;
	}

	void set_next(Paragraph* n)
	{
		next = n;
	}

	void set_prev(Paragraph* p)
	{
		prev = p;
	}

	string get_text()
	{
		return text;
	}

	Paragraph* get_next()
	{
		return next;
	}

	Paragraph* get_prev()
	{
		return prev;
	}

	string cut_tail_of_text(int pos)
	{
		string prev = pos > 0 ? text.substr(0, pos) : "";
		string next = pos < text.size() ? text.substr(pos) : "";
		text = prev;
		return next;
	}

	void insert_char(char ch, int pos)
	{
		string prev = pos > 0 ? text.substr(0, pos) : "";
		string next = pos < text.size() ? text.substr(pos) : "";
		text = prev + ch + next;
	}

	void insert_string(string str, int pos)
	{
		string prev = pos > 0 ? text.substr(0, pos) : "";
		string next = pos < text.size() ? text.substr(pos) : "";
		text = prev + str + next;
	}

	void remove_char(int pos)
	{
		string prev = pos > 0 ? text.substr(0, pos) : "";
		string next = pos + 1 < text.size() ? text.substr(pos + 1) : "";
		text = prev + next;
	}
};

class Paragraph_controller
{
private:
	class Context
	{
	public:
		Paragraph * cur;
		int pos;
		Context()
		{

		}
		Context(Paragraph* first_paragraph) :cur(first_paragraph)
		{
			pos = 0;
		}
	};
	Paragraph* first_paragraph;
	Context context;
	int notepad_width;

public:
	Paragraph_controller()
	{

	}

	Paragraph_controller(int notepad_width, string filename) :notepad_width(notepad_width)
	{
		Paragraph* tmp = new Paragraph();	
		Paragraph* p = tmp;
		ifstream in(filename.c_str());
		while (in.peek() != EOF)
		{
			string s;
			in >> s;
			p->set_next(new Paragraph(s));
			p = p->get_next();
		}
		p = tmp->get_next();
		delete tmp;
		first_paragraph = p;
		context = Context(first_paragraph);
	}
	Paragraph_controller(int notepad_width, Paragraph* first_paragraph):notepad_width(notepad_width), first_paragraph(first_paragraph)
	{		
		context = Context(first_paragraph);
	}
	~Paragraph_controller()
	{
		Paragraph* p = first_paragraph;
		while (p)
		{
			Paragraph* next = p->get_next();
			delete(p);
			p = next;
		}
	}
	int get_text_pos()
	{
		return context.pos;
	}

	Paragraph* get_current_paragraph()
	{
		return context.cur;
	}

	Paragraph* get_first_paragraph()
	{
		return first_paragraph;
	}

	void insert_char(char ch)
	{
		context.cur->insert_char(ch, context.pos);
		context.pos++;
	}

	string cut_tail_of_text()
	{
		return context.cur->cut_tail_of_text(context.pos);
	}

	void create_new_paragraph(string text = "")
	{
		Paragraph* t_next = context.cur->get_next();
		context.cur->set_next(new Paragraph(text));
		context.cur->get_next()->set_prev(context.cur);
		context.cur = context.cur->get_next();
		if (t_next)
		{
			context.cur->set_next(t_next);
			t_next->set_prev(context.cur);
		}
		context.pos = 0;
	}

	bool delete_paragraph(Paragraph* del_para)
	{
		Paragraph* prev = del_para->get_prev();
		Paragraph* next = del_para->get_next();
		if (prev == NULL)return false;
		prev->set_next(next);
		if(next)next->set_prev(prev);
		delete(del_para);
		return true;
	}

	void back_space()
	{
		if (context.pos == 0)
		{
			Paragraph* prev = context.cur->get_prev();
			if (prev)
			{
				Paragraph* t_cur = context.cur;
				context.cur = prev;
				context.pos = context.cur->get_text().size();
				prev->insert_string(t_cur->get_text(), prev->get_text().size());
				delete_paragraph(t_cur);
			}
		}
		else
		{
			context.cur->remove_char(context.pos - 1);
			context.pos--;
		}
	}

	void del()
	{
		if (context.pos == context.cur->get_text().size())
		{
			Paragraph* next = context.cur->get_next();
			if (next)
			{
				context.cur->insert_string(next->get_text(), context.pos);
				delete_paragraph(next);
			}
		}
		else
		{
			context.cur->remove_char(context.pos);
		}
		
	}

	void up()
	{
		
		if (notepad_width <= context.pos)
		{
			context.pos = context.pos - notepad_width;
		}
		else if(context.cur->get_prev())
		{
			int width = context.pos;
			context.cur = context.cur->get_prev();
			context.pos = min(width, int(context.cur->get_text().size() % notepad_width));
		}
	}

	void down()
	{
		int width = context.pos % notepad_width;
		int left = context.cur->get_text().size() - context.pos;
		if (notepad_width <= left)
		{
			context.pos = context.pos + notepad_width;
		}
		else if (notepad_width <= width + left)
		{
			context.pos = context.pos + left;
		}
		else if (context.cur->get_next())//last line
		{
			Paragraph* next = context.cur->get_next();
			context.cur = next;
			context.pos = min(width, int(next->get_text().size()));
		}
		
	}

	void left()
	{
		if (context.pos == 0)
		{
			Paragraph* prev = context.cur->get_prev();
			if (prev)
			{
				context.cur = prev;
				context.pos = prev->get_text().size();
			}
		}
		else
		{
			context.pos--;
		}
	}

	void right()
	{
		if (context.pos == context.cur->get_text().size())
		{
			Paragraph* next = context.cur->get_next();
			if (next)
			{
				context.cur = next;
				context.pos = 0;
			}
		}
		else
		{
			context.pos++;
		}
	}

	void write_paragraph(string filename)
	{
		Paragraph* p = first_paragraph;
		ofstream out(filename.c_str());
		while (p)
		{
			out << p->get_text().c_str() << " ";
			p = p->get_next();
		}
	}

	
	
};