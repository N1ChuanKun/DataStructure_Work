#pragma warning (disable:4996)
#include "library.h"
#include "tools.h"
#define CHARS_NUM 256	// �� ����Ʈ�� ���� �� �ִ� �ִ� ũ�� 2^8 = 256
#define INDEX_TABLE 100
#define BUFF_LENGTH 200
Artist* artist_dir[CHARS_NUM];	// �迭 �� ĭ�� artist ������ Ÿ�� �ڷ����� �����ϴ� �迭
SNode* index_dir[INDEX_TABLE];
Artist* play_list[INDEX_TABLE];
int num_index = 0;
void initialize() // Aritst �迭�� INDEX �迭�� �ʱ�ȭ
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
		name = strtok(buffer, "#");	// ���鹮�� ������ ��ūȭ�Ͽ� ������
		if (strcmp(name, " ") == 0) // �̸��� �������� ���� ��� NULL����
			name = NULL;
		else
			name = strdup(name); // �̸��� �����Ͽ� ������
		title = strtok(NULL, "#");	// ���鹮�� ������ ��ūȭ�Ͽ� ������
		if (strcmp(title, " ") == 0) // �̸��� �������� ���� ��� NULL����
			title = NULL;
		else
			title = strdup(title); // �̸��� �����Ͽ� ������
		printf("%s %s \n", name, title);
		add_song(name, title);
	}
}
Artist* create_artist(char* name) // Artist ��ü�� ����
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
	// Artist ��ü�� ����� ���� �Լ��� �����
	Artist* ptr_artist = create_artist(name);
	Artist* p = artist_dir[(unsigned char)name[0]]; // ���� ���� ���Ḯ��Ʈ�� ù ��°(�迭�� �̸�)
	Artist* q = NULL;	// ���� ����Ʈ���� p�� ���󰡸� ��ġ�� ����ϴ� �뵵�� ���
	while (p != NULL && strcmp(p->name, name) < 0)	// ������ �ִ� ����Ʈ���� p���� ���� ���� ��Ÿ�� ������ �ݺ�
	{
		q = p;
		p = p->next;
	}
	if (p == NULL && q == NULL) // ����Ʈ�� ������� ���� �ǹ�, �� p�� ������ ��尡 ��
	{
		artist_dir[(unsigned char)name[0]] = ptr_artist;
	}
	else if (q == NULL)	// p�� �� �տ� ��ġ��
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
	// head�� ����� ������ ptr_snode�� ����� ������ Ŀ�� ������ while���� �ݺ���
	while (p != NULL && strcmp(p->song->title, ptr_snode->song->title) < 0)
		p = p->next;
	// p�� ptr_snode�� �� �ڸ����� �� �ڸ� �ڿ� �ֱ� ������ p�� �� ��ġ�� ���ο� snode�� ������
	// 1. ���Ḯ��Ʈ�� ������� �� 2. �� �տ� ������ �� 3. �� �ڿ� ������ �� 4. �뷡 ���̿� ������
	if (ptr_artist->head == NULL) { // ù ��° ���̽�
		ptr_artist->head = ptr_snode;
		ptr_artist->tail = ptr_snode;	// ����Ʈ�� ������ ���̱� ������ ���� ���Ḯ��Ʈ�� head�� tail�� ��� ptr_snode�� ��
	}
	else if (p == ptr_artist->head) { // �� ��° ���̽�
		ptr_snode->next = ptr_artist->head;
		ptr_artist->head->prev = ptr_snode;
		ptr_artist->head = ptr_snode; // ���ο� ���� �� ����Ʈ�� ù ��° �� �ڸ��� ��ġ��
	}
	else if (p == NULL) { // �� ��° ���̽�
		ptr_snode->prev = ptr_artist->tail;
		ptr_artist->tail->next = ptr_snode;
		ptr_artist->tail = ptr_snode;
	}
	else {	// �� ��° ���̽�. p �տ� �뷡�� �߰���
		ptr_snode->next = p;
		ptr_snode->prev = p->prev;
		p->prev->next = ptr_snode;
		p->prev = ptr_snode;
	}
}
void insert_to_index_dir(Song* ptr_song)	// �ܹ��� ���Ḯ��Ʈ
{
	SNode* ptr_snode = (SNode*)malloc(sizeof(SNode));
	ptr_snode->song = ptr_song;
	ptr_snode->next = NULL;
	ptr_snode->prev = NULL;
	int index = ptr_song->index % INDEX_TABLE; // �뷡�� �ε����� �迭�� ũ��� ���� ������
	SNode* p = index_dir[index];	// �ش� �ε����� ù ��° ���� �ּ�
	SNode* q = NULL; // �ܹ��� ���Ḯ��Ʈ�� ������ �ִ� ����Ʈ�� ����� ���� ����
	while (p != NULL && strcmp(p->song->title, ptr_song->title) < 0) {	// ���ĺ������� ������
		q = p;	// p���� �� ���� �ڿ� �ִ� ���
		p = p->next;
	}
	if (q == NULL) { // ù ��° ��ġ�� ����
		ptr_snode->next = p;
		index_dir[index] = ptr_snode; // �ش� �ε����� ù ��° ���� ��
	}
	else {	// q ������ �߰�
		ptr_snode->next = p;
		q->next = ptr_snode;
	}
}
void add_song(char* artist, char* title)
{
	// ������ �̹� �����ϴ� ��츦 ������
	// �������� �ʴ´ٸ� NULL return
	Artist* ptr_artist = find_artist(artist);	// ������ ã�Ƽ� Artist �����͸� return
	if (ptr_artist == NULL)	// ���� ������ �÷��̸���Ʈ�� �������� �ʴ� ���
	{
		ptr_artist = add_artist(artist); // artist��� �̸��� ���� Artist ��ü�� �ϳ� �߰��Ͽ� �� �ּҸ� ������
	}
	Song* ptr_song = create_song(ptr_artist, title);
	SNode* ptr_snode = (SNode*)malloc(sizeof(SNode));
	ptr_snode->song = ptr_song;	// SNode�鳢�� ���� ���Ḯ��Ʈ�� ����Ǵ� ����.
	ptr_snode->next = NULL; // �ǵ�ġ ���� �Ǽ��� �����ϱ� ���� ����ü�� ������ ���� null�� ������
	insert_node(ptr_artist, ptr_snode);
	insert_to_index_dir(ptr_song);
}
Artist* find_artist(char* name)
{
	Artist* p = artist_dir[(unsigned char)name[0]]; // ã�� ������ ���� �ʼ� �׷��� ù ��° ����. �迭�� �̸��� �� �ּ��̱� ����!
	/* name[0] : 00000000 ~ 11111111 */
	// int���� ù ��Ʈ 1�� ������ ��
	// �׷��� ������ unsigned char�� ����ȯ��
	// char = 8bit, int = 32bit
	while (p != NULL && strcmp(p->name, name) < 0)	// ���ĺ� ������ �� ū �̸��� ���´ٸ� ������ ���� �ʵ��� ��
		p = p->next; // �̸��� ã�� �� ���� p�� ��� ���� ���� �̵�
	if (p != NULL && strcmp(p->name, name) == 0)
		return p;	// p�� NULL�̶�� NULL return, name�� ã���� p�� return
	else
		return NULL;
}
void status()
{
	for (int i = 0; i < CHARS_NUM; i++)
	{
		Artist* p = artist_dir[i];	// �迭�� �̸��� �� �ּҰ� ��
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
		SNode* p = play_list[i];	// �迭�� �̸��� �� �ּҰ� ��
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
	if (ptr_artist == NULL) {	// ������ �������� ���� ��츦 ����
		printf("No such artist exists.\n");
		return;
	}
	print_artist(ptr_artist);
}
SNode* find_snode(Artist* ptr_artist, char* title)
{
	SNode* ptr_snode = ptr_artist->head;
	while (ptr_snode != NULL && strcmp(ptr_snode->song->title, title) < 0)	// ptr_snode�� �뷡 ������ title���� �� ũ���� �ݺ��� ������ ����
		ptr_snode = ptr_snode->next;
	if (ptr_snode != NULL && strcmp(ptr_snode->song->title, title) == 0) // ���� ã���� ���
		return ptr_snode;
	else	// ���� ã�� �� ���� ��
		return NULL;
}
void search_song(char* artist, char* title) {
	Artist* ptr_artist = find_artist(artist);
	if (ptr_artist == NULL) {	// ������ �������� ���� ��
		printf("No such artist exists.\n");
		return;
	}
	SNode* ptr_snode = find_snode(ptr_artist, title);	// ������ �Լ��� ���� ���
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
	SNode* ptr_snode = index_dir[index % INDEX_TABLE]; // �ش� index�� ù ��° �뷡
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
		fprintf(fp, "%s#", ptr_song->artist->name); // ���Ͽ� ���
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
		Artist* p = artist_dir[i];	// �迭�� �̸��� �� �ּҰ� ��
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
	SNode* p = index_dir[index % INDEX_TABLE]; // �ش� index�� ù ��° �뷡. �� head node
	while (p != NULL && p->song->index != index) {
		q = p;
		p = p->next;
	}
	if (p == NULL) {	// ó������ �� list or ���� �������� ���� ���
		printf("No such song exists.\n");
		return;
	}
	Song* ptr_song = p->song; // SNode�� Song ��� �����ؾ� ��. Artist������ �뷡�� �ε��������� �뷡 ��θ� ����
	if (q == NULL) { // first ����
		index_dir[index % INDEX_TABLE] = p->next; // p ���� �뷡�� �迭�� head�� ������
	}
	else {	// q ������ ������
		q->next = p->next;
	}
	free(p); // ������ �ε����� SNode�� ������
	Artist* ptr_artist = ptr_song->artist;
	SNode* ptr_snode = find_snode(ptr_artist, ptr_song->title);
	if (ptr_snode == NULL) {
		printf("Failed to find snode.\n");
		return;
	}
	// Artist�� SNode�� ã���� ��
	remove_snode(ptr_artist, ptr_snode);
	destroy_song(ptr_song);
}
void remove_snode(Artist* ptr_artist, SNode* ptr_snode)
{
	SNode* first = ptr_artist->head;
	SNode* last = ptr_artist->tail;
	if (first == ptr_snode && last == ptr_snode) { // SNode�� ������ ����� ���
		first = NULL;
		last = NULL;
	}
	else if (first == ptr_snode) { // ù ��°�� ������
		ptr_snode->next->prev = NULL;
		first = ptr_snode->next;
	}
	else if (last == ptr_snode) { // ������ ��°�� ������
		ptr_snode->prev->next = NULL;
		last = ptr_snode->prev;
	}
	else { // ��� ��°
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
/* ������ ��Ʈ ���� �ڵ��Դϴ�
#define SIZE_CHART 10 // 10�� ������ ����� �� �ִ� ��Ʈ

Song* play_chart[SIZE_CHART]; // ��Ʈ �����ϴ� ����Ʈ

int chart_index = 0; // ���� ���� ���� index(��ġ)

void status_chart()
{
	for (int i = 0; i < NUM_CHARS; i++)
	{
		Artist* p = artist_directory[i];   // �� i=65�϶�, p�� �̸�'A'(ASCII�ڵ�)�� �����ϴ� ������ ����Ų��
		init_chart(p);  // ��Ʈ�� ���� �ʱ�ȭ(���ۼ�)
		while (p != NULL)
		{
			creat_chart(p); //"A"������ ����Ʈ�� ���� �Լ� ȣ��
			p = p->next;    // ���� �̸����� �̵�
		}
	}
}

void creat_chart(Artist* p)
{
	SNode* ptr_snode = p->head;
	while (ptr_snode != NULL)
	{
		if (chart_index == 0) { // ��Ʈ�� ����ִ� ��Ȳ, play_chart[0]��ġ�� ���� ������ �ϳ� ����
			play_chart[chart_index] = ptr_snode->song;
			ptr_snode = ptr_snode->next;
			chart_index++; //chart_index = 1;
		}

		while (ptr_snode->song != NULL) {
			if (ptr_snode->song->playtimes > play_chart[chart_index - 1]->playtimes) { // ptr����Ű�� ����� ���ǰ� ������ ��ġ�� ������ playtimes ��
			   // ������ ��Ʈ sort �Ϸ���
				insert_to_chart(ptr_snode->song); // �Լ�ȣ���Ͽ� ptr->song�� ��Ʈ�� ����(playtimes: big->small)
				chart_index++;     //���� ��ġ ǥ��
			}
			ptr_snode = ptr_snode->next;
		}
	}
}

void insert_to_chart(Song* ptr)
{
	for (int i = 0; i > chart_index - 1; i++)//������ �����Ϳ� ��
	{
		if (play_chart[i]->playtimes < ptr->playtimes)
		{
			// (i�� ptr�����ϰ� ���� ��ġ�� index)
//Loop:
// index:8~i�� ������ ������ ��ĭ �̵�
// ptr�� i�� ����
			for (int j = 8; j > i; j++) {
				// ��� �̵����� ����(�̿ϼ�)
				if (play_chart[j] != NULL) {
					// NULL�̸� ������ �̵�
				}
			}
		}
	}
}*/