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

int elevacao();

int main()
{
  int sinal_x, pos_x;
  char digits_str[20];
  char barran[2];
  barran[0] = '\n';
  barran[1] = '\0';

  int a, aux, i, j;
  short int b;

  Vector3 *vector, *uoli_pos;
  get_gyro_angles(vector);
  //achar_amigo(friends_locations[0].x, friends_locations[0].z);
  get_current_GPS_position(uoli_pos);
  pos_x = friends_locations[0].x;

  sinal_x = pos_x - uoli_pos->x; 

  sinal_x = pos_x - uoli_pos->x; 
    if (sinal_x > 5) 
    {
      puts("positivo");
      puts(barran);
      alinha_angulo(90);
    }
    else if (sinal_x < -5)
    {
      
      alinha_angulo(90);
      puts("negativo");
      puts(barran);
    }
    set_torque(20, 20);
    puts("anda");
    puts(barran);
    while (get_us_distance() > 100 && !elevacao()) 
    {
      puts("continua");
      puts(barran);
      continue;
    }
    set_torque(0,0);
    puts("n anda");
    puts(barran);
    get_time(i);
    while (get_time(j) - i < 10) 
    {
      continue;
    }

  while (1) {
    continue;
  }

  return 0;
}

void alinha_angulo(int angulo) {
  Vector3 *aux;
  get_gyro_angles(aux);
  set_torque(20, -20);
  while (aux->y < angulo - 5 || aux->y > angulo + 5) {
    get_gyro_angles(aux);
  }
  set_torque(0,0);    
  return;
}

//retorna a distancia ao quadrado entre dois pontos
int get_distance_squared(int pos1_x, int pos1_z, int pos2_x, int pos2_z) {
  int aux1, aux2;
  aux1 = pos1_x - pos2_x;
  aux2 = pos1_z - pos2_z;
  return ((aux1 * aux1) + (aux2 * aux2));
}

void achar_amigo(int pos_x, int pos_z) {
  int i, j;
  char digits_str[20];
  char barran[2];
  int sinal_x, sinal_y;
  barran[0] = '\n';
  barran[1] = '\0';
  Vector3 *uoli_pos;

  get_current_GPS_position(uoli_pos);

  while (get_distance_squared(pos_x, pos_z, uoli_pos->x, uoli_pos->z) > 25) 
  {
    sinal_x = pos_x - uoli_pos->x; 
    if (sinal_x > 5) 
    {
      puts("positivo");
      alinha_angulo(90);
    }
    else if (sinal_x < -5)
    {
      
      alinha_angulo(270);
      puts("negativo");
    }
    set_torque(20, 20);
    while (get_us_distance() < 100 && !elevacao()) 
    {
      continue;
    }
    set_torque(0,0);
    get_time(i);
    while (get_time(j) - i < 10) 
    {
      continue;
    }
    return;
  }
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

//retorna 1 se o uoli esta em um morro, 0 caso contrario
int elevacao() {
  char barran[2];
  barran[0] = '\n';
  barran[1] = '\0';
  char digits_str[20];
  int ret = 0;
  Vector3 *aux;
  get_gyro_angles(aux);
  if ((aux->x > 20 && aux->x < 355) || (aux->z > 20 && aux->z < 355)) {
    puts(IntToString(aux->x + 1, digits_str));
    puts(barran);
    puts(IntToString(aux->z, digits_str));
    puts(barran);
    ret = 1;
  }
  return ret;
}