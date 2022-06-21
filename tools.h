#pragma once	// ifndef와 동일한 효과를 가짐, 헤더파일 중복선언 방지
#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>

int read_line(FILE* fp, char str[], int n);	// // 키보드 입력인 경우 파일 포인터 자리에 stdin 입력, 저장할 배열과 배열의 크기를 줌. 

#endif
