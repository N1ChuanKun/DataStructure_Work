#pragma warning (disable:4996)
#include "tools.h"// �ҽ� ������ �׻� ������ ��� ������ include �ؾ� ��
int read_line(FILE* fp, char str[], int n) // (���� ������, ���ڿ��� ������ �迭, ���ڿ��� ũ�� ��)
{
	int ch, i = 0;
	while ((ch = fgetc(fp)) != '\n' && ch != EOF)	// fp���� �о���� ���ڿ��� ����Ű�� �ƴϸ� ������ ���� �ƴ� ������ ����
		if (i < n - 1)
			str[i++] = ch; // str[i]�� ch�� �����ϰ� i�� 1 ����
	str[i] = '\0'; // C��� ���ڿ�. �迭�� �������� null ���� ����
	return i; // �� ���ڸ� �о����� ������
}