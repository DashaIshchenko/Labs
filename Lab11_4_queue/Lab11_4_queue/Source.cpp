#include <iostream>
#include <fstream>
using namespace std;
bool is_created;
struct point{
	string key;
	point* prev, * next;
};

struct Queue {
	point* head, * tail;
	int size;
};

Queue *qwe;

Queue* make_queue(){
	int n;
	string s;
	Queue* qu = new(Queue);
	point* p, * r;
	cout << "Input the size of a queue" << endl;
	cin >> n;
	qu->size = n;
	cout << "key - ?" << endl;
	cin >> s;
	p = new (point);
	p->key = s;
	p->prev = nullptr;
	p->next = nullptr;
	qu->head = p;

	for (int i = 1; i < n; i++) {//добавить элементы в конец списка
		r = new(point);
		cout << "key - ?" << endl;
		cin >> s;
		r->key = s;
		p->next = r;
		r->prev = p;
		r->next = nullptr;
		qu->tail = r;
		p = r;
	}
	return qu;
}

void print_queue(Queue*qu){
	if (qu->head == nullptr) {//если список пустой
		cout << "The queue is empty" << endl;
		return;
	}
	point* p = qu->head;
	for (int i = 0; i < qu->size - 1; i++) {
		cout << p->key << " > ";
		p = p->next;
	}
	cout <<p->key<< "\n";
}

int find_key_index(Queue*qu) {
	cout << "Input the key word" << endl;
	string key_word;
	cin >> key_word;
	point* r = qu->head;
	int counter = 1;
	while (r) {
		if (r->key == key_word) return counter;
		r = r->next;
		counter++;
	}
	cout << "not found" << endl;
	return NULL;
}

void push(Queue*qu, string s) {
	point* p;
	p = new(point);
	p->key = s;
	qu->tail->next = p;
	p->prev = qu->tail;
	qu->tail = p;
	qu->size++;
	return;
}

void pop(Queue*qu){
	if (qu->head == nullptr) return;
	point* r = qu->head;
	qu->head = r->next;
	delete r;
	qu->size--;
	return;
}

void delete_queue(Queue*qu) {
	for (int i = 0; i < qu->size; i++) pop(qu);
	delete qu;
}

void printINfile(Queue*qu, string path) {
	ofstream fout;
	fout.open(path);
	if (!fout.is_open()) cout << "Error" << endl;
	else {
		cout << "Information has been copied" << endl;
		point* p = qu->head;
		for (int i = 0; i < qu->size-1; i++) {
			fout << p->key;
			fout << "\n";
			p = p->next;
		}
		fout << p->key;
	}
	fout.close();
}

Queue* readFROMfile(string path) {
	int n = 1;
	ifstream fin;
	fin.open(path);
	Queue* qu = new(Queue);
	if (!fin.is_open()) cout << "Error" << endl;
	else {
		string word;
		point* p, * r;
		p = new (point);
		fin >> word;
		p->key = word;
		p->prev = nullptr;
		p->next = nullptr;
		qu->head = p;

		while (!fin.eof()) { //eof = end of file
			word = "";
			fin >> word;
			r = new(point);
			r->key = word;
			r->prev = p;
			p->next = r;
			r->next = nullptr;
			qu->tail = r;
			p = r;
			n++;
		}
	}
	qu->size = n;
	fin.close();
	return qu;
}

void menu(const int c) {
	string path = "myFile.txt";

	if ((c == 1 || c == 7) && is_created) { // для случая, когда список уже создан
		cout << "Queue is already existing" << endl;
		return;
	}
	else if (c > 1 && c < 7 && !is_created) { // для случая, когда список еще не создан
		cout << "Create or restore a queue" << endl;
		return;
	}
	switch (c) {
	case 1: {
		is_created = true;
		qwe = make_queue();
		break;
	}
	case 2: {
		print_queue(qwe);
		string s;
		cout << "key - ?" << endl;
		cin >> s;
		push(qwe, s);
		print_queue(qwe);
		break;
	}
	case 3: {
		print_queue(qwe);
		pop(qwe);
		if (qwe->head == nullptr) is_created = false;
		print_queue(qwe);
		break;
	}
	case 4: {
		print_queue(qwe);
		break;
	}
	case 5: {
		printINfile(qwe, path);
		break;
	}
	case 6: {
		cout << "Queue was destroyed" << endl;
		delete_queue(qwe);
		is_created = false;
		break;
	}

	case 7: {
		qwe = readFROMfile(path);
		if (qwe->head->key == "") {
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
		print_queue(qwe);
		int num = find_key_index(qwe), k=0;
		string s;
		if (num) {
			for (int i = 1; i < num; i++) {
				push(qwe, qwe->head->key);
				pop(qwe);
			}
			cout << "Enter the K" << endl;
			cin >> k;
			for (int i = 0; i < k; i++) {
				cout << "key - ?" << endl;
				cin >> s;
				push(qwe, s);
			}
			push(qwe, qwe->head->key);
			pop(qwe);
			for (int i = 0; i < k; i++) {
				cout << "key - ?" << endl;
				cin >> s;
				push(qwe, s);
			}
		}
		for (int i = 0; i < (qwe->size) - (num + 2 * k); i++) {
			push(qwe, qwe->head->key);
			pop(qwe);
		}
		print_queue(qwe);
		break;
	}
	default: {
		cout << "Exiting from menu" << endl;
		delete_queue(qwe);
		exit(777);
	}
	}
}

int main() {

	while (true) {
		system("pause");
		system("cls");
		cout << "1. Create a queue" << endl;
		cout << "2. Push element" << endl;
		cout << "3. Pop element" << endl;
		cout << "4. Print queue" << endl;
		cout << "5. Write queue in a file" << endl;
		cout << "6. Delete queue" << endl;
		cout << "7. Read from file" << endl;
		cout << "8. Add K elements before and after key element" << endl;
		cout << "Choose your option: ";
		int c;
		cin >> c;
		system("cls");
		menu(c);
	}
	return 0;
}