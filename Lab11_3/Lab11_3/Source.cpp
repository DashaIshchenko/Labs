#include <iostream>
#include <fstream>
using namespace std;
bool is_created;

struct point {
	string key;
	point* prev = nullptr, * next = nullptr;
};

point* head;

struct Stack {
	point* head = nullptr;
	int size =0 ;
};

Stack* qwerty;

Stack *make_stack(){
	Stack* st = new(Stack);
	string s;
	int n = 0;
	point* p, * r;
	cout << "Input the size of a stack" << endl;
	cin >> n;
	st->size = n;
	cout << "key - ?" << endl;
	cin >> s;
	p = new (point);
	p->key = s;
	p->prev = nullptr;
	p->next = nullptr;
	st->head = p;

	for (int i = 1; i < n; i++) { //добавить элементы в конец списка
		r = new(point);
		cout << "key - ?" << endl;
		cin >> s;
		r->key = s;
		r->prev = p;
		p->next = r;
		r->next = nullptr;
		st->head = r;
		p = r;
	}
	return st;
}

void print_stack(Stack*st){
	if (st->head == nullptr) { //если список пустой
		cout << "The stack is empty" << endl;
		return;
	}
	point* p = st->head;
	//cout << st->size;
	for (int i = 0; i < st->size-1; i++) {
		cout << p->key << " < ";
		p = p->prev; //перейти на следующий элемент
	}
	cout << p->key <<"\n";
}

int find_key_index(Stack*st) {
	cout << "Input the key word" << endl;
	string key_word;
	cin >> key_word;
	point* r = st->head;
	int counter = 1;
	while (r) {
		if (r->key == key_word) return counter;
		r = r->prev;
		counter++;
	}
	cout << "not found" << endl;
	return NULL;
}

void push(Stack*st, string s) {
	point* p, *r = st->head;
	p = new(point);
	p->key = s;
	r->next = p;
	p->prev = r;
	st->head = p;
	st->size++;
	p->next = nullptr;
	return;
}

void pop(Stack*st){
	point* p = st->head;
	if (p == nullptr) return;
	point* r = p->prev;
	st->head = r;
	st->size--;
	delete p;
	return;
}

void delete_stack(Stack*st) {
	while (st->head) pop(st);
	delete st;
}

void printINfile(Stack*st, string path) {
	ofstream fout;
	fout.open(path);
	if (!fout.is_open()) cout << "Error" << endl;
	else {
		cout << "Information has been copied" << endl;
		point* p = st->head;
		while (p != nullptr) {
			fout << p->key;
			fout << "\n";
			p = p->prev;
		}
	}
	fout.close();
}

Stack* readFROMfile(string path) {
	int n = 0;
	ifstream fin;
	fin.open(path);
	Stack* st = new(Stack);
	if (!fin.is_open()) cout << "Error" << endl;
	else {
		string word;
		point* p, * r;
		p = new (point);
		fin >> word;
		p->key = word;
		p->prev = nullptr;
		p->next = nullptr;
		st->head = p;
		while (!fin.eof()) { //eof = end of file
			word = "";
			fin >> word;
			r = new(point);
			r->key = word;
			r->next = p;
			p->prev = r;
			r->prev = nullptr;
			p = r;		
			n++;
		}
	}
	st->size = n;
	fin.close();
	return st;
}

void AddElements(Stack* st, int count, int number) {
	Stack* new_stack = new (Stack);
	point* p = new (point);
	p->prev = nullptr;
	p->next = nullptr;
	new_stack->head = p;
	for (int i = 0; i < number; i++) {
		push(new_stack, st->head->key);
		pop(st);
	}
	string key;
	for (int j = 0; j < count; j++) {
		cout << "key - ? "<< endl;
		cin >> key;
		push(st, key);
	}
	push(st, new_stack->head->key);
	pop(new_stack);

	for (int j = 0; j < count; j++) {
		cout << "key - ? " << endl;
		cin >> key;
		push(st, key);
	}
	while (new_stack->size != 0) {
		push(st, new_stack->head->key);
		pop(new_stack);
	}
	delete new_stack;
	return;
}

void menu(const int c) {
	string path = "myFile.txt";

	if ((c == 1 || c == 7) && is_created) { // для случая, когда список уже создан
		cout << "Stack is already existing" << endl;
		return;
	}
	else if ((c > 1 && c < 7 || c==8)  && !is_created) { // для случая, когда список еще не создан
		cout << "Create or restore a stack" << endl;
		return;
	}
	switch (c) {
	case 1: {
		is_created = true;
		qwerty = make_stack();
		break;
	}
	case 2: {
		print_stack(qwerty);
		string s;
		cout << "key - ?" << endl;
		cin >> s;
		push(qwerty, s);
		print_stack(qwerty);
		break;
	}
	case 3: {
		print_stack(qwerty);
		pop(qwerty);
		if (qwerty->head == nullptr) is_created = false;
		print_stack(qwerty);
		break;
	}
	case 4: {
		print_stack(qwerty);
		break;
	}
	case 5: {
		printINfile(qwerty, path);
		break;
	}
	case 6: {
		cout << "Stack was destroyed" << endl;
		delete_stack(qwerty);
		is_created = false;
		break;
	}


	case 7: {
		qwerty = readFROMfile(path);
		if (qwerty->head->key == "") {
			cout << "There's nothing to restore" << endl;
			is_created = false;
		}
		else {
			is_created = true;
			cout << "Information has been restored" << endl;
		}
		break;
	}
	case 8: {
		print_stack(qwerty);
		int num = find_key_index(qwerty), k = 0;
		if (num != NULL) {
			string s;
			cout << num << " How many words before and after? " << endl;
			cin >> k;
			AddElements(qwerty, k, num);
			print_stack(qwerty);
		}
		break;
	}
	default: {
		cout << "Exiting from menu" << endl;
		delete_stack(qwerty);
		exit(777);
	}
	}
}

int main() {

	/*
	int n;
	cout << "Input the size of a stack" << endl;
	cin >> n;
	point* first = make_stack(n);
	print_stack(first);
	cout << "All the words after key word will be popped. ";
	int num = find_key_index(first);
	if (num) {
		cout << "The position of your key word is " << num << endl;
		for (int i = 0; i < n-num; i++) {
			first = pop(first);
			cout << i + 1 << ") ";
			print_stack(first);
		}
	}

	cout << "How many elements you want to push?" << endl;
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		first = push(first);
	}
	print_stack(first);

	string path = "myFile.txt";
	printINfile(first, path);
	first = delete_stack(first);
	print_stack(first);

	point* second = readFROMfile(path);
	print_stack(second);


	*/
	while (true) {
		system("pause");
		system("cls");
		cout << "1. Create a stack" << endl;
		cout << "2. Push element" << endl;
		cout << "3. Pop element" << endl;
		cout << "4. Print stack" << endl;
		cout << "5. Write stack in a file" << endl;
		cout << "6. Delete stack" << endl;
		cout << "7. Read from file" << endl;
		cout << "8. Add elements before and after key word" << endl;
		cout << "Choose your option: ";
		int c;
		cin >> c;
		system("cls");
		menu(c);
	}
	return 0;
}
