#pragma once

#include <string>
#include <stdlib.h>
#include <list>
using namespace std;
typedef struct stud
{
	int number;
	//char name[20];
	//char sex[5];
	wchar_t name[20];
	wchar_t sex[5];
	int year;
	int month;
	int day;
	int classnumber;
	int math;
	int English;
	int computer;
	int military;
	int PE;
}STU;
wchar_t* c2w(const char *str);
wchar_t* read();
void write(char* s);
void* filescanf();
void filechange(list<STU>* Whole);