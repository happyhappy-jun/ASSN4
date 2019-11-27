//
// Created by Unajun on 2019/11/26.
//

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>

void readppm(char* filename);

int main()
{
    int response;
    char filename[10];

    printf("=======================\n");
    printf("[1] 이미지 불러오기\n");
    printf("[2] 이미지 정합(SSD)\n");
    printf("[3] 이미지 정합(NCC)\n");
    printf("[4] 종료\n");
    printf("=======================\n");
    printf("메뉴 선택> ");
    scanf("%d", &response);

    if (response == 1)
    {
        int  w, h;
        printf("이미지 이름: ");
        scanf("%s", &filename);
        readppm(filename);
        printf("이미지 읽기를 완료했습니다.\n");
        printf("\n");

        int** R;
        int i;
        /*
        R  = (int **)malloc(sizeof(int *) * height);
        for (i  = 0; i< height; i++)
        {
           *(R + i) = (int *)malloc(sizeof(int) * width);
        }*/
    }


    printf("=======================\n");
    printf("[1] 이미지 변경 - 현재 이미지: %s\n", filename);
    printf("[2] 이미지 정합(SSD)\n");
    printf("[3] 이미지 정합(NCC)\n");
    printf("[4] 종료\n");
    printf("=======================\n");
    printf("메뉴 선택> ");
    scanf("%d", &response);
    printf("\n");
}



void readppm(char* filename) // int *width; int* height;

{
    int magicnumber;
    int i, w, h;
    int data1, data2, data3;

    FILE* infile;

    infile = fopen("test1.ppm", "r");
    fscanf(infile, "%*c%*d %d %d %*d\n", &w, &h);

    for (i = 0; i < w * h; i++)
    {
        fscanf(infile, "%d %d %d", &data1, &data2, &data3);
        printf("(%d %d %d)\n", data1, data2, data3);
    }


}