// Code generated by Arduino IoT Cloud, DO NOT EDIT.
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <ESP32Servo.h>  //includes the servo library
#include <Wire.h> // Include the Wire library
#include <LiquidCrystal_I2C.h> // Include the LiquidCrystal_I2C library

#define I2C_ADDRESS 0x3F // I2C address of the LCD module
#define LCD_COLUMNS 16 // Number of columns in the LCD
#define LCD_ROWS 4 // Number of rows in the LCD

LiquidCrystal_I2C lcd(I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS); // Create a new LiquidCrystal_I2C object


const char DEVICE_LOGIN_NAME[] = "af17e661-1f55-452f-ac67-8293fc326aed";

const char SSID[] = "realme";                         // Network SSID (name)
const char PASS[] = "12345678";                   // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[] = "WPHYHVQLSGS857AXHBUR";  // Secret device password

bool s1 = false;
bool s2 = false;
bool s3 = false;
bool s4 = false;
String s5="Parking Empty";

void initProperties() {

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(s1, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(s2, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(s3, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(s4, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(s5, READ, ON_CHANGE, NULL);
}


WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);


Servo myservo;

#define ir_enter 34
#define ir_back 32

#define ir_car1 33
#define ir_car2 25
#define ir_car3 26
#define ir_car4 27


int S1 = 0, S2 = 0, S3 = 0, S4 = 0;
int flag1 = 1, flag2 = 1;
int slot = 4;


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Initialize the I2C communication
  Wire.begin(21,22);

  // Initialize the LCD screen
  lcd.init();

  // Turn on the backlight of the LCD screen
  lcd.backlight();

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_car4, INPUT);
 

  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);

  myservo.attach(13);
  myservo.write(90);

  lcd.begin(20, 4);
  lcd.setCursor(0, 1);
  lcd.print("    Car  parking  ");
  lcd.setCursor(0, 2);
  lcd.print("       System     ");
  delay(2000);
  lcd.clear();

  Read_Sensor();

  int total = S1 + S2 + S3 + S4;
  slot = slot - total;
}

void loop() {
  ArduinoCloud.update();
  // Your code here

  Read_Sensor();

  lcd.setCursor(0, 0);
  lcd.print("   Have Slot: ");
  lcd.print(slot);
  lcd.print("    ");

  lcd.setCursor(0, 1);
  if (S1 == 1) {
    s1 = true;
    lcd.print("S1:Fill ");
  } else {
    s1 = false;
    lcd.print("S1:Empty");
  }

  lcd.setCursor(10, 1);
  if (S2 == 1) {
    s2 = true;
    lcd.print("S2:Fill ");
  } else {
    s2 = false;
    lcd.print("S2:Empty");
  }

  lcd.setCursor(0, 2);
  if (S3 == 1) {
    s3 = true;
    lcd.print("S3:Fill ");
  } else {
    s3 = false;
    lcd.print("S3:Empty");
  }

  lcd.setCursor(10, 2);
  if (S4 == 1) {
    s4 = true;
    lcd.print("S4:Fill ");
  } else {
    s4 = false;
    lcd.print("S4:Empty");
  }

if(s1&&s2&&s3&&s4)
{
  s5="Parking is Full";
}
else {
s5="Parking Empty";
}


  if (digitalRead(ir_enter) == 1 && flag1 == 1) {
    if (slot > 0) {
      flag1 = 0;
      if (flag2 == 1) {
        myservo.write(180);
        slot = slot - 1;
        if(slot<0)
        slot=0;
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.print(" Sorry Parking Full ");
      delay(1000);
    }
  }

  if (digitalRead(ir_back) == 1 && flag2 == 1) {
    flag2 = 0;
    if (flag1 == 1) {
      myservo.write(180);
      slot = slot + 1;
      if(slot>4)
      slot=4;
    }
  }

  if (flag1 == 0 && flag2 == 0) {
    delay(1000);
    myservo.write(90);
    flag1 = 1, flag2 = 1;
  }

  delay(1);
}

void Read_Sensor() {
  S1 = 0, S2 = 0, S3 = 0, S4 = 0;

  if (digitalRead(ir_car1) == 1) { S1 = 1; }
  if (digitalRead(ir_car2) == 1) { S2 = 1; }
  if (digitalRead(ir_car3) == 1) { S3 = 1; }
  if (digitalRead(ir_car4) == 1) { S4 = 1; }
}