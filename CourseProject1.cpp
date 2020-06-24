#include "Menu.h"
#include "subMenu.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <iomanip>

struct info
{
	int ID;             // ключ
	int recBookNo;      // номер зачётной книжки студента
	char studName[100]; // Ф. И. О. студента
	int groupCode;      // шифр группы (6 символов)
	int bdate[3];       // дата рождения
	int admYear;        // год поступления
	int score;          // вступительный балл
};

struct record
{
	record* prev;
	info data;
	record* next;
};

void orgSubMenu(int call);

/* 1. Начальное создание таблицы.
Загрузка таблицы из типизированного файла при новом запуске программы. */
record* loadBFile()
{
	FILE* fin;
	fopen_s(&fin, "binary.txt", "rb");
	record* bfbeg = new record;
	int z;
	if (!fin)
	{
		std::cout << "\nUnable to open\n";
		return 0;
	}
	fread(&bfbeg->data, sizeof(info), 1, fin);
	bfbeg->prev = bfbeg->next = NULL;
	record* temp = bfbeg;
	while (1)
	{
		record* bfnew = new record;
		z = fread(&bfnew->data, sizeof(info), 1, fin);
		if (z != 1)
		{
			break;
		}
		bfnew->prev = temp;
		bfnew->next = NULL;
		temp = temp->next = bfnew;
	}
	fclose(fin);
	std::cout << "\nFile 'binary.txt' loaded\n";
	return bfbeg;
}

/* При необходимости создания новой таблицы, исходные данные считываются из текстового файла.
Имя файла должен задавать пользователь. */
info readBlank(string str) // Считывание данных
{
	info temp;
	int i = 0;
	char c = str[i];
	int tempID = c - '0';
	c = str[++i];
	while (c != ' ')
	{
		tempID = tempID * 10 + (c - '0');
		c = str[++i];
	}
	temp.ID = tempID;
	c = str[++i];
	int tempRecBookNo = c - '0';
	c = str[++i];
	while (c != ' ')
	{
		tempRecBookNo = tempRecBookNo * 10 + (c - '0');
		c = str[++i];
	}
	temp.recBookNo = tempRecBookNo;
	c = str[++i];
	int spcCnt = 0, j;
	for (j = 0; spcCnt != 3; ++j)
	{
		temp.studName[j] = c;
		c = str[++i];
		if (c == ' ')
			spcCnt++;
	}
	temp.studName[j] = '\0';
	c = str[++i];
	int tempGroupCode = c - '0';
	c = str[++i];
	while (c != ' ')
	{
		tempGroupCode = tempGroupCode * 10 + (c - '0');
		c = str[++i];
	}
	temp.groupCode = tempGroupCode;
	c = str[++i];
	int tempBD[3];
	tempBD[0] = c - '0';
	c = str[++i];
	while (c != ' ')
	{
		tempBD[0] = tempBD[0] * 10 + (c - '0');
		c = str[++i];
	}
	temp.bdate[0] = tempBD[0];
	c = str[++i];
	tempBD[1] = c - '0';
	c = str[++i];
	while (c != ' ')
	{
		tempBD[1] = tempBD[1] * 10 + (c - '0');
		c = str[++i];
	}
	temp.bdate[1] = tempBD[1];
	c = str[++i];
	tempBD[2] = c - '0';
	c = str[++i];
	while (c != ' ')
	{
		tempBD[2] = tempBD[2] * 10 + (c - '0');
		c = str[++i];
	}
	temp.bdate[2] = tempBD[2];
	c = str[++i];
	int tempAdmYear = c - '0';
	c = str[++i];
	while (c != ' ')
	{
		tempAdmYear = tempAdmYear * 10 + (c - '0');
		c = str[++i];
	}
	temp.admYear = tempAdmYear;
	c = str[++i];
	int tempScore = c - '0';
	c = str[++i];
	while (c != '\0')
	{
		tempScore = tempScore * 10 + (c - '0');
		c = str[++i];
	}
	temp.score = tempScore;
	return temp;
}

record* createTFile() // Открытие файла и перенос данных
{
	ifstream fin;
	string filePath;
	std::cout << "\nEnter new file path: \n\n";
	std::cin >> filePath;
	fin.open(filePath);
	if (!fin)
	{
		std::cout << "\nInvalid file path\n";
		return 0;
	}
	string unit;
	record* tfbeg = new record;
	getline(fin, unit);
	tfbeg->data = readBlank(unit);
	tfbeg->prev = tfbeg->next = NULL;
	record* temp = tfbeg;
	while (getline(fin, unit))
	{
		record* tfnew = new record;
		tfnew->data = readBlank(unit);
		tfnew->prev = temp;
		tfnew->next = NULL;
		temp = temp->next = tfnew;
	}
	fin.close();
	std::cout << "\nNew file created\n";
	return tfbeg;
}

// 2. Просмотр таблицы. При этом необходимо предусмотреть возможность скроллинга.
int showTable(record* el, bool del, bool edit)
{
	record* temp = el;
	int initKey = 0;
	int naviKey = 0;
	int i = 0;
	while (1)
	{
		system("cls");
		std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		std::cout << "| ID | Record book number |               Name               | Group code | Birth date | Admission year | Score |\n";
		std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		i = 0;
		while (el)
		{
			i++;
			if (i == 1)
			{
				temp = el;
			}
			std::cout << "| " << setw(2) << el->data.ID << ' '
				<< "| " << setw(18) << el->data.recBookNo << ' '
				<< "| " << setw(32) << el->data.studName << ' '
				<< "| " << setw(10) << el->data.groupCode << ' '
				<< "| " << setw(2) << el->data.bdate[0] << '.'
				<< setw(2) << el->data.bdate[1] << '.'
				<< el->data.bdate[2] << ' '
				<< "| " << setw(14) << el->data.admYear << ' '
				<< "| " << setw(5) << el->data.score << " |\n";
			std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
			if ((i == 10) || (!el->next))
			{
				break;
			}
			el = el->next;
		}
		if (edit)
		{
			std::cout << "\nPress [E] to choose element to edit";
		}
		if (del)
		{
			std::cout << "\nPress [D] to choose element to delete";
		}
		i = 0;
		initKey = _getch();
		if (initKey == 224)
		{
			switch (naviKey = _getch())
			{
			case 72: case 75: // Вверх, Влево
			{
				el = temp;
				if (el->prev)
				{
					while (i < 10)
					{
						i++;
						el = el->prev;
					}
				}
				continue;
			}
			case 77: case 80: // Вправо, Вниз
			{
				if (!el->next)
				{
					el = temp;
				}
				else
				{
					el = el->next;
				}
				continue;
			}
			}
		}
		if ((del) && (initKey == 'd'))
		{
			return 0;
		}
		if ((edit) && (initKey == 'e'))
		{
			return 0;
		}
		if ((initKey == 8) || (initKey == 27)) // Backspace, Escape
		{
			return 1;
		}
		else
		{
			el = temp;
			continue;
		}
	}
}

// 3. Добавление новой записи в таблицу.
void bDateInput(record* temp) // Заполнение поля "Дата рождения"
{
	std::cout << "Birth date: \n";
	while (1)
	{
		while (1)
		{
			std::cout << "Day: ";
			std::cin >> temp->data.bdate[0];
			if ((temp->data.bdate[0] < 1) || (temp->data.bdate[0] > 31))
			{
				std::cout << "Invalid input. Enter [1 - 31] value\n";
				system("pause");
				continue;
			}
			else
			{
				break;
			}
		}
		while (1)
		{
			std::cout << "Month: ";
			std::cin >> temp->data.bdate[1];
			if ((temp->data.bdate[1] < 1) || (temp->data.bdate[1] > 12))
			{
				std::cout << "Invalid input. Enter [1 - 12] value\n";
				system("pause");
				continue;
			}
			else
			{
				break;
			}
		}
		if ((temp->data.bdate[0] > 29) && (temp->data.bdate[1] == 2))
		{
			std::cout << "Invalid input. Enter [1 - 29] day value or different month value\n";
			system("pause");
			continue;
		}
		if (((temp->data.bdate[1] != 2) && (temp->data.bdate[1] % 2 == 0) && (temp->data.bdate[1] < 7)) && (temp->data.bdate[0] > 30))
		{
			std::cout << "Invalid input. Enter [1 - 30] day number or [1, 3, 5, 7, 8, 10, 12] month number\n";
			system("pause");
			continue;
		}
		if (((temp->data.bdate[1] % 2 == 1) && (temp->data.bdate[1] > 8)) && (temp->data.bdate[0] > 30))
		{
			std::cout << "Invalid input. Enter [1 - 30] value\n";
			system("pause");
			continue;
		}
		break;
	}
	while (1)
	{
		if ((temp->data.bdate[0] == 29) && (temp->data.bdate[1] == 2))
		{
			std::cout << "(Leap) ";
		}
		std::cout << "Year: ";
		std::cin >> temp->data.bdate[2];
		if ((temp->data.bdate[2] < 1975) || (temp->data.bdate[2] > 2000))
		{
			std::cout << "Invalid input. Enter [1975 - 2000] value\n";
			system("pause");
			continue;
		}
		if ((temp->data.bdate[0] == 29) && (temp->data.bdate[1] == 2) && (temp->data.bdate[2] % 4 != 0))
		{
			std::cout << "Invalid input. Enter a leap year value [1976, 1980, 1984, 1988, 1992, 1996, 2000]\n";
			system("pause");
			continue;
		}
		else
		{
			break;
		}
	}
}

void fillData(record* temp) // Заполнение полей
{
	while (1)
	{
		std::cout << "Record book number: ";
		std::cin >> temp->data.recBookNo;
		if (!std::cin)
		{
			std::cout << "Invalid input\n";
			std::cin.clear();
			std::cin.ignore(255, '\n');
			continue;
		}
		else
		{
			break;
		}
	}
	std::cin.clear();
	std::cin.ignore();
	std::cout << "Name: ";
	gets_s(temp->data.studName);
	while (1)
	{
		std::cout << "Group code: ";
		std::cin >> temp->data.groupCode;
		if ((temp->data.groupCode < 10601) || (temp->data.groupCode > 580401))
		{
			std::cout << "Invalid input\n";
			system("pause");
			continue;
		}
		else
		{
			break;
		}
	}
	bDateInput(temp);
	while (1)
	{
		std::cout << "Admission year: ";
		std::cin >> temp->data.admYear;
		if ((temp->data.admYear < 2015) || (temp->data.admYear > 2019))
		{
			std::cout << "Invalid input. Enter [2015 - 2019] value\n";
			system("pause");
			continue;
		}
		else
		{
			break;
		}
	}
	while (1)
	{
		std::cout << "Score: ";
		std::cin >> temp->data.score;
		if ((temp->data.score < 3) || (temp->data.score > 300))
		{
			std::cout << "Invalid input. Enter [3 - 300] value\n";
			system("pause");
			continue;
		}
		else
		{
			break;
		}
	}
	std::cout << '\n';
}

record* addItem(record* el) // Добавление
{
	system("cls");
	std::cout << "Adding new element\n\n";
	record* temp = new record;
	fillData(temp);
	temp->next = NULL;
	if (el == NULL)
	{
		el = temp;
		el->prev = NULL;
		temp->data.ID = 1;
		std::cout << "First element added\n";
		return el;
	}
	else
	{
		record* head = el;
		int n = 1;
		while (el->next)
		{
			n++;
			el = el->next;
		}
		temp->prev = el;
		el->next = temp;
		temp->data.ID = n + 1;
		std::cout << temp->data.ID << " element added\n";
		return head;
	}
}

/* 4. Удаление записи. Удаляемый элемент выбирается по одному из полей таблицы (ключевому).
Ключевое поле выбирает студент. */
record* delItem(record* el)
{
	if ((!el->prev) && (!el->next))
	{
		std::cout << "\nDeleting only one element\n";
		el->next = NULL;
		el = NULL;
		delete el;
		std::cout << "Element deleted\n";
		return 0;
	}
	if (showTable(el, 1, 0))
	{
		return el;
	}
	record* head = el;
	int delID;
	bool exists = false;
	while (1)
	{
		std::cout << "\nEnter the element ID to delete: ";
		std::cin >> delID;
		if (!std::cin)
		{
			std::cout << "Invalid input\n";
			std::cin.clear();
			std::cin.ignore(255, '\n');
			continue;
		}
		else
		{
			break;
		}
	}
	while (el)
	{
		if (el->data.ID == delID)
			exists = true;
		el = el->next;
	}
	el = head;
	if (!exists)
	{
		std::cout << "\nNo such element\n";
		system("pause");
		return el;
	}
	while (el->data.ID != delID)
		el = el->next;
	record* temp = el;
	if ((el->prev) && (el->next))
	{
		int outID = el->data.ID;
		temp->prev->next = el->next;
		temp->next->prev = el->prev;
		while (temp)
		{
			temp->data.ID--;
			temp = temp->next;
		}
		el = NULL;
		delete el;
		std::cout << outID << " element deleted\n";
		return head;
	}
	if (!el->prev)
	{
		el = el->next;
		record* head2 = el;
		while (el)
		{
			el->data.ID--;
			el = el->next;
		}
		delete temp;
		std::cout << "First element deleted\n";
		return head2;
	}
	if (!el->next)
	{
		el = el->prev;
		el->next = NULL;
		delete temp;
		std::cout << "Last element deleted\n";
		return head;
	}
}

/* 5. Корректировка записи в таблице.
Корректируемую запись выбирают по одному из полей таблицы (ключевому). */
record* editItem(record* el)
{
	if (showTable(el, 0, 1))
	{
		return el;
	}
	orgSubMenu(1);
	record* head = el;
	int initKey = 0;
	int menuKey = 0;
	int editID = 0;
	bool exists = false;
	if (el->next)
	{
		while (1)
		{
			std::cout << "\nEnter the element ID to edit: ";
			std::cin >> editID;
			if (!std::cin)
			{
				std::cout << "Invalid input\n";
				std::cin.clear();
				std::cin.ignore(255, '\n');
				continue;
			}
			else
			{
				break;
			}
		}
		while (el)
		{
			if (el->data.ID == editID)
			{
				exists = true;
			}
			el = el->next;
		}
		el = head;
		if (!exists)
		{
			std::cout << "\nNo such element\n";
			system("pause");
			return el;
		}
		while (el->data.ID != editID)
		{
			el = el->next;
		}
	}
	if ((!el->prev) && (!el->next))
	{
		std::cout << "\n\nOnly one element\n\n";
	}
	while (1)
	{
		system("cls");
		std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		std::cout << "| ID | Record book number |               Name               | Group code | Birth date | Admission year | Score |\n";
		std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		std::cout << "| " << setw(2) << el->data.ID << ' '
			<< "| " << setw(18) << el->data.recBookNo << ' '
			<< "| " << setw(32) << el->data.studName << ' '
			<< "| " << setw(10) << el->data.groupCode << ' '
			<< "| " << setw(2) << el->data.bdate[0] << '.'
			<< setw(2) << el->data.bdate[1] << '.'
			<< el->data.bdate[2] << ' '
			<< "| " << setw(14) << el->data.admYear << ' '
			<< "| " << setw(5) << el->data.score << " |\n";
		std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		std::cout << "\nEdit:\n";
		s_showMenu();
		initKey = _getch();
		if (initKey == 224)
		{
			menuKey = _getch();
			if (menuKey == 72)
			{
				s_scrollUp();
			}
			if (menuKey == 80)
			{
				s_scrollDown();
			}
		}
		if (initKey == 13)
		{
			switch (s_currentPos->s_itemPos)
			{
			case 1: // Номер зачётной книжки
			{
				while (1)
				{
					std::cout << "Record book number: ";
					std::cin >> el->data.recBookNo;
					if (!std::cin)
					{
						std::cout << "Invalid input\n";
						std::cin.clear();
						std::cin.ignore(255, '\n');
						continue;
					}
					else
					{
						break;
					}
				}
				break;
			}
			case 2: // Ф. И. О.
			{
				std::cout << "\nName: ";
				std::cin.clear();
				std::cin.ignore();
				gets_s(el->data.studName);
				break;
			}
			case 3: // Шифр группы
			{
				while (1)
				{
					std::cout << "Group code: ", std::cin >> el->data.groupCode;
					if ((el->data.groupCode < 10601) || (el->data.groupCode > 580401))
					{
						std::cout << "Invalid input\n";
						system("pause");
						continue;
					}
					else
					{
						break;
					}
				}
				break;
			}
			case 4: // Дата рождения
			{
				bDateInput(el);
				break;
			}
			case 5: // Год поступления
			{
				while (1)
				{
					std::cout << "Admission year: ", std::cin >> el->data.admYear;
					if ((el->data.admYear < 2015) || (el->data.admYear > 2019))
					{
						std::cout << "Invalid input. Enter [2015 - 2019] value\n";
						system("pause");
						continue;
					}
					else
					{
						break;
					}
				}
				break;
			}
			case 6: // Вступительный балл
			{
				while (1)
				{
					std::cout << "Score: ", std::cin >> el->data.score;
					if ((el->data.score < 3) || (el->data.score > 300))
					{
						std::cout << "Invalid input. Enter [3 - 300] value\n";
						system("pause");
						continue;
					}
					else
					{
						break;
					}
				}
				break;
			}
			case 7: // Назад
			{
				showTable(head, 0, 0);
				return head;
			}
			}
		}
		if ((initKey == 8) || (initKey == 27))
		{
			showTable(head, 0, 0);
			return head;
		}
	}
}

/* 6. Сортировка таблицы. Сортировка производится по одному из полей таблицы (ключевому).
Метод сортировки выбирает студент. */
record* sortByID(record* el)
{
	info buffer = el->data;
	record* head = el;
	int n = 0;
	while (el->next)
	{
		n++;
		el = el->next;
	}
	el = head;
	for (int i = 0; i < n; el = el->next, i++)
	{
		el = head;
		while (el->next)
		{
			if (el->data.ID > el->next->data.ID)
			{
				buffer = el->data;
				el->data = el->next->data;
				el->next->data = buffer;
			}
			el = el->next;
		}
	}
	return head;
}

record* sortByName(record* el) // Сортировка по алфавиту
{
	info buffer = el->data;
	record* head = el;
	int n = 0;
	while (el->next)
	{
		n++;
		el = el->next;
	}
	el = head;
	for (int i = 0; i < n; el = el->next, i++)
	{
		el = head;
		while (el->next)
		{
			if (strcmp(el->data.studName, el->next->data.studName) > 0)
			{
				buffer = el->data;
				el->data = el->next->data;
				el->next->data = buffer;
			}
			el = el->next;
		}
	}
	return head;
}

record* sortByCode(record* el) // Сортировка по шифру группы
{
	info buffer = el->data;
	record* head = el;
	int n = 0;
	while (el->next)
	{
		n++;
		el = el->next;
	}
	el = head;
	for (int i = 0; i < n; el = el->next, i++)
	{
		el = head;
		while (el->next)
		{
			if (el->data.groupCode > el->next->data.groupCode)
			{
				buffer = el->data;
				el->data = el->next->data;
				el->next->data = buffer;
			}
			el = el->next;
		}
	}
	return head;
}

record* sortByYear(record* el) // Соритровка по году поступления
{
	info buffer = el->data;
	record* head = el;
	int n = 0;
	while (el->next)
	{
		n++;
		el = el->next;
	}
	el = head;
	for (int i = 0; i < n; el = el->next, i++)
	{
		el = head;
		while (el->next)
		{
			if (el->data.admYear > el->next->data.admYear)
			{
				buffer = el->data;
				el->data = el->next->data;
				el->next->data = buffer;
			}
			el = el->next;
		}
	}
	return head;
}

record* sortByScores(record* el) // Сортировка по вступительному баллу
{
	info buffer = el->data;
	record* head = el;
	int n = 0;
	while (el->next)
	{
		n++;
		el = el->next;
	}
	el = head;
	for (int i = 0; i < n; el = el->next, i++)
	{
		el = head;
		while (el->next)
		{
			if (el->data.score < el->next->data.score)
			{
				buffer = el->data;
				el->data = el->next->data;
				el->next->data = buffer;
			}
			el = el->next;
		}
	}
	return head;
}

record* sortMain(record* el, int* sortType)
{
	orgSubMenu(2);
	record* head;
	int initKey = 0;
	int menuKey = 0;
	while (1)
	{
		system("cls");
		std::cout << "Sort by:\n";
		s_showMenu();
		initKey = _getch();
		if (initKey == 224)
		{
			menuKey = _getch();
			if (menuKey == 72)
			{
				s_scrollUp();
			}
			if (menuKey == 80)
			{
				s_scrollDown();
			}
		}
		if (initKey == 13)
		{
			switch (s_currentPos->s_itemPos)
			{
			case 1: // По ID
			{
				*sortType = 0;
				head = sortByID(el);
				std::cout << "\nSorted by ID\n";
				return head;
			}
			case 2: // По имени
			{
				*sortType = 1;
				head = sortByName(el);
				std::cout << "\nSorted by name\n";
				return head;
			}
			case 3: // По шифру
			{
				*sortType = 2;
				head = sortByCode(el);
				std::cout << "\nSorted by group code\n";
				return head;
			}
			case 4: // По году
			{
				*sortType = 3;
				head = sortByYear(el);
				std::cout << "\nSorted by admission year\n";
				return head;
			}
			case 5: // По баллу
			{
				*sortType = 4;
				head = sortByScores(el);
				std::cout << "\nSorted by scores\n";
				return head;
			}
			case 6: // Отмена
				return el;
			}
		}
		if ((initKey == 8) || (initKey == 27))
		{
			return el;
		}
	}
}

// 7. Поиск записи в таблице по ключевому полю.
void searchByID(record* el) // Поиск по ID
{
	system("cls");
	record* head = el;
	int searchID;
	bool exists = false;
	while (1)
	{
		std::cout << "Enter the element ID to search: ";
		std::cin >> searchID;
		if (!std::cin)
		{
			std::cout << "Invalid input\n";
			std::cin.clear();
			std::cin.ignore(255, '\n');
			continue;
		}
		else
		{
			break;
		}
	}
	while (el)
	{
		if (el->data.ID == searchID)
		{
			exists = true;
		}
		el = el->next;
	}
	if (!exists)
	{
		std::cout << "\nNo such element\n";
		system("pause");
		return;
	}
	el = head;
	while (el->data.ID != searchID)
	{
		el = el->next;
	}
	std::cout << '\n';
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	std::cout << "| ID | Record book number |               Name               | Group code | Birth date | Admission year | Score |\n";
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	std::cout << "| " << setw(2) << el->data.ID << ' '
		<< "| " << setw(18) << el->data.recBookNo << ' '
		<< "| " << setw(32) << el->data.studName << ' '
		<< "| " << setw(10) << el->data.groupCode << ' '
		<< "| " << setw(2) << el->data.bdate[0] << '.'
		<< setw(2) << el->data.bdate[1] << '.'
		<< el->data.bdate[2] << ' '
		<< "| " << setw(14) << el->data.admYear << ' '
		<< "| " << setw(5) << el->data.score << " |\n";
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
}

void searchByRecBookNo(record* el) // Поиск по номеру зачётной книжки
{
	system("cls");
	record* head = el;
	int searchNum = 0;
	bool exists = false;
	while (1)
	{
		std::cout << "Enter record book number to search: ";
		std::cin >> searchNum;
		if (!std::cin)
		{
			std::cout << "Invalid input\n";
			std::cin.clear();
			std::cin.ignore(255, '\n');
			continue;
		}
		else
		{
			break;
		}
	}
	while (el)
	{
		if (el->data.recBookNo == searchNum)
			exists = true;
		el = el->next;
	}
	if (!exists)
	{
		std::cout << "\nNo such element\n";
		system("pause");
		return;
	}
	el = head;
	std::cout << '\n';
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	std::cout << "| ID | Record book number |               Name               | Group code | Birth date | Admission year | Score |\n";
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	while (el)
	{
		if (el->data.recBookNo == searchNum)
		{
			std::cout << "| " << setw(2) << el->data.ID << ' '
				<< "| " << setw(18) << el->data.recBookNo << ' '
				<< "| " << setw(32) << el->data.studName << ' '
				<< "| " << setw(10) << el->data.groupCode << ' '
				<< "| " << setw(2) << el->data.bdate[0] << '.'
				<< setw(2) << el->data.bdate[1] << '.'
				<< el->data.bdate[2] << ' '
				<< "| " << setw(14) << el->data.admYear << ' '
				<< "| " << setw(5) << el->data.score << " |\n";
			std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		}
		el = el->next;
	}
}

void searchByName(record* el) // Поиск по Ф. И. О.
{
	system("cls");
	char searchName[100];
	std::cout << "Enter name to search: ";
	std::cin.clear();
	std::cin.ignore();
	gets_s(searchName);
	std::cout << '\n';
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	std::cout << "| ID | Record book number |               Name               | Group code | Birth date | Admission year | Score |\n";
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	while (el)
	{
		if (strstr(el->data.studName, searchName))
		{
			std::cout << "| " << setw(2) << el->data.ID << ' '
				<< "| " << setw(18) << el->data.recBookNo << ' '
				<< "| " << setw(32) << el->data.studName << ' '
				<< "| " << setw(10) << el->data.groupCode << ' '
				<< "| " << setw(2) << el->data.bdate[0] << '.'
				<< setw(2) << el->data.bdate[1] << '.'
				<< el->data.bdate[2] << ' '
				<< "| " << setw(14) << el->data.admYear << ' '
				<< "| " << setw(5) << el->data.score << " |\n";
			std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		}
		el = el->next;
	}
}

void searchByGroupCode(record* el) // Поиск по шифру группы
{
	system("cls");
	record* head = el;
	int searchCode = 0;
	bool exists = false;
	while (1)
	{
		std::cout << "Enter group code to search: ";
		std::cin >> searchCode;
		if (!std::cin)
		{
			std::cout << "Invalid input\n";
			std::cin.clear();
			std::cin.ignore(255, '\n');
			continue;
		}
		else
		{
			break;
		}
	}
	while (el)
	{
		if (el->data.groupCode == searchCode)
			exists = true;
		el = el->next;
	}
	if (!exists)
	{
		std::cout << "\nNo such element\n";
		system("pause");
		return;
	}
	el = head;
	std::cout << '\n';
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	std::cout << "| ID | Record book number |               Name               | Group code | Birth date | Admission year | Score |\n";
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	while (el)
	{
		if (el->data.groupCode == searchCode)
		{
			std::cout << "| " << setw(2) << el->data.ID << ' '
				<< "| " << setw(18) << el->data.recBookNo << ' '
				<< "| " << setw(32) << el->data.studName << ' '
				<< "| " << setw(10) << el->data.groupCode << ' '
				<< "| " << setw(2) << el->data.bdate[0] << '.'
				<< setw(2) << el->data.bdate[1] << '.'
				<< el->data.bdate[2] << ' '
				<< "| " << setw(14) << el->data.admYear << ' '
				<< "| " << setw(5) << el->data.score << " |\n";
			std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		}
		el = el->next;
	}
}

void searchByAdmYear(record* el) // Поиск по году поступления
{
	system("cls");
	record* head = el;
	int searchYear = 0;
	bool exists = false;
	while (1)
	{
		std::cout << "Enter admission year to search: ";
		std::cin >> searchYear;
		if (!std::cin)
		{
			std::cout << "Invalid input\n";
			std::cin.clear();
			std::cin.ignore(255, '\n');
			continue;
		}
		else
		{
			break;
		}
	}
	while (el)
	{
		if (el->data.admYear == searchYear)
			exists = true;
		el = el->next;
	}
	if (!exists)
	{
		std::cout << "\nNo such element\n";
		system("pause");
		return;
	}
	el = head;
	std::cout << '\n';
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	std::cout << "| ID | Record book number |               Name               | Group code | Birth date | Admission year | Score |\n";
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	while (el)
	{
		if (el->data.admYear == searchYear)
		{
			std::cout << "| " << setw(2) << el->data.ID << ' '
				<< "| " << setw(18) << el->data.recBookNo << ' '
				<< "| " << setw(32) << el->data.studName << ' '
				<< "| " << setw(10) << el->data.groupCode << ' '
				<< "| " << setw(2) << el->data.bdate[0] << '.'
				<< setw(2) << el->data.bdate[1] << '.'
				<< el->data.bdate[2] << ' '
				<< "| " << setw(14) << el->data.admYear << ' '
				<< "| " << setw(5) << el->data.score << " |\n";
			std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		}
		el = el->next;
	}
}

void searchByScore(record* el) // Поиск по вступительному баллу
{
	system("cls");
	record* head = el;
	int searchScore = 0;
	bool exists = false;
	while (1)
	{
		std::cout << "Enter score to search: ";
		std::cin >> searchScore;
		if (!std::cin)
		{
			std::cout << "Invalid input\n";
			std::cin.clear();
			std::cin.ignore(255, '\n');
			continue;
		}
		else
		{
			break;
		}
	}
	while (el)
	{
		if (el->data.score == searchScore)
			exists = true;
		el = el->next;
	}
	if (!exists)
	{
		std::cout << "\nNo such element\n";
		system("pause");
		return;
	}
	el = head;
	std::cout << '\n';
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	std::cout << "| ID | Record book number |               Name               | Group code | Birth date | Admission year | Score |\n";
	std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	while (el)
	{
		if (el->data.score == searchScore)
		{
			std::cout << "| " << setw(2) << el->data.ID << ' '
				<< "| " << setw(18) << el->data.recBookNo << ' '
				<< "| " << setw(32) << el->data.studName << ' '
				<< "| " << setw(10) << el->data.groupCode << ' '
				<< "| " << setw(2) << el->data.bdate[0] << '.'
				<< setw(2) << el->data.bdate[1] << '.'
				<< el->data.bdate[2] << ' '
				<< "| " << setw(14) << el->data.admYear << ' '
				<< "| " << setw(5) << el->data.score << " |\n";
			std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		}
		el = el->next;
	}
}

void searchMain(record* el)
{
	if ((!el->prev) && (!el->next))
	{
		std::cout << "Only one element\n";
		std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		std::cout << "| ID | Record book number |               Name               | Group code | Birth date | Admission year | Score |\n";
		std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		std::cout << "| " << setw(2) << el->data.ID << ' '
			<< "| " << setw(18) << el->data.recBookNo << ' '
			<< "| " << setw(32) << el->data.studName << ' '
			<< "| " << setw(10) << el->data.groupCode << ' '
			<< "| " << setw(2) << el->data.bdate[0] << '.'
			<< setw(2) << el->data.bdate[1] << '.'
			<< el->data.bdate[2] << ' '
			<< "| " << setw(14) << el->data.admYear << ' '
			<< "| " << setw(5) << el->data.score << " |\n";
		std::cout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
		return;
	}
	orgSubMenu(3);
	int initKey = 0;
	int menuKey = 0;
	while (1)
	{
		system("cls");
		std::cout << "Search by:\n";
		s_showMenu();
		initKey = _getch();
		if (initKey == 224)
		{
			menuKey = _getch();
			if (menuKey == 72)
				s_scrollUp();
			if (menuKey == 80)
				s_scrollDown();
		}
		if (initKey == 13)
		{
			switch (s_currentPos->s_itemPos)
			{
			case 1: // По ID
			{
				searchByID(el);
				system("pause");
				break;
			}
			case 2: // По номеру зачётной книжки
			{
				searchByRecBookNo(el);
				system("pause");
				break;
			}
			case 3: // По Ф. И. О.
			{
				searchByName(el);
				system("pause");
				break;
			}
			case 4: // По шифру группы
			{
				searchByGroupCode(el);
				system("pause");
				break;
			}
			case 5: // Году поступления
			{
				searchByAdmYear(el);
				system("pause");
				break;
			}
			case 6: // По вступительному баллу
			{
				searchByScore(el);
				system("pause");
				break;
			}
			case 7: // Отмена
				return;
			}
		}
		if ((initKey == 8) || (initKey == 27))
		{
			return;
		}
	}
}

/* 8. Сохранение таблицы в текстовом файле. Имя файла должен вводить пользователь.
Сохранение таблицы в текстовом файле обеспечит при необходимости возможность её печати. */
void saveTFile(record* el)
{
	ofstream fout;
	string filePath;
	std::cout << "\nEnter file path to save: \n\n";
	std::cin >> filePath;
	fout.open(filePath);
	fout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	fout << "| ID | Record book number |               Name               | Group code | Birth date | Admission year | Score |\n";
	fout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+\n";
	while (1)
	{
		fout << "| " << setw(2) << el->data.ID << ' '
			<< "| " << setw(18) << el->data.recBookNo << ' '
			<< "| " << setw(32) << el->data.studName << ' '
			<< "| " << setw(10) << el->data.groupCode << ' '
			<< "| " << setw(2) << el->data.bdate[0] << '.'
			<< setw(2) << el->data.bdate[1] << '.'
			<< el->data.bdate[2] << ' '
			<< "| " << setw(14) << el->data.admYear << ' '
			<< "| " << setw(5) << el->data.score << " |\n";
		fout << "+----+--------------------+----------------------------------+------------+------------+----------------+-------+";
		if (!el->next)
		{
			break;
		}
		fout << '\n';
		el = el->next;
	}
	fout.close();
	std::cout << "\nData saved to '" << filePath << "' file\n";
}

/* 9. Обработка таблицы и просмотр результатов обработки.
Результат обработки необходимо вывести на экран и в текстовый файл. Имя файла вводит пользователь.
Вывести фамилии 5 лучших студентов по годам поступления */
void showBestStudents(record* el)
{
	el = sortByScores(el);
	system("cls");
	record* head = el;
	record* temp = el;
	int tempAdmYear;
	int i = 0;
	int n = 0;
	bool used = false;
	int studCnt = 0;
	while (el)
	{
		el = el->next;
		n++;
	}
	int* usedBuf = new int[n];
	usedBuf[0] = 0;
	el = head;
	std::cout << "+------+----------------------------------+-------+\n";
	std::cout << "| Year |               Name               | Score |\n";
	while (el)
	{
		tempAdmYear = el->data.admYear;
		for (int j = 0; j <= i; j++)
			if (tempAdmYear == usedBuf[j])
			{
				used = true;
			}
		if (!used)
		{
			std::cout << "+------+----------------------------------+-------+\n";
			std::cout << "| " << setw(4) << tempAdmYear << " |" << setw(44) << "|\n";
			std::cout << "|      +----------------------------------+-------+\n";
			while (temp)
			{
				if ((tempAdmYear == temp->data.admYear) && (studCnt != 5))
				{
					std::cout << "|      |" << setw(33) << temp->data.studName << ' '
						<< "| " << setw(5) << temp->data.score << " |\n";
					studCnt++;
				}
				temp = temp->next;
			}
		}
		studCnt = 0;
		temp = head;
		usedBuf[i] = tempAdmYear;
		i++;
		el = el->next;
		used = false;
	}
	std::cout << "+------+----------------------------------+-------+\n";
	delete[]usedBuf;
}

record* sortBack(record* stud, int sortType) // Возврат к порядку сортировки до выполнения функции
{
	switch (sortType)
	{
	case 0:
		stud = sortByID(stud);
		break;
	case 1:
		stud = sortByName(stud);
		break;
	case 2:
		stud = sortByCode(stud);
		break;
	case 3:
		stud = sortByYear(stud);
		break;
	case 4:
		stud = sortByScores(stud);
		break;
	}
	return stud;
}

/* 10. Выход - завершение работы программы.
В процессе работы с одной и той же таблицей она должна автоматически сохраняться в типизированном файле. */
void saveBFile(record* el) // Сохранение в бинарный файл
{
	FILE* fout;
	fopen_s(&fout, "binary.txt", "wb");
	if (!fout)
	{
		std::cout << "\nUnable to open\n";
		return;
	}
	while (el)
	{
		fwrite(&el->data, sizeof(el->data), 1, fout);
		el = el->next;
	}
	fclose(fout);
}

void clearData(record* el) // Освобождение памяти
{
	record* temp;
	if (el)
	{
		while (el->next)
		{
			temp = el;
			el = el->next;
			delete temp;
		}
		delete el;
		el = temp = NULL;
	}
}

void quit(record* el) // Выход
{
	clearMenu();
	s_clearMenu();
	saveBFile(el); // Сохранение в бинарном файле
	clearData(el); // Освобождение памяти
}

void orgSubMenu(int call)
{
	s_clearMenu();
	s_itemNo = 0;
	switch (call)
	{
	case 1: // Редактирование
	{
		s_addMenuItem("record book number", 1);
		s_addMenuItem("name", 0);
		s_addMenuItem("group code", 0);
		s_addMenuItem("birth date", 0);
		s_addMenuItem("admission year", 0);
		s_addMenuItem("score\n", 0);
		s_addMenuItem("Back", 0);
		break;
	}
	case 2: // Сортировка
	{
		s_addMenuItem("ID", 1);
		s_addMenuItem("name", 0);
		s_addMenuItem("group code", 0);
		s_addMenuItem("admission year", 0);
		s_addMenuItem("score\n", 0);
		s_addMenuItem("Cancel", 0);
		break;
	}
	case 3: // Поиск
	{
		s_addMenuItem("ID", 1);
		s_addMenuItem("record book number", 0);
		s_addMenuItem("name", 0);
		s_addMenuItem("group code", 0);
		s_addMenuItem("admission year", 0);
		s_addMenuItem("score\n", 0);
		s_addMenuItem("Cancel", 0);
		break;
	}
	}
}

int main()
{
	record* stud = NULL;
	int initKey, menuKey;
	int sortType = 0;
	addMenuItem("Load", true);                // 1
	addMenuItem("Create blank table", false); // 2
	addMenuItem("Show", false);               // 3
	addMenuItem("Add", false);                // 4
	addMenuItem("Delete", false);             // 5
	addMenuItem("Edit", false);               // 6
	addMenuItem("Sort", false);               // 7
	addMenuItem("Search", false);             // 8
	addMenuItem("Save", false);               // 9
	addMenuItem("Show best students", false); // 10
	addMenuItem("Quit", false);               // 11
	while (1)
	{
		system("cls");
		showMenu();
		initKey = _getch();
		if (initKey == 224)
		{
			menuKey = _getch();
			if (menuKey == 72)
			{
				scrollUp();
			}
			if (menuKey == 80)
			{
				scrollDown();
			}
		}
		if (initKey == 13)
			switch (currentPos->itemPos)
			{
			case 1: // Загрузка
			{
				stud = loadBFile();
				system("pause");
				break;
			}
			case 2: // Создание
			{
				stud = createTFile();
				system("pause");
				break;
			}
			case 3: // Просмотр
			{
				if (stud)
				{
					showTable(stud, 0, 0);
				}
				else
				{
					std::cout << "\nNo file opened or no data\n";
					system("pause");
				}
				break;
			}
			case 4: // Добавление
			{
				stud = addItem(stud);
				sortBack(stud, sortType);
				system("pause");
				break;
			}
			case 5: // Удаление
			{
				if (stud)
				{
					stud = delItem(stud);
					sortBack(stud, sortType);
				}
				else
					std::cout << "\nNo file opened or no data\n";
				system("pause");
				break;
			}
			case 6: // Редактирование
			{
				if (stud)
				{
					stud = editItem(stud);
					sortBack(stud, sortType);
				}
				else
				{
					std::cout << "\nNo file opened or no data\n";
					system("pause");
				}
				break;
			}
			case 7: // Сортировка
			{
				if (stud)
				{
					stud = sortMain(stud, &sortType);
				}
				else
					std::cout << "\nNo file opened or no data\n";
				system("pause");
				break;
			}
			case 8: // Поиск
			{
				if (stud)
				{
					searchMain(stud);
				}
				else
				{
					std::cout << "\nNo file opened or no data\n";
					system("pause");
				}
				break;
			}
			case 9: // Сохранение
			{
				if (stud)
				{
					saveTFile(stud);
				}
				else
				{
					std::cout << "\nNo file opened or no data\n";
				}
				system("pause");
				break;
			}
			case 10: // Лучшие студенты
			{
				if (stud)
				{
					showBestStudents(stud);
					sortBack(stud, sortType);
				}
				else
					std::cout << "\nNo file opened or no data\n";
				system("pause");
				break;
			}
			case 11: // Выход
			{
				quit(stud);
				return 0;
			}
			}
	}
}
