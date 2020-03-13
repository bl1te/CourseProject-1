#include <iostream>
#include <string>
using namespace std;

struct s_menuItem
{
	string s_itemName;
	int s_itemPos = 0;
	bool s_flag = false;
	s_menuItem* prev = NULL;
	s_menuItem* next = NULL;
}
*s_menuTop = NULL, * s_menuBot = NULL, * s_tempItem = NULL, * s_currentPos = NULL;

int s_itemNo = 0;

void s_addMenuItem(string s_itemName, bool s_flag)
{
	if (s_menuTop == NULL)
	{
		s_tempItem = new s_menuItem;
		s_tempItem->s_itemName = s_itemName;
		s_tempItem->s_itemPos = ++s_itemNo;
		s_tempItem->s_flag = s_flag;
		s_tempItem->prev = s_tempItem;
		s_tempItem->next = s_tempItem;
		s_menuTop = s_menuBot = s_currentPos = s_tempItem;
	}
	else
	{
		s_tempItem = new s_menuItem;
		s_tempItem->s_itemName = s_itemName;
		s_tempItem->s_itemPos = ++s_itemNo;
		s_tempItem->s_flag = s_flag;
		s_tempItem->prev = s_menuBot;
		s_tempItem->next = s_menuTop;
		s_menuBot->next = s_tempItem;
		s_menuBot = s_tempItem;
		s_menuTop->prev = s_menuBot;
	}
}

void s_showMenu()
{
	s_tempItem = s_menuTop;
	for (int i = 0; i < s_itemNo; ++i)
	{
		if (s_tempItem->s_flag == true)
			cout << "=>  " << s_tempItem->s_itemName << endl;
		else
			cout << "   " << s_tempItem->s_itemName << endl;
		s_tempItem = s_tempItem->next;
	}
	s_tempItem = NULL;
}

void s_scrollUp()
{
	s_currentPos->s_flag = false;
	s_currentPos = s_currentPos->prev;
	s_currentPos->s_flag = true;
}

void s_scrollDown()
{
	s_currentPos->s_flag = false;
	s_currentPos = s_currentPos->next;
	s_currentPos->s_flag = true;
}

void s_clearMenu()
{
	s_menuItem* s_tempItem;
	for ( ; s_menuTop != s_menuBot; )
	{
		s_tempItem = s_menuTop;
		s_menuTop = s_menuTop->next;
		delete s_tempItem;
	}
	delete s_menuBot;
	s_menuTop = s_menuBot = s_tempItem = NULL;
}