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

int get_distance_squared(int pos1_x, int pos1_z);

char *IntToString(int x, char ret[]);

int tamanhoNumero(int x);

void freiar(int torque1, int torque2);

int elevacao();

int perigo();

int main()
{ 
  /* variáveis para escrita */
  char digits_str[20];
  char barran[2];
  barran[0] = '\n';
  barran[1] = '\0';

  int a, aux, i, j;
  short int b;
  
  i = set_head_servo(0, 28);

  /* acha todos os amigos no vetor de amigos */
  for(int jj = 0; jj < 5; jj++)
  {
    achar_amigo(friends_locations[jj].x, friends_locations[jj].z);
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
  if (angulo == 0) 
  {
    while ((aux->y > 5) && (aux->y < 355))
    {
      get_gyro_angles(aux);
    }
    return;
  }
  while (aux->y < angulo - 5 || aux->y > angulo + 5) {
    get_gyro_angles(aux);
  }
  set_torque(0,0);    
  return;
}

//retorna a distancia ao quadrado entre um ponto e o uoli
int get_distance_squared(int pos1_x, int pos1_z) {
  Vector3 *uoli;

  get_current_GPS_position(uoli);

  int pos2_x, pos2_z;
  pos2_x = uoli->x;
  pos2_z = uoli->z;

  int aux1 = pos1_x - pos2_x;
  int aux2 = pos1_z - pos2_z;
  
  char digits_str[20];
  char barran[2];
  barran[0] = '\n';
  barran[1] = '\0';

  aux1 = aux1 * aux1;
  aux2 = aux2 * aux2;

  puts("Squared distance: ");
  puts(IntToString(aux1 + aux2, digits_str));
  puts(barran);
  
  return aux1 + aux2;
}

void achar_amigo(int pos_x, int pos_z) {
  /* variáveis de escrita */
  char digits_str[20];
  char barran[2];
  int sinal_x, sinal_y;
  barran[0] = '\n';
  barran[1] = '\0';

  int sinal_z;
  int i, j;

  /* struct de posicao */
  Vector3 *uoli_pos;
  get_current_GPS_position(uoli_pos);

  puts("initial pos x: ");
  puts(IntToString(uoli_pos->x, digits_str));
  puts(barran);
  puts("initial pos z: ");
  puts(IntToString(uoli_pos->z, digits_str));
  puts(barran);

  /* anda até perto do amigo */
  while (get_distance_squared(pos_x, pos_z) > 250)
  {
    puts("[ tentativa ]");
    puts(barran);

    /* arruma em x */
    get_current_GPS_position(uoli_pos);
    sinal_x = pos_x - uoli_pos->x; 

    if (sinal_x > 5) 
    {
      puts("[ positivo_x ]");
      puts(barran);
      alinha_angulo(90);
    }
    else if (sinal_x < -5)
    {
      alinha_angulo(270);
      puts("[ negativo_x ]");
      puts(barran);
    }

    set_torque(8, 8);
    puts("[ andando ]");
    puts(barran);

    i = get_us_distance(); 
    while ((i == -1) && (!elevacao()) /*&& (!perigo())*/ && ((get_distance_squared(pos_x, pos_z) > 250))) 
    {
      i = get_us_distance();
      get_current_GPS_position(uoli_pos);
      sinal_x = pos_x - uoli_pos->x;
      if (sinal_x > -5 && sinal_x < 5)
      {
        break;
      }
      puts("[ continua ]");
      puts(barran);

      continue;
    }

    i = get_time();
    j = get_time();

    if (elevacao()) 
    {
      puts("[ ingrime ]");
      puts(barran);
      freiar(50, 50);
      while (j - i < 5000) 
      {
        j = get_time();
        continue;
      }
    }
    else
    {
      freiar(50, 50);
      puts("[ nao anda ]");
      puts(barran);
      i = get_time();
      j = get_time();
      while (j - i < 2000) 
      {
        j = get_time();
        continue;
      }
    }

    /* arruma em z */
    get_current_GPS_position(uoli_pos);
    sinal_z = pos_z - uoli_pos->z; 

    if (sinal_z > 5) 
    {
      puts("[ positivo_z ]");
      puts(barran);
      alinha_angulo(0);
    }
    else if (sinal_z < -5)
    {
      alinha_angulo(180);
      puts("[ negativo_z ]");
      puts(barran);
    }

    set_torque(8, 8);
    puts("[ anda ]");
    puts(barran);

    i = get_us_distance(); 
    while ((i == -1) && (!elevacao()) /*&& (!perigo())*/ && ((get_distance_squared(pos_x, pos_z) > 250))) 
    {
      i = get_us_distance();
      get_current_GPS_position(uoli_pos);
      sinal_z = pos_z - uoli_pos->z;
      if (sinal_z > -5 && sinal_z < 5)
      {
        break;
      }
      puts("[ continua ]");
      puts(barran);

      continue;
    }

    i = get_time();
    j = get_time();

    if (elevacao()) 
    {
      freiar(50, 50);
      puts("[ ingrime ]");
      puts(barran);
      while (j - i < 5000) 
      {
        j = get_time();
        continue;
      }
    }
    else
    {
      freiar(50, 50);
      puts("[ nao anda ]");
      puts(barran);
      while (j - i < 2000) 
      {
        j = get_time();
        continue;
      }
    }

    get_current_GPS_position(uoli_pos); /* pega posicao atual do ouli */
  }
  
  /* avisa que achou amigo e termina funcao */
  puts("[ amigo encontrado! ]");
  puts(barran);

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
  while(tempo2 - tempo1 != 300)
  {
    tempo2 = get_time();
  }

  /* pega cordenada depois de 300 ms */
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

    /* pega tempo após 300 ms */
    while(tempo2 - tempo1 != 300)
    {
      tempo2 = get_time();
    }

    /* pega cordenada depois de 300 ms */
    get_current_GPS_position(cord2);

    /* pega velocidade após 300 ms */
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
    puts("angle: ");
    puts(IntToString(aux->x, digits_str));
    puts(barran);
    puts("angle: ");
    puts(IntToString(aux->z, digits_str));
    puts(barran);
    ret = 1;
  }
  return ret;
}

//retorna 1 se o uoli esta perto do perigo, 0 caso contrario
int perigo()  
{ 
    for(int i=0; i<5; i++)
    {
      /*pega posicao uoli*/
      Vector3 *uoli;
      get_current_GPS_position(uoli);

      /*pega posicao do perigo atual*/
      int x = dangerous_locations[i].x;
      int z = dangerous_locations[i].z;

      int difX = uoli->x-x;
      int difZ = uoli->z-z;

      difX = difX;
      difX = difX*difX;

      difZ = difZ;
      difZ = difZ*difZ;

      if(difX + difZ > 2500)
      {
        continue;
      }
      else
        return 1;
    }  
}
