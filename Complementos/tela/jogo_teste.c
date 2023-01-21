#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "tela.h"

// o tipo de dados "ponto" representa um ponto no espaço bidimensional.
// Um ponto é formado por dois valores float, as coordenadas "x" e "y".
typedef struct {
  float x;
  float y;
} ponto;

// um círculo é definido por um ponto (o centro) e um raio
typedef struct {
  ponto centro;
  float raio;
} circulo;

// o jogo tem alvos para o jogador acertar. eles são círculos
// coloridos, que morrem quando acertados e valem pontos, mas 
// podem conter uma bomba
typedef struct {
  circulo figura;
  int cor;
  bool vivo;
  int pontos;
  bool bomba;
} alvo;

// o estado do jogo
typedef struct {
  alvo alvos[25];  // são círculos que o jogador tem que acertar
  int n_vivos;        // quantos deles ainda estão vivos
  int pontos;         // quantos pontos o jogador tem
  bool levei_bomba;   // acertei um alvo com bomba?
  bool ativo;         // o jogo está ativo?
  bool terminou;      // o jogo ja terminou?
  double tempo;       // quando foi a ultima vez que algo aconteceu?
} jogo;

// função que recebe dois pontos e retorna a distância entre eles.
float dist_pt(ponto a, ponto b)
{
  float c1 = a.x - b.x;
  float c2 = a.y - b.y;
  return sqrt(c1*c1 + c2*c2);
}

// função que retorna a distância entre dois círculos
float dist_circulos(circulo c1, circulo c2)
{
  return dist_pt(c1.centro, c2.centro) - c1.raio - c2.raio;
}

// função que diz se dois círculos se encostam
bool colidem_circ(circulo c1, circulo c2)
{
  return dist_circulos(c1, c2) < 0;
}

// função que diz se um ponto está dentro de um círculo
bool ponto_no_circulo(ponto p, circulo c)
{
  return dist_pt(c.centro, p) < c.raio;
}

// função que retorna a posição no vetor de alvos onde está o ponto
// ou -1, caso o ponto não esteja em nenhum dos alvos
int alvo_no_ponto(int n, alvo alvos[n], ponto p)
{
  for (int i=0; i<n; i++) {
    if (ponto_no_circulo(p, alvos[i].figura)) {
      return i;
    }
  }
  return -1;
}

// --- funções auxiliares de inicialização

int sorteia_cor(void)
{
  int c;
  while (true) {
    c = rand() % 10;
    //if (c == transparente) continue;
    //if (c == preto) continue;
    break;
  }
  return c;
}

alvo cria_alvo(int x, int y)
{
	circulo c = {{x, y}, 10};
  alvo a;
  a.figura = c;
  a.cor = sorteia_cor();
  a.vivo = true;
  a.pontos = rand()%100; // entre 0 e 100
  a.bomba = (a.pontos == 0);  // 0 pontos é bomba!

	return a;
}

void init_jogo(jogo *j)
{
  for (int i=0; i<25; i++) {
    j->alvos[i] = cria_alvo(i/5*30+20, i%5*30+20);
  }
  j->n_vivos = 25;
  j->pontos = 0;
  j->levei_bomba = false;
  j->ativo = true;
  j->terminou = false;
}

// ------- funções para execução de uma partida

// muda o estado do jogo de acordo com as ações do jogador
void verif_entrada(jogo *j)
{
  if (j->ativo && tela_rato_clicado()) {
    ponto rato;
    rato.x = tela_rato_x_clique();
    rato.y = tela_rato_y_clique();
    int i = alvo_no_ponto(25, j->alvos, rato);
    if (i != -1 && j->alvos[i].vivo) {
      j->alvos[i].vivo = false;
      j->pontos += j->alvos[i].pontos;
      j->n_vivos--;
      j->levei_bomba = j->alvos[i].bomba;
    }
  }
  if (tela_tecla() == 'f') j->ativo = false;
}

// muda o estado do jogo independente das ações do jogador
void progride_jogo(jogo *j)
{
  double agora = relogio();
  if (j->ativo) {
    if (j->levei_bomba) {
      // perde metade dos pontos
      j->pontos /= 2;
      j->ativo = false;
    }
    if (j->n_vivos == 0) {
      j->ativo = false;
    }
    if (agora - j->tempo > 1) {
      // tá demorando demais, perde pontos
      j->pontos /= 1.5;
      j->tempo = agora;
    }
  }
  if (agora - j->tempo > 5) {
    j->terminou = true;
  }
}

// ------- desenho da tela

// desenha os alvos vivos do vetor de alvos
void desenha_alvos(int n, alvo a[n])
{
	for (int i=0; i<n; i++) {
		if (a[i].vivo) {
		  circulo c = a[i].figura;
      int cor = a[i].cor;
      tela_circulo(c.centro.x, c.centro.y, c.raio, 1, cor, cor);
		}
	}
}

// desenha os pontos e alvos restantes
void desenha_pontos(int v, int p)
{
  char txt[20];
  sprintf(txt, "v:%d pt:%d", v, p);
  tela_texto_esq(150, 170, 16, branco, txt);
}

// desenha a tela do jogo
void desenha_jogo(jogo *j)
{
  desenha_alvos(25, j->alvos);
  desenha_pontos(j->n_vivos, j->pontos);
  tela_circulo(tela_rato_x(), tela_rato_y(), 2, 1, vermelho, branco);
  if (j->levei_bomba) {
    tela_texto(85, 100, 20, vermelho, "BADABUM");
  }
  tela_atualiza();
}

// joga uma partida
void partida(void)
{
  jogo j;
  init_jogo(&j);
  while (!j.terminou) {
    verif_entrada(&j);
    progride_jogo(&j);
    desenha_jogo(&j);
  }
}

// ---------- programa principal

// retorna true se o usuário quiser jogass outra partida
bool quer_jogar_de_novo(void)
{
  double inicio = relogio();
  char tecla = 0;
  while (tecla == 0) {
    tecla = tela_tecla();
    double t = 10 - (relogio() - inicio);
    if (t <= 0) break;
    int cor = branco;
    if (t < 2) cor = vermelho;
    tela_texto(85, 90, 20, cor, "tecle S");
    char txt[10];
    sprintf(txt, "%.1f", t);
    tela_texto(85, 120, 20, cor, txt);
    tela_atualiza();
  }
  if (tecla == 's' || tecla == 'S') return true;
  return false;
}

int main()
{
  srand(time(0));
	tela_inicio(170, 200, "x");
  do {
    partida();
  } while (quer_jogar_de_novo());
  tela_fim();
  return 0;
}
