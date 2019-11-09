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
    a = set_torque(30, 30);
    a = set_engine_torque(1, 30);
    a = set_engine_torque(2, 30);
    return 0;
}