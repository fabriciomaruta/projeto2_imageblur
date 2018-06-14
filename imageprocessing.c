
#include <stdlib.h>
#include <stdio.h>

#include "imageprocessing.h"

#include <FreeImage.h>

/*
imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo);
void liberar_imagem(imagem *i);
 */

imagem abrir_imagem(char *nome_do_arquivo) {
  FIBITMAP *bitmapIn;
  int x, y;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
  } else {
   // printf("Arquivo lido corretamente!\n");
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = malloc(sizeof(double*) * x);
  for(int i = 0; i< x;i++){
    I.r[i] = malloc(sizeof(double)*y);
  }
  I.g = malloc(sizeof(double*) * x);
  for(int i = 0; i< x;i++){
    I.g[i] = malloc(sizeof(double)*y);
  }
  I.b = malloc(sizeof(double*) * x);
  for(int i = 0; i< x;i++){
    I.b[i] = malloc(sizeof(double)*y);
  }
  //printf("trozobado");
   for (int i=0; i<x; i++) {
     for (int j=0; j <y; j++) {
      //int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);
      I.r[i][j] = color.rgbRed;
      I.g[i][j] = color.rgbGreen;
      I.b[i][j] = color.rgbBlue;
    }
   }
  return I;

}

void liberar_imagem(imagem *I) {
  free(I->r);
  free(I->g);
  free(I->b);
}

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  printf("Salvando imagem %d por %d...\n", I->width, I->height);
  bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);

   for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      //int idx;

      //idx = i + (j*I->width);
      color.rgbRed = I->r[i][j];
      color.rgbGreen = I->g[i][j];
      color.rgbBlue = I->b[i][j];

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
}
