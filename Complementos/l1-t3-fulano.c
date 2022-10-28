// T3 - altere este comentário para ter teu nome

#include <stdio.h>

// Implemente uma função que calcula x^y, usando exponenciação e logaritmo.
//
// Sabe-se que e^(ln(x)) é igual a x
// Sabe-se também que ln(x^y) é igual a y×ln(x)
// Então, pode-se calcular x^y como e^(y×ln(x))
// e^x pode ser calculado usando uma série de Taylor
// ln(x) também, com alguns percalços (que estão resolvidos abaixo)
//
// Você deve implementar as funções que faltam abaixo.


// função que calcula e^x, recebendo x como argumento
// deve usar a série de Taylor (https://en.wikipedia.org/wiki/Taylor_series):
//   e^x = 1 + x + x²/2! + x³/3! + ···
// pode usar as funções fat() e xnan() vistas em aula (ou implementá-las de novo)
// repetir até que o termo somado seja inferior a 1e-7  (também conhecido
//   como 0.0000001)
double enax(double x)
{
  return 0;
}

// função que calcula o logaritmo natural (base e) de 1+x, recebendo x 
//   como argumento
// deve usar a série de Taylor (https://en.wikipedia.org/wiki/Taylor_series):
//   ln(x+1) = x - x²/2 + x³/3 - ···
// repetir até que o termo somado seja inferior a 1e-7  (também conhecido
//   como 0.0000001)
double ln_xmais1(double x)
{
  return 0;
}

// função que calcula o logaritmo natural do número recebido
//   (já tá implementada, não precisa mexer)
double ln(double x)
{
  // Deveria ser suficiente chamar ln_xmais1(x-1), mas a série de Taylor que calcula
  //   ln(x+1) só é precisa para valores de x próximos de zero.
  // Para resolver o problema, vamos transformar x em y×2^n, de forma que:
  //   ln(x) = ln(y×2^n)
  //         = ln(y) + ln(2^n)
  //         = ln(y) + n×ln(2)
  //         = ln_xmais1(y-1) + n×ln_xmais1(2-1)
  // Se y tiver valores próximos de 1, y-1 terá valores próximos de 0.
  // Entretanto, para ln(2), o valor de 2-1 não é suficientemente próximo de 0.
  // Com mais uma gambiarra, dá para resolver:
  //   ln(2) = ln(1.5 * 2/1.5)
  //         = ln(1.5) + ln(2/1.5)
  //         = ln(1+0.5) + ln(1+1/3)
  //         = ln_xmais1(0.5) + ln_xmais1(1/3)
  // Tanto 0.5 quanto 1/3 são suficientemente próximos de 0.
  int n=0;
  double y=x;
  // y × 2^n  é igual a x
  // divide y por 2 até que seja menor que 1.35
  while (y>1.35) {
    y/=2;
    n++;
  }
  // multiplica y por 2 até que seja maior que 0.65
  while (y<0.65) {
    y*=2;
    n--;
  }
  double ln2 = ln_xmais1(0.5)+ln_xmais1(1.0/3.0); 
  return ln_xmais1(y-1) + n*ln2;
}

// função que calcula x^y, recebendo x e y como argumento
// deve usar as funções enax() e ln()
double xnay(double x, double y)
{
  return 0;
}

int main(void)
{
  double a = 19.42;
  double rc = xnay(a, 1.0/3.0);
  double b = rc*rc*rc;
  printf("%lf deve ser parecido com %lf\n", a, b);
  return 0;
  // caso queira comparar os resultados com as funções oficiais, incluindo math.h:
  //   xnay é pow
  //   ln é log
  //   enax é exp
  //   ln_xmais1 é logp1
}
