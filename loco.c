typedef struct 
{
  int x;
  int y;
  int z;
} Vector3;

int set_torque(int engine_1, int engine_2);

int set_engine_torque(int engine_id, int torque);

int set_head_servo(int servo_id, int angle);

unsigned short get_us_distance();

void get_current_GPS_position(Vector3* pos);

void get_gyro_angles(Vector3* angles);

unsigned int get_time();

void set_time(unsigned int t);

void puts(const char*);

/*           Outras Funcoes           */

int seno(int x)
{
  return 1;
}

int cosseno(int x)
{
  return 1;
}

/*escreve e pula linha*/
void putsln(char *str)
{
  puts(str);
  puts("\n\0");
}

/*retorna o tamanho de um numero (para criar string)*/
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

int main()
{ 
    int a;
    short int b;
    Vector3 vector;
    a = set_head_servo(0, 31);
    a = set_head_servo(1, 80);
    a = set_head_servo(2, 78);
    a = set_torque(30, 30);


    get_current_GPS_position(&vector);
    get_gyro_angles(&vector);
    b = get_us_distance();
    a = set_head_servo(0, 80);
    return 0;
}