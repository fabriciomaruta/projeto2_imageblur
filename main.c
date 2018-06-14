#include <imageprocessing.h>
#include <blur.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) {
  imagem img;
  imagem out;
  clock_t t0, t1;
  struct timeval rt0, rt1, drt; /* Tempo baseada em tempo real */
  int convolucao;

  img = abrir_imagem(argv[1]);
  out = abrir_imagem(argv[1]);



  unsigned int tmp;
  gettimeofday(&rt0, NULL);
  t0 = clock();
  out.r = blur(img.r, img.width ,img.height , 15);
  out.g = blur(img.g, img.width ,img.height , 15);
  out.b = blur(img.b, img.width ,img.height , 15);
  convolucao = 15;
  t1 = clock();
  gettimeofday(&rt1, NULL);
    timersub(&rt1, &rt0, &drt);
  printf("Nome do arquivo %s\n", argv[1]);
  printf("Nome do arquivo %s\n", argv[1]);
  printf("Estratégia: single thread  \n");
  printf("Matriz de convolucao: %dx%d\n", convolucao, convolucao );
  printf("Tempo para aplicar filtro: %f milissegundos\n", 1000*(double)(t1-t0)/CLOCKS_PER_SEC);
  printf("Tempo real: %ld.%06ld segundos\n", drt.tv_sec, drt.tv_usec);
  printf("\n");
  salvar_imagem(argv[2], &out);
  liberar_imagem(&img);
  liberar_imagem(&out);
  return 0;
}
