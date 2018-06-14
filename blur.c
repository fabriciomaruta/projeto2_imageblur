#include<stdio.h>
#include<stdlib.h>
#include "blur.h"
/*
int main(){
  double **m;
  m = malloc(5*sizeof(double));
  for(int k = 0; k< 5;k++){
    m[k] = malloc(5*sizeof(double));
  }
  double **saida;
  saida = malloc(5*sizeof(double));
  for(int k = 0; k< 5;k++){
    saida[k] = malloc(5*sizeof(double));
  }
  int b, i ,j;

  scanf("%d", &b);
  for( i = 0; i< 5;i++){
    for( j = 0; j < 5; j++){
      scanf("%lf", &m[i][j]);
    }
  }
  for(int i = 0; i< 5;i++){
    for(int j = 0; j < 5; j++){
      printf("%lf ",m[i][j]);
    }
    printf("\n");
  }
  saida = blur(m, i ,j , b);
  for(int i = 0; i< 5;i++){
    for(int j = 0; j < 5; j++){
      printf("%lf ",saida[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}
*/

double **blur(double **m, int linhas, int colunas, int radio){
  double **aux = malloc(linhas * sizeof(double));
  for(int i = 0; i< linhas;i++){
    aux[i] = malloc(colunas * sizeof(double));
  }
  //printf("oie");
  for(int i = 0; i<linhas;i++){
    for(int j = 0; j<colunas;j++){
      aux[i][j] = calcula_blur(m,i,j,radio, linhas, colunas);
    }
  }
  return aux;
}

double calcula_blur(double** m, int linhas, int colunas, int radio, int extline, int extcol){
  int lado1 = linhas - radio;
  int lado2 = linhas + radio;
  int lado3 = colunas - radio;
  int lado4 = colunas + radio;
  double soma = 0, contador = 0;
  //printf("%d %d\n", extline,extcol);
  if (lado1 < 0)
    lado1 = 0;
  if (lado2 >= extline)
    lado2 = extline-1;
  if (lado3 < 0)
    lado3 = 0;
  if (lado4 >= extcol)
    lado4 = extcol-1;

    //printf("%d %d %d %d\n", lado1, lado2, lado3, lado4);
  for(int i = lado1; i <= lado2; i++){
    for(int j = lado3 ; j <= lado4 ; j ++){

      soma += m[i][j];
      //printf("c:%f %f\n", soma, m[i][j]);
      contador ++;
    }
  }

  return (double)(soma/contador);
}
