#include "stdafx.h"
#include "stdio.h"
#include "list.h"
list<STU> Whole;
char* database = "data.db";
wchar_t * t;
bool cmp(STU x, STU y) {
	if (x.number < y.number)return 1;
	return 0;
}
void* filescanf(){
    FILE *fp;
    //fp=fopen(database,"r");
	fp = fopen("db2.db", "r");
    while(!feof(fp)){
		STU student;
		char name[30], sex[10];
		wchar_t wname[20], wsex[5];
		if (fscanf(fp, "%d%s%s%d%d%d%d%d%d%d%d%d", &student.number, name, sex, &student.year, &student.month, &student.day, &student.classnumber, &student.math, &student.English, &student.computer, &student.military, &student.PE) == 12) {
			MultiByteToWideChar(CP_UTF8, 0, name, -1, wname, 20);
			MultiByteToWideChar(CP_UTF8, 0, sex, -1, wsex, 5);
			wcscpy(student.name, wname);
			wcscpy(student.sex, wsex);
			Whole.push_front(student);
		}
    }
    fclose(fp);
	Whole.sort(cmp);
	return (void*)&Whole;
}
void filechange(list<STU>* Whole){
   list<STU>::iterator p;
   FILE *fp;
   //fp=fopen(database,"w");
   fp = fopen("db2.db", "w");
   for(p=(*Whole).begin();p!=(*Whole).end();++p){
	   char name[30], sex[10];
	   WideCharToMultiByte(CP_UTF8, 0, p->name, -1, name, 30, NULL, NULL);
	   WideCharToMultiByte(CP_UTF8, 0, p->sex, -1, sex, 10, NULL, NULL);
       fprintf(fp,"%d %s %s %d %d %d %d %d %d %d %d %d\n",p->number,name,sex,p->year,p->month,p->day,p->classnumber,p->math,p->English,p->computer,p->military,p->PE);
   }
   fclose(fp);
}
size_t getFileSize(FILE *fn) {
	size_t ret = -1;
	if (fn != NULL) {
		fseek(fn, 0, SEEK_END);
		ret = ftell(fn);
	}
	return ret;
}
wchar_t* c2w(const char *str)
{
	int length = strlen(str) + 1;
	t = (wchar_t*)malloc(sizeof(wchar_t)*length);
	memset(t, 0, length * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str), t, length);
	return t;
}
wchar_t* read() {
	FILE* fn;
	size_t len;
	char* s;
	wchar_t* w;
	fn = fopen(database, "r");
	if (!fn) {
		printf("File Can't Read!");
		return L"";
	}
	len = getFileSize(fn);
	rewind(fn);
	s = (char*)malloc(len + 1);
	len = fread(s, sizeof(unsigned char), len, fn);
	fclose(fn);
	w = c2w(s);
	free(s);
	len = wcslen(w) - 1;
	for (; len > 0 && w[len] != ']'; len--)w[len]=0;
	return w;
}
void write(char* s) {
	FILE* fn;
	fn = fopen(database, "w");
	fwrite(s, sizeof(char), strlen(s), fn);
	fclose(fn);
	return;
}