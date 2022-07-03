#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

int exists(int i, int j, int iw, int ih) {
    if ((i >= 1) && (i <= ih - 1) && (j >= 1) && (j <= iw - 1))
        return 1;
    return 0;
}

void dfs (int i, int j, int iw, int ih, unsigned char* dip, int* v, int col_numer) {
    v[iw * i + j] = col_numer;


    if (exists (i, j - 2, iw, ih) == 1) {
        if ((abs(dip[iw * i + j] - dip[iw * (i) + (j - 2)]) <= 58) && (v[iw * (i) + (j - 2)] == 0)){
            dfs(i, j - 2, iw, ih, dip, v, col_numer);
        }
    }


    if (exists(i - 2, j + 1, iw, ih) == 1) {
        if((abs(dip[iw * i + j] - dip[iw * (i - 2) + (j + 1)]) <= 58) && (v[iw * (i - 2) + (j + 1)] == 0)) {
            dfs(i - 2, j + 1, iw, ih, dip, v, col_numer);
        }
    }


    if(exists(i + 2, j + 1, iw, ih) == 1) {
        if((abs(dip[iw * i + j] - dip[iw * (i + 2) + (j + 1)]) <= 58) && (v[iw * (i + 2) + (j + 1)] == 0)) {
            dfs(i + 2, j + 1, iw, ih, dip, v, col_numer);
        }
    }


    return;
}


int main() {

    char* inham = "hamster.png";
    int iw, ih, n, i, j, k = 0, col_numer = 0;


    unsigned char *idata = stbi_load (inham, &iw, &ih, &n, 0);


    unsigned char* dip = (unsigned char*)malloc(iw * ih * sizeof(unsigned char));
    for (i = 0; i < ih * iw * n; i = i + n){
        dip[k] = 0.35 * idata[i] + 0.49 * idata[i+1] + 0.16 * idata[i+2];
        k++;
    }

    for (i = 2; i <= ih - 2; i++ ){
        for (j = 2; j <= iw - 2; j++) {
            if(dip[iw * i + j] > 149)
                dip[iw * i + j] = 240;
            if(dip[iw * i + j] < 92)
                dip[iw * i + j] = 0;
        }
    }



    int* v = (int*)malloc((iw*ih) * sizeof(int));
    for (i = 0; i < iw * ih; i++)
        v[i] = 0;


    for (i = 1; i <= ih - 1; i++) {
        for (j = 1; j <= iw - 1; j++) {
            if(v[iw * i + j] == 0) {
                col_numer++;
                dfs (i, j, iw, ih, dip, v, col_numer);
            }
        }
    }


    unsigned char* odata = (unsigned char*)malloc(iw * ih * n * sizeof(unsigned char));
    k = 0;
    int kf;
    for (i = 0; i < ih * iw * n; i = i + n) {
        kf = v[k] % 50 + v[k] % 12;
        odata[i] = 3 * kf - 67;
        odata[i+1] = 3 * kf + 24;
        odata[i+2] = 7 * kf + 56;
        odata[i+3] = 255;
        k++;
    }


    char* outham = "output.png";

    stbi_write_png(outham, iw, ih, n, odata, 0);
    stbi_image_free(idata);
    stbi_image_free(dip);
    stbi_image_free(odata);
    return 0;
}
