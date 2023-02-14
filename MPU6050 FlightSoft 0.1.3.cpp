#include <Wire.h>

float roll, pitch, yaw;
float accelerationX, accelerationY, accelerationZ;
float angleRoll, anglePitch;
float loopTimer;

void gyroCommunication (void){
    Wire.beginTransmission(0x68);  // Start Communication with Gyro.
    Wire.write(0x1A);              // Starting Low Pass Filter.
    Wire.write(0x05);
    Wire.endTransmission();
    Wire.beginTransmission(0x68);  
    Wire.write(0x1C);              // Configuring Accelerometer.
    Wire.write(0x10);              // Setting Full Scale Range of Accelerometer to 8 G.(IN HEXADECIMAL.)
    Wire.endTransmission();
    Wire.beginTransmission(0x68);
    Wire.write(0x3B);              // First Register for outputs of Accelerometer.
    Wire.endTransmission();
    Wire.requestFrom(0x68,6);      // Requesting Value from all 6 Registers.

    int16_t accelerationXLSB = Wire.read() << 8 | Wire.read();  // Requesting 8 bits. 
    int16_t accelerationYLSB = Wire.read() << 8 | Wire.read();  // Requesting 8 bits. 
    int16_t accelerationZLSB = Wire.read() << 8 | Wire.read();  // Requesting 8 bits.

    // Read Gyro Measurements.
    Wire.requestFrom(0x68,6);
    int16_t gyroX = Wire.read()<<8 | Wire.read(); 
    int16_t gyroY = Wire.read()<<8 | Wire.read(); 
    int16_t gyroZ = Wire.read()<<8 | Wire.read(); 

    // Convert Measurements in Deg/Sec.
    roll=(float)gyroX/65.5;
    pitch=(float)gyroY/65.5;
    yaw=(float)gyroZ/65.5;

    accelerationX = (float)accelerationXLSB / 4096-0.01;  // Converting LSB values in G.
    accelerationY = (float)accelerationYLSB / 4096+0.01;  // Converting LSB values in G.
    accelerationZ = (float)accelerationZLSB / 4096+0.02;  // Converting LSB values in G.

    // Calculating Absolute angles
    angleRoll = atan(accelerationY / sqrt(accelerationX * accelerationX +   // (3.142/180); means converting Radiance in Degrees
    accelerationZ * accelerationZ))*1/(3.142/180);                          // because atan gives result in Radiance.

    anglePitch = atan(accelerationX / sqrt(accelerationY * accelerationY +
    accelerationZ * accelerationZ))*1/(3.142/180);                          
}

void setup(){
    Serial.begin(57600);    
    Wire.setClock(400000);    // Sets the Clock Speed of I2C Protocol to 400KHZ (IT IS SPECIFIED IN PRODUCT SPECS. OF MPU6050).
    Wire.begin();
    delay(250);
    Wire.beginTransmission(0x68); 
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission();
}

void loop(){
    gyroCommunication();
    Serial.print("Acceleration X (G) = ");     // Printing Acceleration on X axis in G.
    Serial.print(accelerationX);               // Printing Acceleration on X axis in G.
    Serial.print("Acceleration Y (G) = ");     // Printing Acceleration on Y axis in G.
    Serial.print(accelerationY);               // Printing Acceleration on Yaxis in G.
    Serial.print("Acceleration Z (G) = ");     // Printing Acceleration on Z axis in G.
    Serial.print(accelerationZ);               // Printing Acceleration on Z axis in G.

    Serial.print("Roll angles (deg) = ");      // Printing Roll angle in degree.
    Serial.print(angleRoll);                   // Printing Roll angle in degree.
    Serial.print("Pitch angles (deg) = ");     // Printing Pitch angle in degree.
    Serial.println(anglePitch);                  // Printing Pitch angle in degree.

    delay(50);
}