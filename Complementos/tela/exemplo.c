// Programa de exemplo de uso de tela.h e tela.c
//
// Necessita os arquivos exemplo.c (este), tela.h, tela.c e DejaVuSans.ttf
// Necessita ter instalado a biblioteca allegro5
//
// para compilar este programa manualmente em linux:
// gcc -Wall -o exemplo exemplo.c tela.c -lallegro_font -lallegro_color -lallegro_ttf -lallegro_primitives -lallegro
//

#include "tela.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// A função principal
int main(void)
{
  // inicializa a tela gráfica
  tela_inicio(500, 500, "exemplo");

  // cria uma cor (como não tem essa nas pré-definidas?)
  int fucsia = tela_cria_cor(1, 0.2, 0.8);

  // laço principal
  while (true) {
    // desenha um quadrado no contorno da janela
    tela_retangulo(30, 30, 470, 470, 5, fucsia, transparente);

    // faz uma linha se o botão do mouse tiver apertado
    if (tela_rato_apertado()) {
      tela_linha(30, 30, tela_rato_x(), tela_rato_y(), 2, branco);
    }

    // desenha um círculo na posição do mouse
    tela_circulo(tela_rato_x(), tela_rato_y(), 10, 2, vermelho, branco);

    // escreve o valor do relógio no canto da tela
    char txt[10];
    sprintf(txt, "%.1f", relogio());
    tela_texto_esq(450, 445, 20, azul, txt);

    // faz aparecer na tela o que foi desenhado
    tela_atualiza();

    // sai do laço quando algo for digitado no teclado
    if (tela_tecla() != '\0') {
      break;
    }
  }

  // encerra a tela gráfica
  tela_fim();

  return 0;
}

