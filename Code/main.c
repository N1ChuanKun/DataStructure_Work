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
	initialize();	// Artist 배열을 초기화
	command_load();
	command();
	end = clock();
	printf("[time : %f]\n", (float)(end - start) / CLOCKS_PER_SEC); // 프로그램 수행시간 측정

}
void command_load()
{
	char buffer[BUFF_LENGTH];
	printf("File name ? (.txt) ");
	if (read_line(stdin, buffer, BUFF_LENGTH) <= 0) // 줄 단위로 입력을 받음(파일 포인터, 데이터를 읽어올 변수, 길이 순서)
		return;	// 데이터 파일을 불러오지 않음
	FILE* fp = fopen(buffer, "r"); // 파일 포인터 fp에 buffer에 저장된 파일의 이름을 "읽기 전용"으로 저장함
	if (fp == NULL) {
		printf("Failed to search file.\n");
		return;
	}
	load(fp); // 파일 포인터를 매개변수로 넘겨줌
	fclose(fp); // 파일을 다 읽은 다음 파일을 닫음
}
void command()
{
	char command_line[BUFF_LENGTH];
	char* command;
	while (1)	// 무한 루프로 설정 -> 프롬프트를 출력하며 사용자의 명령어를 입력받음
	{
		printf("$ "); // prompt
		if (read_line(stdin, command_line, BUFF_LENGTH) <= 0) // 줄 단위로 입력을 받음(파일 포인터, 데이터를 읽어올 변수, 길이)
			continue;	// 다시 명령 프롬프트를 호출
		command = strtok(command_line, " ");	// 공백문자 전까지 토큰화하여 저장함
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
void command_remove()	// 노래의 index를 토큰화해서 해당 곡을 삭제함
{
	char* id_str = strtok(NULL, " "); // COMMAND 다음에 오는 번호를 읽어냄
	int index = atoi(id_str);
	remove1(index);
}
void command_save()
{
	char* file_name = strtok(NULL, " ");
	FILE* fp = fopen(file_name, "w");	// 파일을 열어서 새 노래를 저장함
	save(fp);
	fclose(fp);
}
void command_add_p()
{
	char* id_str = strtok(NULL, " "); // COMMAND 다음에 오는 번호를 읽어냄
	int index = atoi(id_str);
	playlist(index);
}
void command_play()
{
	char* id_str = strtok(NULL, " "); // COMMAND 다음에 오는 번호를 읽어냄
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
	printf("    Title: "); // 1. 가수가 존재하지 않을 때 2. 가수가 존재할 때
	int length = read_line(stdin, title, BUFF_LENGTH);
	if (length <= 0) {
		search_song1(name);
	}
	else
		search_song(name, title); // 함수 오버로딩을 통해 search_song 호출
}
void command_add()
{
	char buffer[BUFF_LENGTH];
	char* artist = NULL, * title = NULL;
	printf("    Artist: ");
	if (read_line(stdin, buffer, BUFF_LENGTH) > 0) // 적어도 한 글자 이상 입력을 받아야 함
		artist = strdup(buffer); // 입력받은 이름을 복사해서 저장
	printf("    Title: ");
	if (read_line(stdin, buffer, BUFF_LENGTH) > 0) // 적어도 한 글자 이상 입력을 받아야 함
		title = strdup(buffer); // 입력받은 곡을 복사해서 저장
	printf("%s %s \n", artist, title);	// 입력받은 정보를 출력
	/* 음악 라이브러리에 추가 */
	add_song(artist, title);
}