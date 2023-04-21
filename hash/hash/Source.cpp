#include <iostream>
#include <string>
#include <fstream>
using namespace std;

bool is_created = false;

struct Human {
	string name = "NULL";
	string date = "NULL";
	string passport = "NULL";
	Human* next = nullptr;

	void SetHuman(string name, string date, string passport){
		this->name = name;
		this->date = date;
		this->passport = passport;
	}
	void OutputHuman() {
		cout << "Name:\t" << this->name << endl;
		cout << "Date:\t" << this->date << endl;
		cout << "ID:\t" << this->passport << endl;
	}
	void CopyHuman(Human*copy){
		this->name = copy->name;
		this->date = copy->date;
		this->passport = copy->passport;
		this->next = nullptr;
	}
};

struct list {
	Human* first = nullptr;
	int size = 0;
};


list* make_list(int n){
	string name, date, passport;
	getline(cin, name);
	cout << "Enter name: ";
	getline(cin, name);
	cout << "Enter date: ";
	getline(cin, date);
	cout << "Enter ID: ";
	getline(cin, passport);
	Human* p = new Human, *r;
	p->SetHuman(name, date, passport);
	p->next = nullptr;
	list* l = new list;
	l->first = p;
	l->size=n;

	for (int i = 1; i < n; i++) {
		r = new(Human);
		getline(cin, name);
		cout << "Enter name: ";
		getline(cin, name);
		cout << "Enter date: ";
		getline(cin, date);
		cout << "Enter ID: ";
		getline(cin, passport);
		r->SetHuman(name, date, passport);
		p->next = r;
		r->next = nullptr;
		p = r;
	}
	return l;
}


void add_point(list* l, int k) {
	k--;
	string name, date, passport;
	getline(cin, name);
	cout << "Enter name: ";
	getline(cin, name);
	cout << "Enter date : ";
	getline(cin, date);
	cout << "Enter ID : ";
	getline(cin, passport);

	Human* pep = new Human;
	pep->SetHuman(name, date, passport);

	if (k == 0) {
		pep->next = l->first;
		l->first = pep;
		l->size++;
		return;
	}

	Human* temp = l->first;
	for (int i = 0; i < k-1 && temp->next != nullptr; i++) temp = temp->next;
	pep->next = temp->next;
	if (temp->next == nullptr) pep->next = nullptr;
	temp->next = pep;
	l->size++;
	return;
}

void del_point(list* l, int k){
	k--;
	Human* p = l->first;
	if (k == 0) {
		l->first = l->first->next;
		if (l->first == nullptr) return;
		delete p;
		l->size--;
		return;
	}
	for (int i = 0; i < k-1 && p != nullptr; i++, p = p->next); // бежим по элементам k-1 раз (встаём на предыдущий нужному)
	if (p == nullptr || p->next == nullptr) return;
	Human* r = p->next; //встать на удаляемый элемент
	p->next = r->next; //изменить ссылку
	delete r;
	l->size--;
	r = p->next; //встать на следующий
	return;
}

void delete_list(list*l) {
	int size = l->size;
	for (int i = size; i > 0; i--) {
		del_point(l, i);
	}
}

void print_list(list* l){
	if (l->size==0) {
		cout << "The list is empty\n" << endl;
		return;
	}
	Human* pep = l->first;
	for (int i = 0; i < l->size; i++) {
		pep->OutputHuman();
		cout << "-------------" << endl;
		pep = pep->next;
	}
	cout << "\n";
}

void printINfile(list *l, string path) {
	ofstream fout;
	fout.open(path);
	if (!fout.is_open()) { cout << "Error" << endl; }
	else {
		cout << "Information has been copied" << endl;
		Human* p = l->first;
		for (int i = 0; i < l->size; i++) {
			fout << p->name << "\n";
			fout << p->date << "\n";
			fout << p->passport << "\n";
			p = p->next;
		}
	}
	fout.close();
}

list* readFROMfile(string path) {
	ifstream fin;
	fin.open(path);
	list* l = new(list);
	if (!fin.is_open()) { cout << "Error" << endl; }
	else {
		cout << "Information has been restored" << endl;
		string word1, word2, word3;
		Human* p, * r;
		p = new (Human);
		fin >> word1 >> word2 >> word3;
		p->SetHuman(word1, word2, word3);
		p->next = nullptr;
		l->first = p;
		l->size++;
		while (!fin.eof()) { //eof = end of file
			word1 = ""; word2 = ""; word3 = "";
			fin >> word1 >> word2 >> word3;
			r = new(Human);
			r->SetHuman(word1, word2, word3);
			p->next = r;
			r->next = nullptr;
			p = r;
			l->size++;
		}
	}
	fin.close();
	return l;
}

int hash_func(string pep, int size) {
	float num;
	num = stoi(pep.substr(0, 2)) + stoi(pep.substr(3, 5)) + stoi(pep.substr(6, 10));
	num *= 0.414141;
	num -= (int)num;
	num *= size;
	return (int)num;
}

Human** open_address_method(list* l) {
	int size = l->size, pos;
	Human* pep = l->first;
	Human** table = new Human*[size];
	for (int i = 0; i < size; i++) table[i] = new Human;
	
	for (int i = 0; i < size; i++) {
		pos = hash_func(pep->date, size);
		if (table[pos]->date == "NULL") { 
			table[pos] = pep; 
			pep = pep->next;
		}
		else {
			while (table[pos]->date != "NULL") {
				pos++;
				if (pos >= size) pos = pos - size;
			}
			table[pos] = pep;
			pep = pep->next;
		}
	}
	return table;
}

Human** chain_method(list* l) {
	int size = l->size, pos;
	Human* curr = l->first;
	Human* pep = new Human, * r;
	Human** table = new Human * [size];
	for (int i = 0; i < size; i++) table[i] = new Human;
	pep->CopyHuman(curr);
	for (int i = 0; i < size; i++) {
		pos = hash_func(curr->date, size);
		
		if (table[pos]->date == "NULL") {
			table[pos] = pep;
		}
		else {
			r = table[pos];
			pep->next = r;
			table[pos] = pep;
			
		}
		pep = new Human;
		if (curr->next != nullptr) {
			curr = curr->next;
			pep->CopyHuman(curr);
		}

	}
	return table;
}

void print_hash(Human** table, int size, int type) {
	if (type == 1) for (int i = 0; i < size; i++) {
		cout << i << ":\n";
		table[i]->OutputHuman();
		cout << "-----------" << endl;
	}
	else {
		for (int i = 0; i < size; i++) {
			cout << i << ":\n";
			Human* pep = table[i];
			do {
				pep->OutputHuman();
				pep = pep->next;
				cout << endl;
			}
			while (pep != nullptr);
			cout << "-----------" << endl;
		}
	}
}
list* people;
void menu(const int c) {
	string path = "myFile.txt";
	
	if ((c == 1 || c == 9) && is_created) { // для случая, когда список уже создан
		cout << "List is already existing" << endl;
		return;
	}
	else if (c > 1 && c < 9 && !is_created) { // для случая, когда список еще не создан
		cout << "Create or restore a list" << endl;
		return;
	}
	switch (c) {
	case 1: {
		is_created = true;
		int n;
		cout << "Input the size: ";
		cin >> n;
		people = make_list(n);
		break;
	}
	case 2: {
		print_list(people);
		int k;
		cout << "Input position" << endl;
		cin >> k;
		add_point(people, k);
		print_list(people);
		break;
	}
	case 3: {
		print_list(people);
		int n;
		cout << "Position of an element you want to delete?" << endl;
		cin >> n;
		del_point(people, n);
		print_list(people);
		break;
	}
	case 4: {
		print_list(people);
		break;
	}
	case 5: {
		printINfile(people, path);
		break;
	}
	case 6: {
		cout << "List was destroyed" << endl;
		delete_list(people);
		is_created = false;
		break;
	}
	case 7: {
		string date; int pos, counter=0;
		Human** table = open_address_method(people);
		print_hash(table, people->size, 1);
		cout << "Enter the date of birth: ";
		getline(cin, date); getline(cin, date);
		while (date.length() != 10) {
			cout << "Enter the date of birth: ";
			getline(cin, date);
		}
		pos = hash_func(date, people->size);
		if (table[pos]->date == date) {
			table[pos]->OutputHuman();
		}
		else {
			while (table[pos]->date != date && counter<people->size) {
				pos++; counter++;
				if (pos >= people->size) pos = pos - people->size;
			}
			if (counter < people->size) table[pos]->OutputHuman();
			else cout << "Not found" << endl;
		}
		break;
	}
	case 8: {
		string date; int pos; bool flag = false;
		Human** table = chain_method(people);
		print_hash(table, people->size, 2);
		cout << "Enter the date of birth: ";
		getline(cin, date); getline(cin, date);
		while (date.length() != 10) {
			cout << "Enter the date of birth: ";
			getline(cin, date);
		}
		pos = hash_func(date, people->size);
		Human* pep = table[pos];
		while (!flag && pep != nullptr) {
			if (pep->date == date) flag = true;
			else pep = pep->next;
		}
		if (flag) pep->OutputHuman();
		else cout << "Not found" << endl;
		break;
	}
	case 9: {
		people = new list;
		people = readFROMfile(path);
		if (people->first == nullptr) cout << "There's nothing to restore" << endl;
		else is_created = true;
		break;
	}
	
	default: {
		cout << "\nExiting from menu" << endl;
		delete_list(people);
		exit(777);
	}
	}
}

int main() {
	while (true) {
		system("pause");
		system("cls");
		cout << "1. Create a list" << endl;
		cout << "2. Add element on position" << endl;
		cout << "3. Delete an element" << endl;
		cout << "4. Print list" << endl;
		cout << "5. Write list in a file" << endl;
		cout << "6. Delete list" << endl;
		cout << "7. Open address method" << endl;
		cout << "8. Chain method" << endl;
		cout << "9. Read from file" << endl;
		
		cout << "Choose your option: ";
		int c;
		cin >> c;
		system("cls");
		menu(c);
	}
	return 0;
}
