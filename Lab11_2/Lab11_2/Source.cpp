#include <iostream>
#include <fstream>
using namespace std;
bool is_created;

struct point 
{
	string key;
	point* prev, * next; 
};

point* first, * last;

void make_list(point*&first, point*&last)
{
	int n;
	cout << "Input the size of a list" << endl;
	cin >> n;
	cout << "key - ?" << endl;
	string s;
	cin >> s;

	point* p, * r;
	p = new (point); 
	p->key = s; //инициализация первого элемента в списке
	p->prev = nullptr; 
	p->next = nullptr; 
	first = p;
	last = p;

	for (int i = 1; i < n; i++) //добавить элементы в конец списка
	{
		r = new(point);  //создание нового элемента
		cout << "key - ?" << endl;
		cin >> s;
		
		r->key = s; 
		p->next = r; //для предыдущего элемента p следующий элемент - это r
		r->prev = p; //для нынешнего элемента r предыдущий элемент - это p
		r->next = nullptr; //для нынешнего элемента r следующий элемент - это ничего
		p = r; //в качестве предыдущего элемента берём нынешний элемент
		last = p;
	}
}

void print_list(point* beg)
{
	if (beg == nullptr) //если список пустой
	{
		cout << "The list is empty" << endl;
		return;
	}
	point* p = beg;
	while (p != nullptr) //пока не конец списка
	{
		cout << p->key << " \t ";
		p = p->next; //перейти на следующий элемент
	}
	cout << "\n";
}

void del_point(point* &beg, int k, point*&fin)
{
	k--;
	point* p = beg;
	if (k == 0) { //если удаляется первый элемент
		beg = beg->next; //началом становится следующий после первого
		if (beg == nullptr) { 
			// если после первого элемента ничего нет, то список пустой, не надо ничего удалять
			return;
		}
		beg->prev = nullptr;//уничтожаем предыдущий элемент (то есть первый)
		delete p;
		return;
	}
	for (int i = 0; i < k - 1 && p != nullptr; i++, p = p->next); //проходим по списку до нужного элемента
	if (p == nullptr || p->next == nullptr) return; //если по индексу такого элемента нет, то удалять ничего не требуется
	point* r = p->next; //встать на удаляемый элемент
	p->next = r->next; //изменить ссылку
	delete r;
	r = p->next; //встать на следующий
	//если r существует, то связать элементы
	if (r) r->prev = p;
	else fin = p;
}

void add_point(point*& beg, int k, point*&fin){
	point* p;
	string s;
	cout << "key - ?" << endl;
	cin >> s;
	k--;
	p = new(point);
	p->key = s;
	if (k == 0)
	{
		p->next = beg; //добавить перед beg
		p->prev = nullptr; //обнулить адрес предыдущего
		//связать список с добавленным элементом
		beg->prev = p;
		beg = p; //запомнить первый элемент в beg
		return; 
	}
	point* r = beg; //встать на начало списка
	for (int i = 0; i < k - 1 && r->next != nullptr; i++, r = r->next); //ищем элемент раньше места вставки

	p->next = r->next; //связать р со списком
	//если элемент не последний, то связать конец списка с р
	if (r->next != nullptr) r->next->prev = p;
	else {
		p->next = nullptr;
		fin = p;
	};
	p->prev = r; //связать р и r
	r->next = p;
	return;
}

int find_key_index(point* beg) {
	string key_word;
	point* r = beg;
	int counter = 1;
	cout << "Input the key word" << endl;
	cin >> key_word;
	while (r) {
		//проверяем ключ для нынешнего элемента
		if (r->key == key_word) return counter; //если найден, то возвращаем НОМЕР элемента
		r = r->next; //двигаемся дальше
		counter++;
	}
	cout << "not found" << endl;
	return NULL;
}

void delete_list(point*& beg, point*&fin) {
	point* p = beg;
	int counter = 1;
	while (p->next != nullptr) { //считаем количество элементом
		p = p->next;
		counter++;
	}
	for (int i = counter; i > 0; i--) {// удаление элементов списка, начиная с последнего
		del_point(beg, i, fin);
	}
	return;
}

void printINfile(point*beg, string path) {
	ofstream fout;
	fout.open(path);
	if (!fout.is_open()) { cout << "Error" << endl; }
	else {
		cout << "Information has been copied" << endl;
		point* p = beg;
		while (p != nullptr) {
			fout << p->key; // запись ключа в файл
			fout << "\n";
			p = p->next; //переход на следующий элемент
		}
	}
	fout.close();
}

void readFROMfile(string path, point*&first, point*&last) {
	ifstream fin;
	fin.open(path);
	//point*beg = new(point);
	if (!fin.is_open()) cout << "Error" << endl;
	else {
		cout << "Information has been restored" << endl;	
		string word;
		point* p, * r;
		fin >> word; // чтение ключа из файла
		p = new (point); //создание нового элемента в списке
		p->key = word;
		p->prev = nullptr;
		p->next = nullptr;
		first = p;

		while (!fin.eof()) { //eof = end of file
			word = "";
			fin >> word;
			r = new(point);
			r->key = word;
			p->next = r; // установление связей в списке (аналогично make_list)
			r->prev = p;
			r->next = nullptr;
			p = r;
			last = p;
		}
	}
	fin.close();
	return;
}

void menu(const int c) {
	string path = "myFile.txt";

	if ((c == 1 || c == 7) && is_created) { // для случая, когда список уже создан
		cout << "List is already existing" << endl;
		return;
	}
	else if (c > 1 && c < 7 && !is_created) { // для случая, когда список еще не создан
		cout << "Create or restore a list"<< endl;
		return;
	}
	switch (c) {
	case 1:{
		is_created = true;
		first = nullptr;
		last = nullptr;
		make_list(first, last);
		break;
	}
	case 2:{
		print_list(first);
		int num = find_key_index(first);
		int k;
		cout << "Input K" << endl;
		cin >> k;
		for (int i = 0; i < k; i++) {
			add_point(first, num + i, last);
		}
		num += k;
		for (int i = 0; i < k; i++) {
			add_point(first, num + i + 1, last);
		}
		print_list(first);
		break;
	}
	case 3:{
		print_list(first);
		int n;
		cout << "Position of an element you want to delete?" << endl;
		cin >> n;
		del_point(first, n, last);
		print_list(first);
		break;
	}
	case 4:{
		print_list(first);
		break;
	}
	case 5:{
		printINfile(first, path);
		break;
	}
	case 6:{
		cout << "List was destroyed" << endl;
		delete_list(first, last);
		is_created = false;
		break;
	}

	case 7:{
		readFROMfile(path, first = nullptr, last = nullptr);
		if (first == nullptr) cout << "There's nothing to restore"<< endl;
		else is_created = true;
		break;
	}
	default:{
		cout << "Exiting from menu" << endl;
		delete_list(first, last);
		exit(777);
	}
	}
}

int main() {
	/*point* first = nullptr, * last = nullptr;
	make_list(first, last);
	print_list(first);
	
	cout << "Position of an element you want to delete?" << endl;
	int n;
	cin >> n;

	del_point(first, n, last);
	print_list(first);

	int num = find_key_index(first);

	cout << "Input K" << endl;
	int k;
	cin >> k;
	for (int i = 0; i < k; i++) {
		add_point(first, num+i, last);
	}
	num += k;
	for (int i = 0; i < k; i++) {
		add_point(first, num + i + 1, last);
	}
	print_list(first);
	
	string path = "myFile.txt";
	printINfile(first, path);
	delete_list(first, last);
	print_list(first);
	point* second = nullptr;
	readFROMfile(path, second, last = nullptr);
	print_list(second);
		*/
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