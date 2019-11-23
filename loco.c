#include "api_robot.h"

int set_torque(int engine_1, int engine_2);

int set_engine_torque(int engine_id, int torque);

int set_head_servo(int servo_id, int angle);

unsigned short get_us_distance();

void get_current_GPS_position(Vector3* pos);

void get_gyro_angles(Vector3* angles);

unsigned int get_time();

void set_time(unsigned int t);

void puts(const char*);

void alinha_angulo(int sinal);

void achar_amigo(int pos_x, int pos_z);

int get_distance_squared(int pos1_x, int pos1_z, int pos2_x, int pos2_z);

char *IntToString(int x, char ret[]);

int tamanhoNumero(int x);

void freiar(int torque1, int torque2);

int elevacao();

int main()
{ 
  /* variáveis para escrita */
  char digits_str[20];
  char barran[2];
  barran[0] = '\n';
  barran[1] = '\0';

  /* nao sei  oq fazem aqui */
  int a, aux, i, j;
  short int b;

  Vector3 *vector;
  get_gyro_angles(vector);
  
  i = set_head_servo(0, 27);
  /* acha todos os amigos no vetor de amigos */
  
  achar_amigo(friends_locations[0].x, friends_locations[0].z);
  
  while (1) {
    continue;
  }

  return 0;
}

/*parametros: 1 (angulo para alinhamento)*/
void alinha_angulo(int angulo) {
  /*o angulo para o eixo x positivo e 90 - gira o uoli até chegar no angulo desejado */
  Vector3 *aux;
  get_gyro_angles(aux);
  set_torque(20, -20);
  while (aux->y < angulo - 5 || aux->y > angulo + 5) {
    get_gyro_angles(aux);
  }
  set_torque(0,0);
  return;
}

int get_distance_squared(int pos1_x, int pos1_z, int pos2_x, int pos2_z) {
  int aux1, aux2, ret;
  aux1 = pos1_x - pos2_x;
  aux2 = pos1_z - pos2_z;
  
  char digits_str[20];
  char barran[2];
  barran[0] = '\n';
  barran[1] = '\0';

  aux1 = aux1 / 10;
  aux2 = aux2 / 10;

  puts(IntToString(aux1, digits_str));
  puts(barran);
  puts(IntToString(aux2, digits_str));
  puts(barran);

  aux1 = aux1 * aux1;
  aux2 = aux2 * aux2;
  ret = aux1 + aux2;
  return ret;
}

void achar_amigo(int pos_x, int pos_z) {

  /* variáveis de escrita */
  char digits_str[20];
  char barran[2];
  barran[0] = '\n';
  barran[1] = '\0';

  int sinal_x, sinal_z;
  int i, j;

  /* struct de posicao */
  Vector3 *uoli_pos;

  get_current_GPS_position(uoli_pos);

  while (get_distance_squared(pos_x, uoli_pos->x, pos_z, uoli_pos->z) > 25)
  {
    puts("tentativa");
    puts(barran);
    get_current_GPS_position(uoli_pos); /* pega posicao atual do ouli */

    sinal_x = pos_x - uoli_pos->x; 

    if (sinal_x > 5) 
    {
      puts("positivo_x");
      puts(barran);
      alinha_angulo(90);
    }

    else if (sinal_x < -5)
    {
      alinha_angulo(270);
      puts("negativo_x");
      puts(barran);
    }

    set_torque(8, 8);
    puts("anda");
    puts(barran);

    i = get_us_distance(); 
    while ((i == -1) && (!elevacao())) 
    {
      i = get_us_distance();
      puts("continua");
      puts(barran);
      continue;
    }

    i = get_time();
    j = get_time();
    if (elevacao()) 
    {
      puts("ingrime");
      freiar(50, 50);
      while (j - i < 50) 
      {
      j = get_time();
      continue;
      }
    }
    else
    {
      freiar(50, 50);
      puts("n anda");
      puts(barran);
      i = get_time();
      j = get_time();
      while (j - i < 10) 
      {
      j = get_time();
      continue;
      }
    }

    sinal_z = pos_z - uoli_pos->z; 

    if (sinal_z > 5) 
    {
      puts("positivo_z");
      puts(barran);
      alinha_angulo(0);
    }

    else if (sinal_z < -5)
    {
      alinha_angulo(180);
      puts("negativo_z");
      puts(barran);
    }

    set_torque(8, 8);
    puts("anda");
    puts(barran);

    i = get_us_distance(); 
    while ((i == -1) && (!elevacao())) 
    {
      i = get_us_distance();
      puts("continua");
      puts(barran);
      continue;
    }
    i = get_time();
    j = get_time();

    if (elevacao()) 
    {
      freiar(50, 50);
      puts("ingrime");
      while (j - i < 50) 
      {
      j = get_time();
      continue;
      }
    }

    else
    {
      freiar(50, 50);
      puts("n anda");
      puts(barran);
      while (j - i < 10) 
      {
      j = get_time();
      continue;
      }
    }
  }
  puts("achei tt");
  return;
}

int tamanhoNumero(int x)
{
   int aux = 0;
   if(x < 0)
       aux = 1;

   while(x != 0)
   {
     aux++;
     x /= 10;
   }
   return aux;
}

/*retorna a conversao do inteiro em string*/
char *IntToString(int x, char ret[])
{
  /*ve se o numero é negativo*/
  int tam = tamanhoNumero(x);
  if(x < 0)
  {
    ret[0] = '-';
    x *= -1;
  }

  /*converte cada casa do numero em char*/
  ret[tam] = '\0';

  while(x != 0)
  {
    tam--;
    ret[tam] = 48 + x%10;
    x /= 10;
  }

  return ret;
}

void freiar(int torque1, int torque2)
{ 
  /* enquanto a velocidade for diferente de 0 */
  /* seta torque contrário até que ela seja 0 */
  /* a velocidade sendo 0, zera o torque */

  /* pega cordenadas antes dos 100 ms */
  Vector3 *cord;
  get_current_GPS_position(cord);

  /* obtem tempo */
  unsigned int tempo1 = get_time();
  unsigned int tempo2 = get_time();

  /* pega tempo após 3 ms */
  while(tempo2 - tempo1 != 3)
  {
    tempo2 = get_time();
  }

  /* pega cordenada depois de 3 ms */
  Vector3 *cord2;
  get_current_GPS_position(cord2);


  /* obtem velocidade */
  int vx = (cord2->x - cord->x)/(tempo2-tempo1);
  int vz = (cord2->z - cord->z)/(tempo2-tempo1);
  set_torque((-1)*torque1,(-1)*torque2);

  while(vx != 0 && vz != 0)
  {
    tempo1 = get_time();
    tempo2 = get_time();

    /* pega cordenadas antes dos 3 ms */
    get_current_GPS_position(cord);

    /* pega tempo após 3 ms */
    while(tempo2 - tempo1 != 3)
    {
      tempo2 = get_time();
    }

    /* pega cordenada depois de 3 ms */
    get_current_GPS_position(cord2);

    /* pega velocidade após 3 ms */
    vx = (cord2->x - cord->x)/(tempo2-tempo1);
    vz = (cord2->z - cord->z)/(tempo2-tempo1);
  }

  /* chegou aqui, a velocidade é zero: zera o torque */
  set_torque(0,0);
}

//retorna 1 se o uoli esta em um morro, 0 caso contrario
int elevacao() {
  char barran[2];
  barran[0] = '\n';
  barran[1] = '\0';
  char digits_str[20];
  int ret = 0;
  Vector3 *aux;
  get_gyro_angles(aux);
  if ((aux->x > 10 && aux->x < 355) || (aux->z > 10 && aux->z < 355)) {
    ret = 1;
  }
  return ret;
}

