#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int x, y;
} shift;

void readppm(FILE* fp, int* w, int* h);
shift calc_SSD(int** color, int** B, int w, int h);
shift calc_NCC(int** color, int** B, int w, int h);

int main()
{
    int response, response2;
    int width = 0, height = 0;
    int** R = NULL;
    int** G = NULL;
    int** B = NULL;
    int r, g, b;
    FILE* fp;
    char* filename[30];
    char* ptr = strchr(filename, '.');

    do {
        int i, j;

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
            printf("이미지 이름: ");
            scanf("%s", &filename);
            fp = fopen(filename, "r");
            if (fp == NULL)
            {
                printf("Cannot open the file!\n");
                printf("\n");
                continue;
            }
            readppm(fp, &width, &height);
            R = (int**)malloc(sizeof(int*) * height);
            for (i = 0; i < height; i++)
            {
                *(R + i) = (int*)malloc(sizeof(int) * width);
            }

            G = (int**)malloc(sizeof(int*) * height);
            for (i = 0; i < height; i++)
            {
                *(G + i) = (int*)malloc(sizeof(int) * width);
            }

            B = (int**)malloc(sizeof(int*) * height);
            for (i = 0; i < height; i++)
            {
                *(B + i) = (int*)malloc(sizeof(int) * width);
            }

            for (i = 0; i < height; i++)
            {
                for (j = 0; j < width; j++)
                {
                    fscanf(fp, "%d%d%d", &r, &g, &b);
                    R[i][j] = r;
                    G[i][j] = g;
                    B[i][j] = b;
                }
            }
            printf("이미지 읽기를 완료했습니다.\n");
            printf("\n");

            printf("=======================\n");
            printf("[1] 이미지 변경 - 현재 이미지: %s\n", filename);
            printf("[2] 이미지 정합(SSD)\n");
            printf("[3] 이미지 정합(NCC)\n");
            printf("[4] 종료\n");
            printf("=======================\n");
            printf("메뉴 선택> ");
            scanf("%d", &response2);

            if (response2 == 1)
            {
                // 이미 불러온 다른 이미지 삭제(동적할당 해제)
                printf("이미지 이름: ");
                scanf("%s", &filename);
                printf("=======================\n");
                printf("[1] 이미지 변경 - 현재 이미지: %s\n", filename);
                printf("[2] 이미지 정합(SSD)\n");
                printf("[3] 이미지 정합(NCC)\n");
                printf("[4] 종료\n");
                printf("=======================\n");
                printf("메뉴 선택> ");
                scanf("%d", &response2);
            }
            else if (response2 == 2)
            {
                shift R_min_shift = calc_SSD(R, B, width, height);
                shift G_min_shift = calc_SSD(G, B, width, height);
                printf("\n");
                printf("=======================\n");
                printf("SSD - R : [%d, %d]  G : [%d, %d]\n", R_min_shift.x, R_min_shift.y, G_min_shift.x, G_min_shift.y);

                while (ptr != NULL)
                {
                    ptr = strchr(ptr + 1, '.'); // 어케 하는겨 ㅡㅡ
                }

                printf("결과 이미지 파일: %s_SSD_R%d_%d_G%d_%d%s\n", filename, R_min_shift.x, R_min_shift.y, G_min_shift.x, G_min_shift.y, ptr);


            }
            else if (response2 == 3)
            {
                shift R_min_shift = calc_NCC(R, B, width, height);
                shift G_min_shift = calc_NCC(G, B, width, height);
                printf("\n");
                printf("=======================\n");
                printf("NCC - R : [%d, %d]  G : [%d, %d]\n", R_min_shift.x, R_min_shift.y, G_min_shift.x, G_min_shift.y);
                printf("결과 이미지 파일: %s_NCC_R%d_%d_G%d_%d.ppm\n", filename, R_min_shift.x, R_min_shift.y, G_min_shift.x, G_min_shift.y);
            }
            else if (response2 == 4)
            {
                return 0;
            }
            else // (response2 < 1 || response2 > 4)
            {
                printf("It is not a valid number!\n");
                continue; // 여기서도 continue가 맞나?
            }
        }
        else if (response == 2)
        {
            printf("Call an image first!\n");
        }
        else if (response == 3)
        {
            printf("Call an image first!\n");
        }
        else if (response == 4)
        {
            return 0;
        }
        else if (response < 1 || response > 4) // response가 1~4 외의 숫자일 때
        {
            printf("It is not a valid number!\n");
            continue;
        }
    } while (response != 4);

    return 0;
}

void readppm(FILE *fp, int* w, int* h)
{
    int magicnumber;
    int i, j;
    int r, g, b;

    FILE* infile;

    fscanf(fp, "%*c%*d %d %d %*d\n", w, h);
}

shift calc_SSD(int** color, int** B, int w, int h)
{
    int min_sum = 2147483647; // _CRT_INT_MAX?
    int sum =0;
    int row, col; // row는 height, col은 width
    int x_shift, y_shift, temp1, temp2;

    shift min_shift = { -15, -15 };

    for (x_shift = -15; x_shift < 16; x_shift++)
    {
        for (y_shift = -15; y_shift < 16; y_shift++)
        {
            sum = 0;
            for (row = 0; row < h- abs(y_shift); row++)
            {
                for (col = 0; col < w-abs(x_shift); col++)
                {
                    if (x_shift >= 0)
                    {
                        if (y_shift >= 0)
                        {
                            temp1 = color[row+abs(y_shift)][col];
                            temp2 = B[row][col + abs(x_shift)];
                        }
                        else // y_shift < 0
                        {
                            temp1 = color[row][col];
                            temp2 = B[row + abs(y_shift)][col + abs(x_shift)];
                        }
                    }
                    else // x_shift < 0
                    {
                        if (y_shift >= 0)
                        {
                            temp1 = color[row+abs(y_shift)][col + abs(x_shift)];
                            temp2 = B[row][col];
                        }
                        else // y_shift < 0
                        {
                            temp1 = color[row][col+abs(x_shift)];
                            temp2 = B[row+abs(y_shift)][col];
                        }
                    }
                    sum += (temp1 - temp2) * (temp1 - temp2);
                }
            }
            if (min_sum > sum)
            {
                min_sum = sum;
                min_shift.x = x_shift;
                min_shift.y = y_shift;
            }
        }
    }

    return min_shift;
}

shift calc_NCC(int** color, int** B, int w, int h)
{
    double max_sum = -1.0;
    double sum = 0;
    int row, col; // row는 height, col은 width
    int x_shift, y_shift, temp1, temp2;
    unsigned int temp12 , temp1sqrt, temp2sqrt;

    shift min_shift = { -15, -15 };
    for (x_shift = -15; x_shift < 16; x_shift++)
    {
        for (y_shift = -15; y_shift < 16; y_shift++)
        {
            temp12 = 0;
            temp1sqrt = 0;
            temp2sqrt = 0;
            for (row = 0; row < h - abs(y_shift); row++)
            {
                for (col = 0; col < w - abs(x_shift); col++)
                {
                    if (x_shift >= 0)
                    {
                        if (y_shift >= 0)
                        {
                            temp1 = color[row + abs(y_shift)][col];
                            temp2 = B[row][col + abs(x_shift)];
                        }
                        else // y_shift < 0
                        {
                            temp1 = color[row][col];
                            temp2 = B[row + abs(y_shift)][col + abs(x_shift)];
                        }
                    }
                    else // x_shift < 0
                    {
                        if (y_shift >= 0)
                        {
                            temp1 = color[row + abs(y_shift)][col + abs(x_shift)];
                            temp2 = B[row][col];
                        }
                        else // y_shift < 0
                        {
                            temp1 = color[row][col + abs(x_shift)];
                            temp2 = B[row + abs(y_shift)][col];
                        }
                    }

                    temp12 += temp1 * temp2;
                    temp1sqrt += temp1 * temp1;
                    temp2sqrt += temp2 * temp2;
                }
            }

            sum = temp12 / (sqrt(temp1sqrt) * sqrt(temp2sqrt));
            if (max_sum < sum)
            {
                max_sum = sum;
                min_shift.x = x_shift;
                min_shift.y = y_shift;
            }
        }
    }

    return min_shift;
}