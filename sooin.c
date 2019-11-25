//
// Created by Unajun on 2019/11/25.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void img_print(int wid, int hei, int** red, int** blue, int** green, char pname[]);
void main_menu(int* con, int wid, char pname[30]);
void call_img(char pname[30], int** red, int** green, int** blue, int* hei, int* wid, FILE* photo);

int main() {
    int con=1,wid=0,hei;
    char pname[30];
    FILE* photo=NULL;
    int** red=NULL, ** green=NULL, ** blue=NULL;
    while (con)
    {
        main_menu(&con,wid,pname);
        switch (con)
        {
            case 3:
            case 2:
            case 1:call_img(pname, red, green, blue, &hei, &wid, photo);break;
            case 4:con = 0;continue;
            case 5:img_print(wid, hei, red, blue, green, pname); break;
            default:
                printf("%d�� ��ȿ���� �ʽ��ϴ�. �ٽ� �Է����ּ���.\n\n", con);
                continue;
        }

    }

    return 0;
}
void call_img(char pname[30], int** red, int** green, int **blue, int * hei, int *wid,FILE* photo) {
    int vaild=0;
    int* ori;
    int i,j,count=0;
    char type;
    int t, d;
    printf("�̹��� �̸�:");
    do
    {
        while (getchar() != '\n');
        gets(pname);
        photo = fopen(pname, "r");
        vaild = 0;
        if (photo == NULL) {
            printf("�ش� �̸��� ������ �������� �ʽ��ϴ�. �����̸��� �ٽ� �Է����ּ���!\n");vaild = 1;
        }
    } while (vaild);
    fscanf(photo,"%*c%*d %d %d %*d\n", wid, hei);
    ori = (int*)malloc(sizeof(int) * (*wid) * (*hei) * 3);
    for (i = 0;i < (*wid) * (*hei) * 3;i++) {
        fscanf(photo,"%d", ori+i);
        printf("%d ", ori[i]);
    }
    printf("test");
    red = (int**)calloc((*hei),sizeof(int));
    for (i = 0;i < (*hei); i++) {
        red[i] = (int*)malloc(sizeof(int) * (*wid));
    }
    blue = (int**)calloc((*hei), sizeof(int)*(*wid));
    for (i = 0;i < (*hei); i++) {
        blue[i] = (int*)malloc(sizeof(int) * (*wid));
    }
    green = (int**)calloc((*hei), sizeof(int)*(*wid));
    for (i = 0;i < (*hei); i++) {
        green[i] = (int*)malloc(sizeof(int) * (*wid));
    }
    for (i = 0;i < *hei; i++) {
        for (j = 0;j < *wid*3; j++) {
            switch (j%3)
            {
                case 0: red[i][*wid / 3] = ori[count++];break;
                case 1:green[i][*wid / 3] = ori[count++];break;
                case 2:blue[i][*wid / 3] = ori[count++];break;
            }

        }
    }
    fclose(photo);


}
void main_menu(int *con,int wid,char pname[30]) {

    if (wid == 0) {
        printf("===================;\n\n");
        printf("[1] �̹��� �ҷ�����\n\n");
        printf("[2] �̹��� ����(SSD)\n\n");
        printf("[3] �̹��� ����(NCC)\n\n");
        printf("[4] ����\n\n");
        printf("===================;\n\n");
        printf("�޴� ����>");
        scanf("%d", con);
    }
    else {
        printf("===================;\n\n");
        printf("[1] �̹��� ���� - ���� �̹���: %s\n\n",pname);
        printf("[2] �̹��� ����(SSD)\n\n");
        printf("[3] �̹��� ����(NCC)\n\n");
        printf("[4] ����\n\n");
        printf("===================;\n\n");
        printf("�޴� ����>");
        scanf("%d\n", con);
    }
}
void ssd() {

}
void ncc() {

}
void img_print(int wid, int hei, int** red, int** blue, int** green, char pname[]){
    FILE* photo;
    photo = fopen(pname, "w");
    fprintf(photo, "P2 %d %d 255\n", wid, hei);
    for (int i = 0;i < hei;i++) {
        for (int j = 0;j < wid;j++) {
            fprintf(photo, "%d",red[hei][wid]);
        }
    }
}