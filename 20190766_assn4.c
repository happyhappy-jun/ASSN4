#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#define RED 0
#define GREEN 1
#define LENGTH 70 //buf length for filename, more doesn't hurt!
#define max(x, y) (((x) > (y)) ? (x) : (y)) //min max macro
#define min(x, y) (((x) < (y)) ? (x) : (y))

typedef unsigned int pixel[3]; //pixel data type

typedef struct {
    unsigned int width, height; //img type, including width, height, and address for data (will be allocated)
    pixel *data;
} image_t;

typedef struct {
    int x, y;
float evaluation;
} shift_t; //shift data type, x,y value and evaluation value for each method

typedef image_t *image; //struct pointer for image

image readPPM(FILE *fp);

void writePPM(image img, const char *filename);

image fillImage(image img, shift_t rsft, shift_t gsft); //fill image structure with given shift value, fill only duplicated area

image alloc_img(unsigned int width, unsigned int height); //alloc img, and data

void free_img(image img); //free img and data

unsigned int getRGB(image img, int mode, unsigned int x, unsigned int y);

shift_t SSD(image img, int mode); //Sum of Squared Differences

shift_t NCC(image img, int mode); //Normalized Cross Correlation

int main() {
    int choice;
    double begin, end;
    shift_t r_shift, g_shift; //red and green shift data
    char filename[LENGTH], temp[LENGTH]; //file names
    char outfile[LENGTH];
    image img = NULL; //init img struct
    FILE *fp;
    do {
        printf("\n===================\n");
        if (img == NULL)printf("[1] 이미지 불러오기\n");
        else printf("[1] 이미지 변경 - 현재 이미지: %s\n", filename); //if data is loaded print loaded image
        printf("[2] 이미지 정합(SSD)\n");
        printf("[3] 이미지 정합(NCC)\n");
        printf("[4] 종료\n");
        printf("===================\n");
        printf("메뉴 선택> ");
        scanf("%d", &choice); //assume int input (as mentioned in instruction)
        switch (choice) {
            case 1:
                printf("이미지 이름:");
                scanf("%s", temp);
                if ((fp = fopen(temp, "r")) == NULL) { //try to open file, failed > return to menu
                    printf("Unable to open image\nReturning Main menu\n\n");
                    break;
                }
                if (img != NULL) { //if img is loaded
                    free_img(img); //free it
                    if ((img = readPPM(fp)) != NULL) { //read img, if fail print error
                        printf("이미지 변경을 완료했습니다.\n");
                        strcpy(filename, temp); //change display filename (case for img is loaded, but change target is unable to open)
                        break;
                    } else {
                        printf("Error while reading image\nReturning Main menu\n\n");
                        break;
                    }
                } else if ((img = readPPM(fp)) != NULL) { //if img is not loaded, read it. Fail -> break
                    printf("이미지 읽기를 완료했습니다.\n");
                    strcpy(filename, temp);
                    break;
                } else {
                    printf("Error while reading image\nReturning Main menu\n\n");
                    break;
                }
            case 2:
                if (img == NULL) { //error msg for image is not loaded, but tried to registration
                    printf("Error: Image is not loaded\nReturning Main menu\n\n");
                    break;
                }
                printf("Calculating...\n");
                begin = clock(); //elapsed time for benchmark
                r_shift = SSD(img, RED); //calc shift for each channel
                g_shift = SSD(img, GREEN);
                printf("Elapsed time: %.3f sec\n", (clock() - begin) / CLOCKS_PER_SEC); //print elapsed time
                image ssd = fillImage(img, r_shift, g_shift); //fill image with given shifts
                strcpy(temp, filename); //copy file name to temp for strtok
                //make file name with sprintf, strtok will cut file name before .ppm
                sprintf(outfile, "%s_%s_R%d_%d_G%d_%d.ppm", strtok(temp, "."), "SSD", r_shift.x, r_shift.y, g_shift.x, g_shift.y);
                writePPM(ssd, outfile); //write image to ppm file
                printf("===================\n");
                printf("SSD - R:[%d, %d] G:[%d, %d]\n결과 이미지 파일: %s\n", r_shift.x, r_shift.y, g_shift.x, g_shift.y, outfile);
                printf("===================\n");
                free_img(ssd); //free ssd image after all done
                break;
            case 3:
                //all process is identical with SSD, so omit comments
                if (img == NULL) {
                    printf("Error: Image is not loaded\nReturning Main menu\n\n");
                    break;
                }
                printf("Calculating...\n");
                begin = clock();
                r_shift = NCC(img, RED);
                g_shift = NCC(img, GREEN);
                end = clock();
                printf("Elapsed time: %.3f sec\n", (end - begin) / CLOCKS_PER_SEC);
                image ncc = fillImage(img, r_shift, g_shift);
                strcpy(temp, filename);
                sprintf(outfile, "%s_%s_R%d_%d_G%d_%d.ppm", strtok(temp, "."), "NCC", r_shift.x, r_shift.y, g_shift.x, g_shift.y);
                writePPM(ncc, outfile);
                printf("===================\n");
                printf("NCC - R:[%d, %d] G:[%d, %d]\n결과 이미지 파일: %s\n", r_shift.x, r_shift.y, g_shift.x, g_shift.y, outfile);
                printf("===================\n");
                free_img(ncc);
                break;
            case 4:
                printf("Thank you\nWritten by Byungjun Yoon\n"); //exit
                break;
            default:
                printf("Invalid input!\nReturning Main Menu\n"); //error handling for choice out of 1~4 range
                break;
        }
    } while (choice != 4); //if choice is 4 exit
    if (img != NULL) free_img(img); //free img if it is allocated (condition is for exiting program right after running)
    fclose(fp); //close fp
    return 0;
}

image alloc_img(const unsigned int width, const unsigned int height) {
    image img;
    img = malloc(sizeof(image_t)); //alloc size of image_t to image_t pointer
    img->data = malloc(width * height * sizeof(pixel)); //alloc pixels
    img->width = width;
    img->height = height;
    return img;
}

image readPPM(FILE *fp) {
    image img;
    char format; //magic num
    unsigned int i, num, w, h, d; //var's
    i = fscanf(fp, "%c%d %u %u %u\n", &format, &num, &w, &h, &d); //read header
    if (format != 'P' || num != 3 || i < 5 || d != 255) return NULL;
    img = alloc_img(w, h); //alloc pixels to img
    if (img != NULL) {
        for (i = 0; i < w * h; i++) {
            fscanf(fp, "%u %u %u", &img->data[i][0], &img->data[i][1], &img->data[i][2]); //read pixels data by 3 u_int
        }
        return img;
    }
    return NULL;
}

image fillImage(image img, shift_t rsft, shift_t gsft) {
    int xs = max(max(rsft.x, gsft.x), 0);//max x start coordinate (+ is right -> max)
    int ys = min(min(rsft.y, gsft.y), 0);//min y start coordinate (- is down -> min)
    unsigned int walk_x, walk_y, width, height;
    width = rsft.x * gsft.x < 0 ? img->width - abs(rsft.x) - abs(gsft.x) : img->width - max(abs(rsft.x), abs(gsft.x)); //calc new width and height
    height = (rsft.y * gsft.y < 0) ? img->height - abs(rsft.y) - abs(gsft.y) : img->height - max(abs(rsft.y), abs(gsft.y));
    image adjusted = alloc_img(width, height); //alloc registered image
    for (walk_x = 0; walk_x < adjusted->width; walk_x++) { //walk through all pixels (of registered)
        for (walk_y = 0; walk_y < adjusted->height; walk_y++) {
            //get shifted pixel value
            adjusted->data[walk_y * adjusted->width + walk_x][0] = getRGB(img, 0, walk_x + xs - rsft.x, walk_y + rsft.y - ys);
            adjusted->data[walk_y * adjusted->width + walk_x][1] = getRGB(img, 1, walk_x + xs - gsft.x, walk_y + gsft.y - ys);
            adjusted->data[walk_y * adjusted->width + walk_x][2] = getRGB(img, 2, walk_x + xs, walk_y + abs(ys));
        }
    }
    return adjusted;
}

void writePPM(image img, const char *filename) {
    unsigned int i;
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "P3 %d %d 255\n", img->width, img->height); //write header
    for (i = 0; i < img->width * img->height; i++) {
        fprintf(fp, "%u %u %u ", img->data[i][0], img->data[i][1], img->data[i][2]); //write pixel (three u_int's)
    }
    fclose(fp);
}

void free_img(image img) {
    free(img->data); //free pixels
    free(img); //free img pointer
}

unsigned int getRGB(image img, const int mode, const unsigned int x, const unsigned int y) { //0 Red, 1 blue 2 green
    return img->data[y * img->width + x][mode]; //get pixel data from 1D-array based on x and y value
}

shift_t SSD(image img, const int mode) { //MODE 0 Red 2 Green
    shift_t sft = {-15, -15, (float) INT_MAX}; //smaller is better -> initial evaluation is INT_MAX
    int I1, I2, shift_x, shift_y, walk_x, walk_y;
    float eval;
    for (shift_x = -15; shift_x < 16; shift_x++) { //walk through all possible shifts
        for (shift_y = -15; shift_y < 16; shift_y++) {
            eval = 0; //reset evaluation
            for (walk_x = 0; walk_x < img->width - abs(shift_x); walk_x++) {
                for (walk_y = 0; walk_y < img->height - abs(shift_y); walk_y++) {
                    I1 = getRGB(img, mode, walk_x + abs(min(shift_x, 0)), walk_y + abs(max(shift_y, 0))); //get shifted pixel data
                    I2 = getRGB(img, 2, walk_x + abs(max(shift_x, 0)), walk_y + abs(min(shift_y, 0)));
                    eval += (I1 - I2) * (I1 - I2); //simple multiplication is faster than pow(I1-I2,2)!!!
                }
            }
            eval = eval / ((img->width - abs(shift_x)) * (img->height - abs(shift_y))); //get eval value by dividing dimension
            if (sft.evaluation > eval) { //if final eval is smaller, update
                sft.x = shift_x;
                sft.y = shift_y;
                sft.evaluation = eval;
            }
        }
    }
    return sft;
}

shift_t NCC(image img, const int mode) { //MODE 0 Red 2 Green
    int I1, I2;
    unsigned long long sumI12, sumI1sq, sumI2sq;
    float eval;
    shift_t sft = {-15, -15, -1}; //min evaluation is -1
    int shift_x, shift_y, walk_x, walk_y;
    for (shift_x = -15; shift_x < 16; shift_x++) { //walk through all shift
        for (shift_y = -15; shift_y < 16; shift_y++) {
            sumI12 = 0, sumI1sq = 0, sumI2sq = 0;
            for (walk_x = 0; walk_x < img->width - abs(shift_x); walk_x++) { //walk through all pixels
                for (walk_y = 0; walk_y < img->height - abs(shift_y); walk_y++) {
                    I1 = getRGB(img, mode, walk_x + abs(min(shift_x, 0)), walk_y + abs(max(shift_y, 0)));
                    I2 = getRGB(img, 2, walk_x + abs(max(shift_x, 0)), walk_y + abs(min(shift_y, 0)));
                    sumI12 += I1 * I2; //continuous addition to each values
                    sumI1sq += I1 * I1;
                    sumI2sq += I2 * I2;
                }
            }
            eval = sumI12 / (sqrt(sumI1sq) * sqrt(sumI2sq)); //calc evaluation
            if (sft.evaluation < eval) { //if cur eval is greater, update
                sft.x = shift_x;
                sft.y = shift_y;
                sft.evaluation = eval;
            }
        }
    }
    return sft;
}