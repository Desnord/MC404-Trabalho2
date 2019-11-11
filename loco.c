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

int main()
{ 
    int a;
    short int b;
    Vector3 vector;
    a = set_head_servo(0, 31);
    a = set_head_servo(1, 80);
    a = set_head_servo(2, 78);
    a = set_torque(30, 30);
    char teste[8];
    teste[0] = 's';
    teste[1] = 't';
    teste[2] = 'r';
    teste[3] = 'i';
    teste[4] = 'n';
    teste[5] = 'g';
    teste[7] = '\0'; 
    puts(&teste[0]);
    get_current_GPS_position(&vector);
    get_gyro_angles(&vector);
    b = get_us_distance();
    a = set_head_servo(0, 80);
    return 0;
}