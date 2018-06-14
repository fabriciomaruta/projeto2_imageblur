#include <imageprocessing.h>
#include <blur.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>


#define WORKERS 3

typedef struct {
  int altura;
  int largura;
  int radio;
  double **mat;
}argumentos;
void* worker(void *args){
  double ** saida;
  argumentos *pass = (argumentos *)args;
  saida = blur(pass->mat, pass->altura,pass->largura,pass->radio);
  return saida;
}


int main(int argc, char *argv[]) {
  imagem img;
  imagem out;
  clock_t t0, t1;
  struct timeval rt0, rt1, drt; /* Tempo baseada em tempo real */
  pthread_t thread[3];
  argumentos image1;
  argumentos image2;
  argumentos image3;
  int convolucao;
  img = abrir_imagem(argv[1]);
  out = abrir_imagem(argv[1]);

  image1.mat = img.r;
  image1.altura = img.width;
  image1.largura = img.height;
  image1.radio = 15;
  image2.mat = img.g;
  image2.altura = img.width;
  image2.largura = img.height;
  image2.radio = 15;
  image3.mat = img.b;
  image3.altura = img.width;
  image3.largura = img.height;
  image3.radio = 15;
  convolucao = image3.radio;

  t0=clock();
  gettimeofday(&rt0, NULL);
  unsigned int tmp;
  pthread_create(&(thread[0]), NULL, worker,(void *)&image1);
  pthread_create(&(thread[1]), NULL, worker,(void *)&image2);
  pthread_create(&(thread[2]), NULL, worker,(void *)&image3);

  pthread_join(thread[0], (void*)&out.r);
  pthread_join(thread[1], (void*)&out.g);
  pthread_join(thread[2], (void*)&out.b);

  t1=clock();
  gettimeofday(&rt1, NULL);
    timersub(&rt1, &rt0, &drt);
  printf("Nome do arquivo %s\n", argv[1]);
  printf("Estratégia: threads, 3 threads  \n");
  printf("Matriz de convolucao: %dx%d\n", convolucao, convolucao );
  printf("Tempo para aplicar filtro: %f milissegundos\n", 1000*(double)(t1-t0)/CLOCKS_PER_SEC);
  printf("Tempo real: %ld.%06ld segundos\n", drt.tv_sec, drt.tv_usec);
  printf("\n");
  salvar_imagem(argv[2], &out);
  liberar_imagem(&img);
  liberar_imagem(&out);
  return 0;
}
