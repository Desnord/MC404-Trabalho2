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

int main()
{
  char digits_str[20];
  char barran[2];
  barran[0] = '\n';
  barran[1] = '\0';

  int a, aux, i;
  short int b;


  set_engine_torque(20,20);

  Vector3 *vector;
  get_gyro_angles(vector);
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
  int aux1, aux2;
  aux1 = pos1_x - pos2_x;
  aux2 = pos1_z - pos2_z;
  return ((aux1 * aux1) + (aux2 * aux2));
}

void achar_amigo(int pos_x, int pos_z) {
  char digits_str[20];
  char barran[2];
  barran[0] = '\n';
  barran[1] = '\0';
  Vector3 *uoli_pos;
  get_current_GPS_position(uoli_pos);
  
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

void freiar()
{

}
