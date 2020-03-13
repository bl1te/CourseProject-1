#include <iostream>
#include <string>
using namespace std;

struct menuItem
{
	string itemName;
	int itemPos = 0;
	bool flag = false;
	menuItem* prev = NULL;
	menuItem* next = NULL;
}
* menuTop = NULL, * menuBot = NULL, * tempItem = NULL, * currentPos = NULL;
int itemNo = 0;

void addMenuItem(string itemName, bool flag)
{
	if (menuTop == NULL)
	{
		tempItem = new menuItem;
		tempItem->itemName = itemName;
		tempItem->itemPos = ++itemNo;
		tempItem->flag = flag;
		tempItem->prev = tempItem;
		tempItem->next = tempItem;
		menuTop = menuBot = currentPos = tempItem;
	}
	else
	{
		tempItem = new menuItem;
		tempItem->itemName = itemName;
		tempItem->itemPos = ++itemNo;
		tempItem->flag = flag;
		tempItem->prev = menuBot;
		tempItem->next = menuTop;
		menuBot->next = tempItem;
		menuBot = tempItem;
		menuTop->prev = menuBot;
	}
}

void showMenu()
{
	tempItem = menuTop;
	for (int i = 0; i < itemNo; ++i)
	{
		if (tempItem->flag == true)
			cout << "=>  " << tempItem->itemName << endl;
		else
			cout << "   " << tempItem->itemName << endl;
		tempItem = tempItem->next;
	}
	tempItem = NULL;
}

void scrollUp()
{
	currentPos->flag = false;
	currentPos = currentPos->prev;
	currentPos->flag = true;
}

void scrollDown()
{
	currentPos->flag = false;
	currentPos = currentPos->next;
	currentPos->flag = true;
}

void clearMenu()
{
	menuItem* tempItem;
	for ( ; menuTop != menuBot; )
	{
		tempItem = menuTop;
		menuTop = menuTop->next;
		delete tempItem;
	}
	delete menuBot;
	menuTop = menuBot = tempItem = NULL;
}