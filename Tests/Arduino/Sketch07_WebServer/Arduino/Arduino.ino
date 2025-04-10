
//#include "Tools.h"
#include <ArduinoBLE.h>
#include "Arduino_BMI270_BMM150.h" //  Nano 33 BLE Sense Rev2
//#include <Arduino_LSM9DS1.h> // Nano 33 BLE Sense :( 
//#include <Arduino_LSM6DS3.h> // Nano 33 IoT 

uint32_t value = 0;
const int PIN_1 = LED_BUILTIN; // ORANGE?
const int PIN_2 = 23; // GREEN 
const int PIN_3 = 24; // BLUE


#define SERVICE_UUID               "19b10000-e8f2-537e-4f6c-d104768a1214"
#define SENSOR_CHARACTERISTIC_UUID "19b10001-e8f2-537e-4f6c-d104768a1214"

#define    LED_CHARACTERISTIC_UUID "19b10006-e8f2-537e-4f6c-d104768a1214"


BLEService customService = BLEService(SERVICE_UUID);  

BLEBoolCharacteristic           ledCharacteristic(   LED_CHARACTERISTIC_UUID, BLEWrite | BLERead | BLENotify);
BLEUnsignedIntCharacteristic sensorCharacteristic(SENSOR_CHARACTERISTIC_UUID, BLERead | BLENotify);

float values[4] = {0.0, 0.0, 0.0, 0.0};
#define ACCEL_UUID "19b10010-e8f2-537e-4f6c-d104768a1214"
BLECharacteristic accel(ACCEL_UUID, BLERead | BLENotify, sizeof(values)); 

#define GYROS_UUID "19b10011-e8f2-537e-4f6c-d104768a1214"
BLECharacteristic gyros(GYROS_UUID, BLERead | BLENotify, sizeof(values)); 

float arr[1+9]; // = {0.0, 0.0,0.0,0.0, 0.0,0.0,0.0, 0.0,0.0,0.0};
#define IMU_UUID "19b10020-e8f2-537e-4f6c-d104768a1214"
BLECharacteristic imu(IMU_UUID, BLERead | BLENotify, sizeof(arr)); 


unsigned long startMillis;

bool deviceConnected = false;

float dummy = 0.0; 
float x = 0.0, y = 0.0, z = 0.0;

void setup() 
{
  //#ifdef DEBUG
  Serial.println("[SetUp] ...");  
  Serial.begin(9600);
  //#endif
  pinMode(PIN_1, OUTPUT);

  /*
  pinMode(PIN_2, OUTPUT);
  pinMode(PIN_3, OUTPUT);

  digitalWrite(PIN_1, LOW); 
  digitalWrite(PIN_2, LOW); 
  digitalWrite(PIN_3, LOW); 
  */
  if (!BLE.begin()) {
  //#ifdef DEBUG
    Serial.println("Failed to initialize BLE!");
  //#endif
    while (1);
  }
  delay(100); 

  if (!IMU.begin()) 
  {
    Serial.println("[setup] !imu");
    while (1); 
  }

  BLE.setLocalName("Nano33BLE");
  BLE.setAdvertisedService(customService);

  customService.addCharacteristic(sensorCharacteristic);
  customService.addCharacteristic(ledCharacteristic);
  customService.addCharacteristic(accel);
  customService.addCharacteristic(gyros);
  customService.addCharacteristic(imu);

  BLE.addService(customService);
  BLE.advertise();

  //#ifdef DEBUG
  Serial.println("Waiting for client connection...");
  //#endif
}


void loop() 
{
  Serial.println("[Loop] ...");

  BLEDevice central = BLE.central();
  if (central) 
  {
    deviceConnected = true;
//#ifdef DEBUG
    Serial.println("[Loop] Connected to central device!");
//#endif

    startMillis = millis();

    while (central.connected()) 
    {
/*
      digitalWrite(PIN_2, HIGH); 
      delay(100); 
      digitalWrite(PIN_2, LOW);
      delay(100);
*/
      if (ledCharacteristic.written()) 
      {
        bool ledValue = ledCharacteristic.value();
        if (ledValue) 
        {
          digitalWrite(PIN_1, HIGH);
//#ifdef DEBUG
          Serial.println("[Loop] LED turned ON by central device");
//#endif    
        } else {
          digitalWrite(PIN_1, LOW);
//#ifdef DEBUG
          Serial.println("[Loop] LED turned OFF by central device");
//#endif          
        }
      } // ledCharacteristic
      sensorCharacteristic.writeValue(value);

      dummy = millis() - startMillis; 
      if( IMU.accelerationAvailable() )
      {   
        IMU.readAcceleration(x, y, z);
/*
        Serial.print(value);
        Serial.print(' ');

        Serial.print( dummy );
        Serial.print(' ');

        Serial.print(x);
        Serial.print(' ');
        Serial.print(y);
        Serial.print(' ');
        Serial.println(z);
*/
        values[0] = dummy;
        values[1] = x;
        values[2] = y;
        values[3] = z;
        accel.writeValue((uint8_t*)values, sizeof(values));
      }
/*
      if (IMU.gyroscopeAvailable()) 
      {
        IMU.readGyroscope(values[1], values[2], values[3]);
        Serial.print(values[1]); Serial.print(' ');
        Serial.print(values[2]); Serial.print(' ');
        Serial.println(values[3]);
        
        values[0] = dummy;
        gyros.writeValue((uint8_t*)values, sizeof(values));
      }
*/
      

      //if( IMU.accelerationAvailable() && IMU.gyroscopeAvailable())
      {   
        for(int i=0; i<10; i++) arr[i] = 0.0; 
        IMU.readAcceleration( x, y, z);
        arr[1]=x; arr[2]=y; arr[3]=z;
        
        IMU.readGyroscope(    x, y, z);
        arr[4]=x; arr[5]=y; arr[6]=z;
        
        IMU.readMagneticField(x, y, z);
        arr[7]=x; arr[8]=y; arr[9]=z;

        arr[0] = millis() - startMillis; 
        imu.writeValue((uint8_t*)values, sizeof(arr));
      }      

      
//#ifdef DEBUG
//      Serial.print("[Loop] Notified value: ");
//      Serial.println(value);
      value++;
//#endif 
    
      //delay(1000); 
    } // central.connected 

//#ifdef DEBUG
    Serial.println("[Loop] Central device disconnected.");
//#endif
    
    deviceConnected = false;
  } // central 

}