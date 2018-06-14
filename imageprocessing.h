
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H


typedef struct {
   int width, height;
  double **r, **g, **b;
} imagem;

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I);
void liberar_imagem(imagem *i);

#endif