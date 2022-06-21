#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef LIBRARY_H
#define LIBRARY_H
typedef struct song Song; //자기참조 구조체
typedef struct snode SNode;
typedef struct artist Artist;
struct song
{
	Artist* artist;
	char* title;
	int index;
};
struct snode
{
	struct snode* next, * prev;
	Song* song;
};
struct artist
{
	char* name;
	struct artist* next; // 이름의 초성이 같은 가수끼리 단방향 리스트로 연결 
	SNode* head, * tail;
};
void initialize();
Artist* create_artist(char* name);
Artist* add_artist(char* name);
Song* create_song(Artist* ptr_artist, char* title);
void insert_node(Artist* ptr_artist, SNode* ptr_snode);
void status();	//status 기능
void status_p();	//플레이리스트 목록 확인
void add_song(char* artist, char* title); //음악 추가 기능
Artist* find_artist(char* name);
void print_artist(Artist* p);
void print_song(Song* ptr_song);
void load(FILE* fp);
void search_song(char* artist);		//가수 검색
void search_song(char* artist, char* title);	//가수, 음악제목 검색
void play(int index);
void playlist(int index);
void save(FILE* fp);
void save_artist(Artist* p, FILE* fp);
void save_song(Song* ptr_song, FILE* fp);
void remove1(int index);
SNode* find_song(int index);
void remove_snode(Artist* ptr_artist, SNode* ptr_snode);
void destroy_song(Song* ptr_song);
#endif