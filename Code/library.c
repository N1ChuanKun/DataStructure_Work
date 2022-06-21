#pragma warning (disable:4996)
#include "library.h"
#include "tools.h"
#define CHARS_NUM 256	// 한 바이트가 가질 수 있는 최대 크기 2^8 = 256
#define INDEX_TABLE 100
#define BUFF_LENGTH 200
Artist* artist_dir[CHARS_NUM];	// 배열 한 칸당 artist 포인터 타입 자료형을 저장하는 배열
SNode* index_dir[INDEX_TABLE];
Artist* play_list[INDEX_TABLE];
int num_index = 0;
void initialize() // Aritst 배열과 INDEX 배열을 초기화
{
	for (int i = 0; i < CHARS_NUM; i++)
		artist_dir[i] = NULL;
	for (int i = 0; i < INDEX_TABLE; i++)
		index_dir[i] = NULL;
	for (int i = 0; i < INDEX_TABLE; i++)
		play_list[i] = NULL;
	//play_list[SIZE_INDEX_TABLE] = NULL;
}
void load(FILE* fp)
{
	char buffer[BUFF_LENGTH];
	char* name, * title;
	while (1)
	{
		if (read_line(fp, buffer, BUFF_LENGTH) <= 0)
			break;
		name = strtok(buffer, "#");	// 공백문자 전까지 토큰화하여 저장함
		if (strcmp(name, " ") == 0) // 이름이 존재하지 않을 경우 NULL리턴
			name = NULL;
		else
			name = strdup(name); // 이름을 복제하여 저장함
		title = strtok(NULL, "#");	// 공백문자 전까지 토큰화하여 저장함
		if (strcmp(title, " ") == 0) // 이름이 존재하지 않을 경우 NULL리턴
			title = NULL;
		else
			title = strdup(title); // 이름을 복제하여 저장함
		printf("%s %s \n", name, title);
		add_song(name, title);
	}
}
Artist* create_artist(char* name) // Artist 객체를 생성
{
	Artist* ptr_artist = (Artist*)malloc(sizeof(Artist));
	ptr_artist->name = name;
	ptr_artist->head = NULL;
	ptr_artist->tail = NULL;
	ptr_artist->next = NULL;
	return ptr_artist;
}
Artist* add_artist(char* name)
{
	// Artist 객체를 만드는 일을 함수로 사용함
	Artist* ptr_artist = create_artist(name);
	Artist* p = artist_dir[(unsigned char)name[0]]; // 새로 만들 연결리스트의 첫 번째(배열의 이름)
	Artist* q = NULL;	// 연결 리스트에서 p를 따라가며 위치를 기억하는 용도로 사용
	while (p != NULL && strcmp(p->name, name) < 0)	// 순서가 있는 리스트에서 p보다 작은 값이 나타날 때까지 반복
	{
		q = p;
		p = p->next;
	}
	if (p == NULL && q == NULL) // 리스트가 비어있을 때를 의미, 즉 p가 유일한 노드가 됨
	{
		artist_dir[(unsigned char)name[0]] = ptr_artist;
	}
	else if (q == NULL)	// p가 맨 앞에 위치함
	{
		ptr_artist->next = artist_dir[(unsigned char)name[0]];
		artist_dir[(unsigned char)name[0]] = ptr_artist;
	}
	else
	{
		ptr_artist->next = p;
		q->next = ptr_artist;
	}
	return ptr_artist;
}
Song* create_song(Artist* ptr_artist, char* title)
{
	Song* ptr_song = (Song*)malloc(sizeof(Song));
	ptr_song->artist = ptr_artist;
	ptr_song->title = title;
	ptr_song->index = num_index;
	num_index++;
	return ptr_song;
}
void insert_node(Artist* ptr_artist, SNode* ptr_snode)
{
	SNode* p = ptr_artist->head;
	// head에 저장된 값보다 ptr_snode에 저장된 제목이 커질 때까지 while문을 반복함
	while (p != NULL && strcmp(p->song->title, ptr_snode->song->title) < 0)
		p = p->next;
	// p가 ptr_snode가 들어갈 자리보다 한 자리 뒤에 있기 때문에 p의 앞 위치에 새로운 snode를 대입함
	// 1. 연결리스트가 비어있을 때 2. 맨 앞에 대입할 때 3. 맨 뒤에 대입할 때 4. 노래 사이에 대입함
	if (ptr_artist->head == NULL) { // 첫 번째 케이스
		ptr_artist->head = ptr_snode;
		ptr_artist->tail = ptr_snode;	// 리스트의 유일한 곡이기 때문에 이중 연결리스트의 head와 tail이 모두 ptr_snode가 됨
	}
	else if (p == ptr_artist->head) { // 두 번째 케이스
		ptr_snode->next = ptr_artist->head;
		ptr_artist->head->prev = ptr_snode;
		ptr_artist->head = ptr_snode; // 새로운 곡이 곡 리스트의 첫 번째 곡 자리에 위치함
	}
	else if (p == NULL) { // 세 번째 케이스
		ptr_snode->prev = ptr_artist->tail;
		ptr_artist->tail->next = ptr_snode;
		ptr_artist->tail = ptr_snode;
	}
	else {	// 네 번째 케이스. p 앞에 노래를 추가함
		ptr_snode->next = p;
		ptr_snode->prev = p->prev;
		p->prev->next = ptr_snode;
		p->prev = ptr_snode;
	}
}
void insert_to_index_dir(Song* ptr_song)	// 단방향 연결리스트
{
	SNode* ptr_snode = (SNode*)malloc(sizeof(SNode));
	ptr_snode->song = ptr_song;
	ptr_snode->next = NULL;
	ptr_snode->prev = NULL;
	int index = ptr_song->index % INDEX_TABLE; // 노래의 인덱스를 배열의 크기로 나눈 나머지
	SNode* p = index_dir[index];	// 해당 인덱스의 첫 번째 곡의 주소
	SNode* q = NULL; // 단방향 연결리스트를 순서가 있는 리스트로 만들기 위한 조건
	while (p != NULL && strcmp(p->song->title, ptr_song->title) < 0) {	// 알파벳순으로 정렬함
		q = p;	// p보다 한 순서 뒤에 있는 노드
		p = p->next;
	}
	if (q == NULL) { // 첫 번째 위치에 저장
		ptr_snode->next = p;
		index_dir[index] = ptr_snode; // 해당 인덱스의 첫 번째 곡이 됨
	}
	else {	// q 다음에 추가
		ptr_snode->next = p;
		q->next = ptr_snode;
	}
}
void add_song(char* artist, char* title)
{
	// 가수가 이미 존재하는 경우를 가정함
	// 존재하지 않는다면 NULL return
	Artist* ptr_artist = find_artist(artist);	// 가수를 찾아서 Artist 포인터를 return
	if (ptr_artist == NULL)	// 만약 가수가 플레이리스트에 존재하지 않는 경우
	{
		ptr_artist = add_artist(artist); // artist라는 이름을 가진 Artist 객체를 하나 추가하여 그 주소를 리턴함
	}
	Song* ptr_song = create_song(ptr_artist, title);
	SNode* ptr_snode = (SNode*)malloc(sizeof(SNode));
	ptr_snode->song = ptr_song;	// SNode들끼리 이중 연결리스트로 연결되는 구조.
	ptr_snode->next = NULL; // 의도치 않은 실수를 방지하기 위해 구조체의 포인터 값은 null로 설정함
	insert_node(ptr_artist, ptr_snode);
	insert_to_index_dir(ptr_song);
}
Artist* find_artist(char* name)
{
	Artist* p = artist_dir[(unsigned char)name[0]]; // 찾는 가수가 속한 초성 그룹의 첫 번째 가수. 배열의 이름이 곧 주소이기 때문!
	/* name[0] : 00000000 ~ 11111111 */
	// int에서 첫 비트 1은 음수가 됨
	// 그렇기 때문에 unsigned char로 형변환함
	// char = 8bit, int = 32bit
	while (p != NULL && strcmp(p->name, name) < 0)	// 알파벳 순으로 더 큰 이름이 나온다면 끝까지 읽지 않도록 함
		p = p->next; // 이름을 찾을 때 까지 p는 계속 다음 노드로 이동
	if (p != NULL && strcmp(p->name, name) == 0)
		return p;	// p가 NULL이라면 NULL return, name을 찾으면 p를 return
	else
		return NULL;
}
void status()
{
	for (int i = 0; i < CHARS_NUM; i++)
	{
		Artist* p = artist_dir[i];	// 배열의 이름이 곧 주소가 됨
		while (p != NULL)
		{
			print_artist(p);
			p = p->next;
		}
	}
}
void status_p()
{
	for (int i = 0; i < INDEX_TABLE; i++)
	{
		SNode* p = play_list[i];	// 배열의 이름이 곧 주소가 됨
		while (p != NULL)
		{
			print_song(p->song);
			p = p->next;
		}
	}
}
void print_artist(Artist* p)
{
	printf("%s\n", p->name);
	SNode* ptr_snode = p->head;
	while (ptr_snode != NULL)
	{
		if (ptr_snode->song == NULL)
			ptr_snode = ptr_snode->next;
		print_song(ptr_snode->song);
		ptr_snode = ptr_snode->next;
	}
}
void print_song(Song* ptr_song)
{
	printf("    %d: %s\n", ptr_song->index, ptr_song->title);
}
void search_song1(char* artist) {
	Artist* ptr_artist = find_artist(artist);
	if (ptr_artist == NULL) {	// 가수가 존재하지 않을 경우를 가정
		printf("No such artist exists.\n");
		return;
	}
	print_artist(ptr_artist);
}
SNode* find_snode(Artist* ptr_artist, char* title)
{
	SNode* ptr_snode = ptr_artist->head;
	while (ptr_snode != NULL && strcmp(ptr_snode->song->title, title) < 0)	// ptr_snode의 노래 제목이 title보다 더 크더라도 반복할 이유가 없음
		ptr_snode = ptr_snode->next;
	if (ptr_snode != NULL && strcmp(ptr_snode->song->title, title) == 0) // 곡을 찾았을 경우
		return ptr_snode;
	else	// 곡을 찾지 못 했을 때
		return NULL;
}
void search_song(char* artist, char* title) {
	Artist* ptr_artist = find_artist(artist);
	if (ptr_artist == NULL) {	// 가수가 존재하지 않을 때
		printf("No such artist exists.\n");
		return;
	}
	SNode* ptr_snode = find_snode(ptr_artist, title);	// 독립된 함수로 만들어서 사용
	if (ptr_snode != NULL) {
		printf("Found:\n");
		print_song(ptr_snode->song);
	}
	else {
		printf("No such song exists.\n");
		return;
	}
}
SNode* find_song(int index)
{
	SNode* ptr_snode = index_dir[index % INDEX_TABLE]; // 해당 index의 첫 번째 노래
	while (ptr_snode != NULL && ptr_snode->song->index != index)
		ptr_snode = ptr_snode->next;
	return ptr_snode;
}
void play(int index)
{
	SNode* ptr_snode = find_song(index);
	if (ptr_snode == NULL) {
		printf("No such song exists.\n");
	}
	printf("Playing : %s \n", ptr_snode->song->title);
}
void playlist(int index)
{
	SNode* ptr_snode = find_song(index);
	if (ptr_snode == NULL) {
		printf("No such song exists.\n");
	}
	for (int i = 0; i < INDEX_TABLE; i++)
		if (play_list[i] == NULL) {
			play_list[i] = ptr_snode;
			break;
		}
	printf("add the playlist: %s \n", ptr_snode->song->title);
}
void save_song(Song* ptr_song, FILE* fp)
{
	if (ptr_song->artist != NULL)
		fprintf(fp, "%s#", ptr_song->artist->name); // 파일에 출력
	else
		fprintf(fp, " #");
	if (ptr_song->title != NULL)
		fprintf(fp, "%s#\n", ptr_song->title);
	else
		fprintf(fp, " #\n");
}
void save_artist(Artist* p, FILE* fp)
{
	SNode* ptr_snode = p->head;
	while (ptr_snode != NULL)
	{
		save_song(ptr_snode->song, fp);
		ptr_snode = ptr_snode->next;
	}
}
void save(FILE* fp)
{
	for (int i = 0; i < CHARS_NUM; i++)
	{
		Artist* p = artist_dir[i];	// 배열의 이름이 곧 주소가 됨
		while (p != NULL)
		{
			save_artist(p, fp);
			p = p->next;
		}
	}
}
void remove1(int index)
{
	SNode* q = NULL;
	SNode* p = index_dir[index % INDEX_TABLE]; // 해당 index의 첫 번째 노래. 즉 head node
	while (p != NULL && p->song->index != index) {
		q = p;
		p = p->next;
	}
	if (p == NULL) {	// 처음부터 빈 list or 곡이 존재하지 않을 경우
		printf("No such song exists.\n");
		return;
	}
	Song* ptr_song = p->song; // SNode와 Song 모두 삭제해야 함. Artist에서의 노래와 인덱스에서의 노래 모두를 삭제
	if (q == NULL) { // first 삭제
		index_dir[index % INDEX_TABLE] = p->next; // p 다음 노래를 배열의 head로 저장함
	}
	else {	// q 다음을 삭제함
		q->next = p->next;
	}
	free(p); // 삭제된 인덱스의 SNode를 삭제함
	Artist* ptr_artist = ptr_song->artist;
	SNode* ptr_snode = find_snode(ptr_artist, ptr_song->title);
	if (ptr_snode == NULL) {
		printf("Failed to find snode.\n");
		return;
	}
	// Artist의 SNode를 찾았을 때
	remove_snode(ptr_artist, ptr_snode);
	destroy_song(ptr_song);
}
void remove_snode(Artist* ptr_artist, SNode* ptr_snode)
{
	SNode* first = ptr_artist->head;
	SNode* last = ptr_artist->tail;
	if (first == ptr_snode && last == ptr_snode) { // SNode가 유일한 노드일 경우
		first = NULL;
		last = NULL;
	}
	else if (first == ptr_snode) { // 첫 번째를 삭제함
		ptr_snode->next->prev = NULL;
		first = ptr_snode->next;
	}
	else if (last == ptr_snode) { // 마지막 번째를 삭제함
		ptr_snode->prev->next = NULL;
		last = ptr_snode->prev;
	}
	else { // 가운데 번째
		ptr_snode->next->prev = ptr_snode->prev;
		ptr_snode->prev->next = ptr_snode->next;
	}
	free(ptr_snode);
}
void destroy_song(Song* ptr_song)
{
	if (ptr_song->title != NULL)
		free(ptr_song->title);
	free(ptr_song);
}
/* 다음은 차트 관련 코드입니다
#define SIZE_CHART 10 // 10개 음악을 출력할 수 있는 차트

Song* play_chart[SIZE_CHART]; // 차트 저장하는 리스트

int chart_index = 0; // 다음 음악 저장 index(위치)

void status_chart()
{
	for (int i = 0; i < NUM_CHARS; i++)
	{
		Artist* p = artist_directory[i];   // 예 i=65일때, p가 이름'A'(ASCII코드)로 시작하는 가수를 가리킨다
		init_chart(p);  // 차트에 대한 초기화(미작성)
		while (p != NULL)
		{
			create_chart(p); //"A"가수의 리스트에 대해 함수 호출
			p = p->next;    // 다음 이름으로 이동
		}
	}
}

void create_chart(Artist* p)
{
	SNode* ptr_snode = p->head;
	while (ptr_snode != NULL)
	{
		if (chart_index > 10) { 
			play_chart[chart_index] = ptr_snode->song;
			ptr_snode = ptr_snode->next;
			chart_index++; // till chart_index = 10;
		}
		
		
		
		while (ptr_snode->song != NULL) {
			if (ptr_snode->song->playtimes > play_chart[chart_index - 1]->playtimes) { // ptr가리키는 대상의 음악과 마지막 위치의 음악의 playtimes 비교
			   // 다음에 차트 sort 하려고
				insert_to_chart(ptr_snode->song); // 함수호출하여 ptr->song를 차트에 삽입(playtimes: big->small)
				chart_index++;     //다음 위치 표기
			}
			ptr_snode = ptr_snode->next;
		}
	}
}

// 차트 초기화
void sort_chart() {
	int i, j, max;
	Song* temp;
	for (i = 0; i < chart_index - 1; i++) {
		max = temp;
		for (j = i + 1; j < chart_index; j++) {
			if (play_chart[max] < play_chart[j])
				max = j;
		}
		play_chart[max] = temp;
		play_chart[max] = play_chart[i];
		play_chart[i] = temp;
	}
}

void insert_to_chart(Song* ptr)
{
	for (int i = 0; i > chart_index - 1; i++)//마지막 데이터와 비교
	{
		if (play_chart[i]->playtimes < ptr->playtimes)
		{
			// (i가 ptr저장하고 싶은 위치의 index)
//Loop:
// index:8~i의 데이터 뒤으로 한칸 이동
// ptr을 i에 삽입
			for (int j = 8; j > i; j++) {
				// 어떻게 이동할지 구현(미완성)
				if (play_chart[j] != NULL) {
					// NULL이면 앞으로 이동
				}
			}
		}
	}
}*/
