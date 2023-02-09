#include <Wire.h>

float roll, pitch, yaw;
float rollCalibration, pitchCalibration, yawCalibration;
int calibrationNumber;

void gyroCommunication (void){
    Wire.beginTransmission(0x68);  // Start Communication with Gyro.

    // Starting Low Pass Filter.
    Wire.write(0x1A);              
    Wire.write(0x05);
    Wire.endTransmission();

    // Setting Sensitivity Scale factor.
    Wire.beginTransmission(0x68);  
    Wire.write(0x1B);
    Wire.write(0x8);
    Wire.endTransmission();

    // Acessing Registers storing Gyro Measurements.
    Wire.beginTransmission(0x68);
    Wire.write(0x43);
    Wire.endTransmission(); 

    // Read Gyro Measurements around X axis
    Wire.requestFrom(0x68);
    int16_t gyroX = Wire.read()<<8 | Wire.read(); 
    int16_t gyroY = Wire.read()<<8 | Wire.read(); 
    int16_t gyroZ = Wire.read()<<8 | Wire.read(); 

    // Convert Measurements in Deg/Sec.
    roll=(float)GyroX/65.5;
    pitch=(float)GyroY/65.5;
    yaw=(float)GyroZ/65.5;
}


void void setup() {
    Serial.begin(57600);
    Wire.setClock(400000);
    Wire.begin();
    delay(250);
    Wire.beginTransmission(0x68); 
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission();

    // Calibration measurements. NOT TO MOVE MPU6050 DURING THIS 2 SECONDS.
    for (calibrationNumber = 0; calibrationNumber < 2000; calibrationNumber++){
        gyroCommunication();
        rollCalibration+=roll;
        pitchCalibration+=pitch;
        yawCalibration+=yaw;
        delay(1);
    }

    // Calculate Calibration Values.
    rollCalibration/=2000;
    pitchCalibration/=2000;
    yawCalibration/=2000;
}

void void loop() {
    gyroCommunication();
    roll-=rollCalibration;      
    pitch-=pitchCalibration;
    yaw-=yawCalibration;
    Serial.print("Roll rate [°/s]= ");
    Serial.print(roll);
    Serial.print(" Pitch Rate [°/s]= ");
    Serial.print(pitch);
    Serial.print(" Yaw Rate [°/s]= ");
    Serial.println(yaw);
    delay(50);
}