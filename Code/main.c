#pragma warning (disable:4996)
#include "tools.h"
#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFF_LENGTH 200
void command_add();
void command();
void command_load();
void command_search();
void command_play();
void command_save();
void command_remove();
void command_add_p();

int main()
{
	clock_t start, end;
	start = clock();
	initialize();	// Artist �迭�� �ʱ�ȭ
	command_load();
	command();
	end = clock();
	printf("[time : %f]\n", (float)(end - start) / CLOCKS_PER_SEC); // ���α׷� ����ð� ����

}
void command_load()
{
	char buffer[BUFF_LENGTH];
	printf("File name ? (.txt) ");
	if (read_line(stdin, buffer, BUFF_LENGTH) <= 0) // �� ������ �Է��� ����(���� ������, �����͸� �о�� ����, ���� ����)
		return;	// ������ ������ �ҷ����� ����
	FILE* fp = fopen(buffer, "r"); // ���� ������ fp�� buffer�� ����� ������ �̸��� "�б� ����"���� ������
	if (fp == NULL) {
		printf("Failed to search file.\n");
		return;
	}
	load(fp); // ���� �����͸� �Ű������� �Ѱ���
	fclose(fp); // ������ �� ���� ���� ������ ����
}
void command()
{
	char command_line[BUFF_LENGTH];
	char* command;
	while (1)	// ���� ������ ���� -> ������Ʈ�� ����ϸ� ������� ��ɾ �Է¹���
	{
		printf("$ "); // prompt
		if (read_line(stdin, command_line, BUFF_LENGTH) <= 0) // �� ������ �Է��� ����(���� ������, �����͸� �о�� ����, ����)
			continue;	// �ٽ� ��� ������Ʈ�� ȣ��
		command = strtok(command_line, " ");	// ���鹮�� ������ ��ūȭ�Ͽ� ������
		if (strcmp(command, "add") == 0)
			command_add();
		else if (strcmp(command, "search") == 0)
			command_search();
		else if (strcmp(command, "play") == 0)
			command_play();
		else if (strcmp(command, "remove") == 0)
			command_remove();
		else if (strcmp(command, "add_p") == 0)
			command_add_p();
		else if (strcmp(command, "status_p") == 0)
			status_p();
		else if (strcmp(command, "save") == 0) {
			char* tmp = strtok(NULL, " ");
			if (strcmp(tmp, "as") != 0)
				continue;
			command_save();
		}
		else if (strcmp(command, "status") == 0)
			status();
		else if (strcmp(command, "exit") == 0)
			break;
	}
}
void command_remove()	// �뷡�� index�� ��ūȭ�ؼ� �ش� ���� ������
{
	char* id_str = strtok(NULL, " "); // COMMAND ������ ���� ��ȣ�� �о
	int index = atoi(id_str);
	remove1(index);
}
void command_save()
{
	char* file_name = strtok(NULL, " ");
	FILE* fp = fopen(file_name, "w");	// ������ ��� �� �뷡�� ������
	save(fp);
	fclose(fp);
}
void command_add_p()
{
	char* id_str = strtok(NULL, " "); // COMMAND ������ ���� ��ȣ�� �о
	int index = atoi(id_str);
	playlist(index);
}
void command_play()
{
	char* id_str = strtok(NULL, " "); // COMMAND ������ ���� ��ȣ�� �о
	int index = atoi(id_str);
	play(index);
}
void command_search()
{
	char name[BUFF_LENGTH], title[BUFF_LENGTH];
	printf("    Artist: ");
	if (read_line(stdin, name, BUFF_LENGTH) <= 0) {
		printf("    Artist name required \n");
		return;
	}
	printf("    Title: "); // 1. ������ �������� ���� �� 2. ������ ������ ��
	int length = read_line(stdin, title, BUFF_LENGTH);
	if (length <= 0) {
		search_song1(name);
	}
	else
		search_song(name, title); // �Լ� �����ε��� ���� search_song ȣ��
}
void command_add()
{
	char buffer[BUFF_LENGTH];
	char* artist = NULL, * title = NULL;
	printf("    Artist: ");
	if (read_line(stdin, buffer, BUFF_LENGTH) > 0) // ��� �� ���� �̻� �Է��� �޾ƾ� ��
		artist = strdup(buffer); // �Է¹��� �̸��� �����ؼ� ����
	printf("    Title: ");
	if (read_line(stdin, buffer, BUFF_LENGTH) > 0) // ��� �� ���� �̻� �Է��� �޾ƾ� ��
		title = strdup(buffer); // �Է¹��� ���� �����ؼ� ����
	printf("%s %s \n", artist, title);	// �Է¹��� ������ ���
	/* ���� ���̺귯���� �߰� */
	add_song(artist, title);
}