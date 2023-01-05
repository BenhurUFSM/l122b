// inclui as definicoes
#include "tela.h"
#include <stdio.h>

// Os includes do allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

void cai_fora(char *msg)
{
  printf("\n\nERRO\n%s\n\n", msg);
  exit(13);
}

// fila para receber os eventos do teclado
ALLEGRO_EVENT_QUEUE *tela_eventos_teclado;
void tela_inicializa_teclado(void)
{
  if (!al_install_keyboard()) cai_fora("problema na inicialização do teclado do allegro");
  // cria e inicializa a fila de eventos do teclado
  tela_eventos_teclado = al_create_event_queue();
  if (tela_eventos_teclado == NULL) cai_fora("problema na criação da fila de eventos do teclado do allegro");
  al_register_event_source(tela_eventos_teclado,
                           al_get_keyboard_event_source());
}

void tela_inicializa_janela(float l, float a, char n[])
{
  // pede para tentar linhas mais suaves (multisampling)
  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  // cria uma janela
  ALLEGRO_DISPLAY *janela;
  janela = al_create_display(l, a);
  if (janela == NULL) cai_fora("problema na criação de janela do allegro");
  // esconde o cursor do mouse
  al_hide_mouse_cursor(janela);
  al_set_window_title(janela, n);
}

// vetor com as cores
#define NCORES 100 // número máximo de cores diferentes
ALLEGRO_COLOR cores[NCORES];
int ncores_predef = 10;  // quantas cores são predefinidas
int prox_cor = 10; // numero da proxima cor definida pelo usuario
void tela_inicializa_cores(void)
{
  cores[transparente] = al_map_rgba_f(0, 0, 0, 0);
  cores[azul]         = al_map_rgb_f(0, 0, 1);
  cores[vermelho]     = al_map_rgb_f(1, 0, 0);
  cores[verde]        = al_map_rgb_f(0, 1, 0);
  cores[amarelo]      = al_map_rgb_f(1, 1, 0);
  cores[preto]        = al_map_rgb_f(0, 0, 0);
  cores[laranja]      = al_map_rgb_f(1, 0.65, 0);
  cores[rosa]         = al_map_rgb_f(1, 0, 0.5);
  cores[branco]       = al_map_rgb_f(1, 1, 1);
  cores[marrom]       = al_map_rgb_f(0.58, 0.29, 0);
}

int tela_cria_cor(float vm, float az, float vd)
{
  int cor = prox_cor;
  prox_cor = prox_cor+1;
  if (prox_cor >= NCORES) {
    prox_cor = ncores_predef;
  }
  cores[cor] = al_map_rgb_f(vm, az, vd);
  return cor;
}

void tela_inicio(int largura, int altura, char nome[])
{
  // inicializa os subsistemas do allegro
  if (!al_init()) cai_fora("problema na inicialização do allegro");
  if (!al_install_mouse()) cai_fora("problema na inicialização do mouse do allegro");
  if (!al_init_primitives_addon()) cai_fora("problema na inicialização de addons do allegro");
  al_init_font_addon();
  if (!al_init_ttf_addon()) cai_fora("problema na inicialização do addon de fontes ttf");

  // inicializa a tela
  tela_inicializa_janela(largura, altura, nome);
  tela_inicializa_teclado();
  tela_inicializa_cores();
}


void tela_fim(void)
{
  // badabum!
  al_uninstall_system();
}

double tempo_ultima_tela = 0;
void tela_atualiza(void)
{
  double agora = relogio();
  double quando_mostrar = tempo_ultima_tela + SEGUNDOS_POR_QUADRO;
  double tempo_ate_mostrar = quando_mostrar - agora;
  if (tempo_ate_mostrar > 0) {
    // é muito cedo, dá uma cochilada
    al_rest(tempo_ate_mostrar);
  }
  // troca a tela mostrada pela que foi desenhada em memória
  al_flip_display();
  tempo_ultima_tela = relogio();

  // limpa todo o canvas em memória, para desenhar a próxima tela
  al_clear_to_color(cores[preto]);
}


void tela_circulo(float x, float y, float r, float l, int corl, int corint)
{
  // preenche
  al_draw_filled_circle(x, y, r, cores[corint]);
  // faz o contorno
  al_draw_circle(x, y, r, cores[corl], l);
}

void tela_linha(float x1, float y1, float x2, float y2, float l, int corl)
{
  al_draw_line(x1, y1, x2, y2, cores[corl], l);
}

void tela_retangulo(float x1, float y1, float x2, float y2, float l,
                    int corl, int corint)
{
  al_draw_filled_rectangle(x1, y1, x2, y2, cores[corint]);
  al_draw_rectangle(x1, y1, x2, y2, cores[corl], l);
}

// tem que ter uma fonte para poder escrever
ALLEGRO_FONT *fonte = NULL;
int tamanho_das_letras = 0;

void tela_prepara_fonte(int tam)
{
  // se se quer o mesmo tamanho que antes, usa a mesma
  if (tam == tamanho_das_letras) return;

  // se ja tinha uma fonte carregada, livra-se dela antes de carregar outra
  if (fonte != NULL) {
    al_destroy_font(fonte);
  }

  // carrega uma fonte, para poder escrever na tela
  fonte = al_load_font("DejaVuSans.ttf", tam, 0);
  if (fonte == NULL) {
    al_uninstall_system();
    printf("\n\nERRO FATAL\n");
    printf("ARQUIVO QUE DEFINE DESENHO DAS LETRAS (DejaVuSans.ttf) NAO ENCONTRADO.\n"
           "COPIE ESSE ARQUIVO, OU MUDE tela.c PARA USAR UM ARQUIVO QUE EXISTA.\n\n");
    exit(1);
  }
  tamanho_das_letras = tam;
}

void tela_texto(float x, float y, int tam, int c, char t[])
{
  tela_prepara_fonte(tam);
  al_draw_text(fonte, cores[c], x, y-tam/2, ALLEGRO_ALIGN_CENTRE, t);
}

void tela_texto_esq(float x, float y, int tam, int c, char t[])
{
  tela_prepara_fonte(tam);
  al_draw_text(fonte, cores[c], x, y, ALLEGRO_ALIGN_RIGHT, t);
}

void tela_texto_dir(float x, float y, int tam, int c, char t[])
{
  tela_prepara_fonte(tam);
  al_draw_text(fonte, cores[c], x, y, ALLEGRO_ALIGN_LEFT, t);
}

float tela_rato_x(void)
{
  ALLEGRO_MOUSE_STATE rato;
  al_get_mouse_state(&rato);
  return al_get_mouse_state_axis(&rato, 0);
}

float tela_rato_y(void)
{
  ALLEGRO_MOUSE_STATE rato;
  al_get_mouse_state(&rato);
  return al_get_mouse_state_axis(&rato, 1);
}

bool tela_rato_apertado(void)
{
  ALLEGRO_MOUSE_STATE rato;
  al_get_mouse_state(&rato);
  // só nos interessa o botão da esquerda
  return al_mouse_button_down(&rato, 1);
}

bool tava_apertado = false;
float x_clicado = 0;
float y_clicado = 0;
bool tela_rato_clicado(void)
{
  bool ta_apertado = tela_rato_apertado();
  bool foi_clicado = (tava_apertado && !ta_apertado);
  if (foi_clicado) {
    x_clicado = tela_rato_x();
    y_clicado = tela_rato_y();
  }
  tava_apertado = ta_apertado;
  return foi_clicado;
}

float tela_rato_x_clique(void)
{
  return x_clicado;
}

float tela_rato_y_clique(void)
{
  return y_clicado;
}

char tela_tecla(void)
{
  ALLEGRO_EVENT ev;

  while (al_get_next_event(tela_eventos_teclado, &ev)) {
    if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
      int c = ev.keyboard.unichar;
      if (c == '\r') c = '\n'; // corrige windowscentrismo
      // só retorna caracteres imprimíveis, backspace e enter
      if ((c >= ' ' && c <= '~') || c == '\b' || c == '\n') {
        return (char) c;
      }
    }
  }
  // nada foi pressionado (ou foi pressionado algo não imprimível)
  return '\0';
}

double relogio(void)
{
  return al_get_time();
}

