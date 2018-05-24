#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
using namespace std;

#pragma pack(push, 2)
typedef struct {
     int8_t id[2];            // Завжди дві літери 'B' і 'M'
     int32_t filesize;        // Розмір файла в байтах
     int16_t reserved[2];     // 0, 0
     int32_t headersize;      // 54L для 24-бітних зображень
     int32_t infoSize;        // 40L для 24-бітних зображень
     int32_t width;           // ширина зображення в пікселях
     int32_t depth;           // висота зображення в пікселях
     int16_t biPlanes;        // 1 (для 24-бітних зображень)
     int16_t bits;            // 24 (для 24-бітних зображень)
     int32_t biCompression;   // 0L
     int32_t biSizeImage;     // Можна поставити в 0L для зображень без компрессії (наш варіант)
     int32_t biXPelsPerMeter; // Рекомендована кількість пікселів на метр, можна 0L
     int32_t biYPelsPerMeter; // Те саме, по висоті
     int32_t biClrUsed;       // Для індексованих зображень, можна поставити 0L
     int32_t biClrImportant;  // Те саме
} BMPHEAD;

typedef struct {
     int8_t redComponent;
     int8_t greenComponent;
     int8_t blueComponent;
} PIXELDATA;
#pragma pack(pop)

bool readBMP(FILE *inputFile, BMPHEAD &myBMPHead, vector<vector<PIXELDATA>> &pixels){
    int8_t nullByte = 0;
    fread(&myBMPHead, sizeof(myBMPHead), 1, inputFile);
    if (myBMPHead.id[0] != 'B' || myBMPHead.id[1] != 'M' || myBMPHead.bits != 24){
        cout << "File is corrupted.";
        return false;
    }
    pixels.resize(myBMPHead.depth);
    int plusToWidth = 4 - (myBMPHead.width * 3) % 4;
    if (myBMPHead.width % 4 == 0){
        plusToWidth = 0;
    }
    for (int i = 0; i < myBMPHead.depth; i++){
        pixels[i].resize(myBMPHead.width);
        for (int j = 0; j < myBMPHead.width; j++){
            fread(&pixels[i][j], sizeof(pixels[i][j]), 1, inputFile);
        }
        fread(&nullByte, 1, plusToWidth, inputFile);
    }
    return true;
}

PIXELDATA interpolize(double x, double y, double stepW, double stepD, int maxWidth, int maxDepth, vector<vector<PIXELDATA>> &pixels){
    double eps = 0.00001;
    int x1 = floor(x - stepD / 2 + eps), y1 = floor(y - stepW / 2 + eps), x2 = floor(x + stepD / 2 + eps), y2 = floor(y + stepW / 2 + eps);
    if (x2 >= maxDepth || (x2 > 0 && int(floor(x + stepD / 2 + eps)) == int(ceil(x + stepD / 2 - eps)))){
        x2--;
    }
    if (y2 >= maxWidth || (y2 > 0 && int(floor(y + stepW / 2 + eps)) == int(ceil(y + stepW / 2 - eps)))){
        y2--;
    }
    int8_t newRed = 255 - ((255 - pixels[x1][y1].redComponent + 255 - pixels[x2][y1].redComponent) / 2 + (255 - pixels[x1][y2].redComponent + 255 - pixels[x2][y2].redComponent) / 2) / 2;
    int8_t newGreen = 255 - ((255 - pixels[x1][y1].greenComponent + 255 - pixels[x2][y1].greenComponent) / 2 + (255 - pixels[x1][y2].greenComponent + 255 - pixels[x2][y2].greenComponent) / 2) / 2;
    int8_t newBlue = 255 - ((255 - pixels[x1][y1].blueComponent + 255 - pixels[x2][y1].blueComponent) / 2 + (255 - pixels[x1][y2].blueComponent + 255 - pixels[x2][y2].blueComponent) / 2) / 2;
    int minDifRed = min(min(abs(newRed - pixels[x1][y1].redComponent), abs(newRed - pixels[x2][y1].redComponent)), min(abs(newRed - pixels[x1][y2].redComponent), abs(newRed - pixels[x2][y2].redComponent)));
    int minDifGreen = min(min(abs(newGreen - pixels[x1][y1].greenComponent), abs(newGreen - pixels[x2][y1].greenComponent)), min(abs(newGreen - pixels[x1][y2].greenComponent), abs(newGreen - pixels[x2][y2].greenComponent)));
    int minDifBlue = min(min(abs(newBlue - pixels[x1][y1].blueComponent), abs(newBlue - pixels[x2][y1].blueComponent)), min(abs(newBlue - pixels[x1][y2].blueComponent), abs(newBlue - pixels[x2][y2].blueComponent)));
    if (minDifRed + minDifGreen + minDifBlue > 20){
        int minX, minY;
        if (x - x1 < x2 - x1){
            minX = x1;
        } else {
            minX = x2;
        }
        if (y - y1 < y2 - y1){
            minY = y1;
        } else {
            minY = y2;
        }
        newRed = pixels[minX][minY].redComponent;
        newGreen = pixels[minX][minY].greenComponent;
        newBlue = pixels[minX][minY].blueComponent;
    }
    PIXELDATA newPixel = {newRed, newGreen, newBlue};
    return newPixel;
}

void scaleBMP(FILE *outputFile, BMPHEAD &myBMPHead, vector<vector<PIXELDATA>> &pixels, double scale){
    int8_t nullByte = 0;
    int prevWidth = myBMPHead.width, prevDepth = myBMPHead.depth;
    myBMPHead.width = int(abs(myBMPHead.width * scale));
    myBMPHead.depth = int(abs(myBMPHead.depth * scale));
    double stepW = prevWidth / double(abs(myBMPHead.width)), stepD = prevDepth / double(abs(myBMPHead.depth));
    int plusToWidth = 4 - (int(abs(myBMPHead.width)) * 3) % 4;
    if (int(abs(myBMPHead.width)) % 4 == 0){
        plusToWidth = 0;
    }
    myBMPHead.filesize = abs(myBMPHead.depth) * (abs(myBMPHead.width) + plusToWidth) * scale + 54;
    fwrite(&myBMPHead, sizeof(myBMPHead), 1, outputFile);
    int sign = scale / abs(scale);
    double x = prevDepth * (sign < 0) + sign * stepD / 2;
    for (int i = 0; i < abs(myBMPHead.depth); i++){
        double y = prevWidth * (sign < 0) + sign * stepW / 2;
        for (int j = 0; j < abs(myBMPHead.width); j++){
            PIXELDATA newPixel = interpolize(x, y, stepW, stepD, prevWidth, prevDepth, pixels);
            fwrite(&newPixel, sizeof(newPixel), 1, outputFile);
            y += sign * stepW;
        }
        x += sign * stepD;
        if (plusToWidth > 0){
            fwrite(&nullByte, 1, plusToWidth, outputFile);
        }
    }
}

int main(int argc, char* argv[]){
    BMPHEAD myBMPHead;
    vector<vector<PIXELDATA>> pixels;
    vector<string> filesList = {"bmp.bmp", "BillieJoeForLab.bmp", "test.bmp"};
    string inputFilePath;
    double scale;
    if (argc > 1){
        if (argc > 3){
            cout << "Too many arguments." << endl;
            return 0;
        }
        if (argc < 3){
            cout << "Too few arguments." << endl;
            return 0;
        }
        inputFilePath = argv[1];
        scale = 0;
        bool point = false;
        double aftrPnt = 0.1;
        int sign = 1;
        for (char* ptr = argv[2]; *ptr != '\0'; ptr++){
            if (*ptr == '-'){
                sign = -1;
                continue;
            }
            if (*ptr == '.'){
                point = true;
                continue;
            }
            if (point){
                scale += (*ptr - '0') * aftrPnt;
                aftrPnt *= 0.1;
            } else {
                scale = scale * 10 + (*ptr - '0');
            }
        }
        scale *= sign;

    } else {
        cout << "Would you like to choose file from the list? y/n" << endl;
        char choosed;
        cin >> choosed;
        if (choosed == 'y'){
            int numOfFile = 0;
            cout << "Choose your file." << endl;
            for (int i = 0; i < filesList.size(); i++){
                cout << i + 1 << ". " << filesList[i] << endl;
            }
            cin >> numOfFile;
            if (numOfFile < 1 || numOfFile > 3){
                cout << "Wrong number of file." << endl;
                return 0;
            } else {
                inputFilePath = filesList[numOfFile - 1];
            }
        } else {
            cout << "Enter file path." << endl;
            cin >> inputFilePath;
        }
        cout << "Enter scale." << endl;
        cin >> scale;
    }
    FILE *inputFile = fopen(&inputFilePath[0], "rb"), *outputFile = fopen("out.bmp", "wb");
    if(!inputFile){
        cout << "Can't open file.";
        return 0;
    }
    if (!readBMP(inputFile, myBMPHead, pixels)){
        return 0;
    }
    scaleBMP(outputFile, myBMPHead, pixels, scale);
    fclose(inputFile);
    fclose(outputFile);
}
