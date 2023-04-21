#include <iostream>
#include <fstream>
using namespace std;
bool is_created;

struct point
{
	string key;
	point* next;
};

point* first;

point* make_list()
{
	int n;
	cout << "Input the size of a list" << endl;
	cin >> n;
	cout << "key - ?" << endl;
	string s;
	cin >> s;

	point* p, * r, * beg;
	p = new (point);
	p->key = s;
	p->next = nullptr;
	beg = p;

	for (int i = 1; i < n; i++) //добавить элементы в конец списка
	{
		r = new(point);
		cout << "key - ?" << endl;
		string s;
		cin >> s;

		r->key = s;
		p->next = r;
		r->next = nullptr;
		p = r;
	}
	return beg;
}

void print_list(point* beg)
{
	if (beg == 0) //если список пустой
	{
		cout << "The list is empty\n" << endl;
		return;
	}
	point* p = beg;
	while (p != 0) //пока не конец списка
	{
		cout << p->key << " \t ";
		p = p->next; //перейти на следующий элемент
	}
	cout << "\n";
}

point* del_point(point* beg, int k)
{
	k--; //перевод из номера в индекс
	point* p = beg;
	if (k == 0) {
		beg = beg->next;
		if (beg == nullptr) return nullptr;
		delete p;
		return beg;
	}
	for (int i = 0; i < k - 1 && p != nullptr; i++, p = p->next); // бежим по элементам k-1 раз (встаём на предыдущий нужному)
	if (p == nullptr || p->next == nullptr) return beg;
	point* r = p->next; //встать на удаляемый элемент
	p->next = r->next; //изменить ссылку
	delete r;
	r = p->next; //встать на следующий
	return beg;
}

point* add_point(point* beg, int k)
{
	k--; //перевод из номера в индекс
	point* p;
	p = new(point);
	cout << "key - ?" << endl;
	string s;
	cin >> s;
	p->key = s;
	if (k == 0)
	{
		p->next = beg; //добавить перед beg
		//связать список с добавленным элементом
		beg = p; //запомнить первый элемент в beg
		return beg;
	}
	point* r = beg; //встать на начало списка
	for (int i = 0; i < k - 1 && r->next != nullptr; i++, r = r->next);
	p->next = r->next; //связать р с концом списка
	//если элемент не последний, то связать конец списка с р
	if (r->next == nullptr) p->next = nullptr;
	r->next = p;
	return beg;
}

int find_key_index(point* beg) {
	cout << "Input the key word" << endl;
	string key_word;
	cin >> key_word;
	point* r = beg;
	int counter = 1;
	while (r) {
		if (r->key == key_word) return counter;
		r = r->next;
		counter++;
	}
	cout << "not found" << endl;
	return NULL;
}
point* delete_list(point* beg) {
	point* p = beg;
	int counter = 1;
	while (p->next != nullptr) {
		p = p->next;
		counter++;
	}
	for (int i = counter; i > 0; i--) {
		beg = del_point(beg, i);
	}
	return beg;
}

void printINfile(point* beg, string path) {
	ofstream fout;
	fout.open(path);
	if (!fout.is_open()) { cout << "Error" << endl; }
	else {
		cout << "Information has been copied" << endl;
		point* p = beg;
		while (p != nullptr) {
			fout << p->key;
			fout << "\n";
			p = p->next;
		}
	}
	fout.close();
}

point* readFROMfile(string path) {
	ifstream fin;
	fin.open(path);
	point* beg = new(point);
	if (!fin.is_open()) { cout << "Error" << endl; }
	else {
		cout << "Information has been restored" << endl;
		string word;
		point* p, * r;
		p = new (point);
		fin >> word;
		p->key = word;
		p->next = nullptr;
		beg = p;

		while (!fin.eof()) { //eof = end of file
			word = "";
			fin >> word;
			r = new(point);
			r->key = word;
			p->next = r;
			r->next = nullptr;
			p = r;
		}
	}
	fin.close();
	return beg;
}

void menu(const int c) {
	string path = "myFile.txt";

	if ((c == 1 || c == 7) && is_created) { // для случая, когда список уже создан
		cout << "List is already existing" << endl;
		return;
	}
	else if (c > 1 && c < 7 && !is_created) { // для случая, когда список еще не создан
		cout << "Create or restore a list" << endl;
		return;
	}
	switch (c) {
	case 1: {
		is_created = true;
		first = make_list();
		break;
	}
	case 2: {
		print_list(first);
		int num = find_key_index(first);
		int k;
		cout << "Input K" << endl;
		cin >> k;
		for (int i = 0; i < k; i++) {
			add_point(first, num + i);
		}
		num += k;
		for (int i = 0; i < k; i++) {
			add_point(first, num + i + 1);
		}
		print_list(first);
		break;
	}
	case 3: {
		print_list(first);
		int n;
		cout << "Position of an element you want to delete?" << endl;
		cin >> n;
		del_point(first, n);
		print_list(first);
		break;
	}
	case 4: {
		print_list(first);
		break;
	}
	case 5: {
		printINfile(first, path);
		break;
	}
	case 6: {
		cout << "List was destroyed" << endl;
		delete_list(first);
		is_created = false;
		break;
	}

	case 7: {
		first = readFROMfile(path);
		if (first == nullptr) cout << "There's nothing to restore" << endl;
		else is_created = true;
		break;
	}
	default: {
		cout << "Exiting from menu" << endl;
		delete_list(first);
		exit(777);
	}
	}
}

int main() {
	while (true) {
		system("pause");
		system("cls");
		cout << "1. Create a list" << endl;
		cout << "2. Add K elements before and after an element with key word" << endl;
		cout << "3. Delete an element" << endl;
		cout << "4. Print list" << endl;
		cout << "5. Write list in a file" << endl;
		cout << "6. Delete list" << endl;
		cout << "7. Read from file" << endl;
		cout << "Choose your option: ";
		int c;
		cin >> c;
		system("cls");
		menu(c);
	}
	return 0;
}
