#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include <Adafruit_GFX.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// declaring SSD1306 display object connected to I2C
#define RST_PIN    -1 // Reset pin
#define I2C_ADDRESS 0x3D //0x3D for 128x64

SSD1306AsciiWire oled;

const int buzzer = 9; //buzzer to arduino pin 9

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;


// accelerometer values
int accel_reading;
int accel_corrected;
int accel_offset = 200;
float accel_angle;
float accel_scale = 1; // set to 0.01


// gyro values
int gyro_offset = 151; // 151
int gyro_corrected;
int gyro_reading;
float gyro_rate;
float gyro_scale = 0.02;
float gyro_angle;
float loop_time = 0.05; // 50ms loop


// time stamp variables
int last_update;
int cycle_time;
long last_cycle = 0;


void time_stamp(){
while ((millis() - last_cycle) < 50){
delay(1);
}
// once loop cycle reaches 50ms, reset timer value and continue
cycle_time = millis() - last_cycle;
last_cycle = millis();
}


void setup() {
  Wire.begin();
  Wire.setClock(400000000L);
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  #if RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0

  oled.setFont(System5x7);
  oled.set2X();
  oled.clear();

  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

int count = 0;
bool is_first = true;
bool max_reached, min_reached;
double start_angle, hammer_angle;

String inString;
int input;
bool reset;

void loop(){
  oled.clear();
  
  if (count == 20){
    tone(buzzer, 1000);
    oled.print("OVER!");
  }

  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // accelerometer_X_Axis angle calc
  accel_reading = ax;
  accel_corrected = accel_reading - accel_offset;
  accel_corrected = map(accel_corrected, -16800, 16800, -90, 90);
  accel_corrected = constrain(accel_corrected, -90, 90);
  accel_angle = (float)(accel_corrected * accel_scale);

  //getting start angle to determine orientation
  if(is_first){
    delay(2000);
    start_angle = accel_angle;
    is_first = false;
  }

  if(start_angle > 0){
      if (accel_angle < -60.0){
        max_reached = true;
      } else if(accel_angle > 35.0){
        min_reached = true;
      }
    } else{
      if(accel_angle > 40.0){
        max_reached = true;
      } else if(accel_angle < -30.0){
        min_reached = true;
      }
  } 

  /* FOR HAMMER LIFT IF NEEDED
    if(start_angle > 0){
      if (accel_angle < -40.0){
        max_reached = true;
      } else if(accel_angle > 60.0){
        min_reached = true;
      }
    } else{
      if(accel_angle > 50.0){
        max_reached = true;
      } else if(accel_angle < -40.0){
        min_reached = true;
      }
  }
  */

  //counting
  if(max_reached && min_reached){
    count += 1;
    max_reached = false;
    min_reached = false;
  }

  Serial.println(count);
  oled.print(count);

  //timestamp
  time_stamp();
  delay(1000);
}
