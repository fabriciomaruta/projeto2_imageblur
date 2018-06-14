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
#include <sys/time.h>

typedef struct {
  int altura;
  int largura;
  int radio;
  double **mat;
}argumentos;

void verifica_filho(int width, int height, int radio, double **matrix, double *buffer_compartilhado);
double *disassemble_matrix(int width, int height, double **matrix);
double **assemble_matrix(int width, int height, double *buffer);

int main(int argc, char *argv[]) {
  pid_t filho[3];
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;
  int ARRAY_SIZE;
  clock_t t0, t1;
  int convolucao;

  double *blue;
  double *red;
  double *green;

  imagem img;
  imagem out;
  argumentos image1;
  argumentos image2;
  argumentos image3;
  struct timeval rt0, rt1, drt; /* Tempo baseada em tempo real */
  img = abrir_imagem(argv[1]);
  out = abrir_imagem(argv[1]);

  ARRAY_SIZE = img.width*img.height;

  blue = (double *) mmap(NULL, sizeof(double)*ARRAY_SIZE, protection, visibility, 0, 0);
  red = (double *) mmap(NULL, sizeof(double)*ARRAY_SIZE, protection, visibility, 0, 0);
  green = (double *) mmap(NULL, sizeof(double)*ARRAY_SIZE, protection, visibility, 0, 0);

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
  convolucao = image3.radio ;

  t0=clock();
  gettimeofday(&rt0, NULL);
  filho[0] = fork();
  if(filho[0] == 0)
    verifica_filho(image1.largura, image1.altura, image1.radio, image1.mat, red);

  filho[1] = fork();
  if(filho[1] == 0)
   verifica_filho(image2.largura, image2.altura, image2.radio, image2.mat, green);

  filho[2] = fork();
  if(filho[2] == 0)
    verifica_filho(image3.largura, image3.altura, image3.radio, image3.mat, blue);

  int status;
  waitpid(filho[0], &status, 0);
  waitpid(filho[1], &status, 0);
  waitpid(filho[2], &status, 0);

  t1=clock();
  gettimeofday(&rt1, NULL);
    timersub(&rt1, &rt0, &drt);
  printf("Nome do arquivo %s\n", argv[1]);
  printf("\nMatrix de convolucao: %dx%d\n", convolucao, convolucao );
  printf("Estratégia: processos, 3 processos\n");
  printf("Tempo para aplicar filtro: %f milissegundos\n", 1000*(double)(t1-t0)/CLOCKS_PER_SEC);
  printf("Tempo real: %ld.%06ld segundos\n", drt.tv_sec, drt.tv_usec);
  out.r = assemble_matrix(image1.largura, image1.altura, red);
  out.g = assemble_matrix(image2.largura, image2.altura, green);
  out.b = assemble_matrix(image3.largura, image3.altura, blue);
  printf("\n");
  salvar_imagem(argv[2], &out);
  liberar_imagem(&img);
  liberar_imagem(&out);
  return 0;
}

void verifica_filho(int width, int height, int radio, double **matrix, double *buffer_compartilhado){
  double *vetor_aux = malloc(width*height * sizeof(double));;
  double **saida = malloc(height * sizeof(double));
  for(int i = 0; i<height;i++){
    saida[i] = malloc(width * sizeof(double));
  }
  saida = blur(matrix, height, width, radio);
  vetor_aux = disassemble_matrix(width,height, saida);
  for(int i=0;i<width*height;i++)
  	buffer_compartilhado[i] = vetor_aux[i];
  exit(0);
}

double *disassemble_matrix(int width, int height, double **matrix){
  int i, j, h=0;
  double *array = malloc(width*height * sizeof(double));
  for ( i=0; i<height; i++ ){
    for ( j=0; j<width; j++ ){
       array[h] = matrix[i][j];
       h++;
    }
  }
  return array;
}

double **assemble_matrix(int width, int height, double *buffer){
  int i, j, h = 0;
  double **matriz = malloc(height * sizeof(double));
  for(i = 0; i<height;i++){
    matriz[i] = malloc(width * sizeof(double));
  }

  for ( i=0; i<height; i++ ){
    for ( j=0; j<width; j++ ){
       matriz[i][j] = buffer[h];
       h++;
    }
  }
  return matriz;
}
