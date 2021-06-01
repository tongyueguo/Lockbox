
#include <Servo.h>
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
#define button 2
#define motor 3
#define red 4
#define green 5
#define blue 6
#define speaker 12
#define v_location A3
int correct = 0, setting_button = 0, location = 0, i = 0, range = 10, n_password = 3, check_password = 0, a = 0, setting = 0, reset_password = 0, refresh = 0;
int password[3][3], input[3], v_setting[3] = { 0, 0, 0 };
Servo servo;
char buffer[20];
void unlock();
void lock();
void alarm();
void about();
char base[] U8G_PROGMEM ={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xC0,
0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,
0x00,0x03,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xE0,0x00,0x00,0x00,
0x00,0x00,0x00,0x03,0xE3,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xE0,0x00,
0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,
0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,
0x03,0xC1,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xE0,0x00,0x00,0x00,0x00,
0x00,0x00,0x03,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xE0,0x00,0x00,
0x00,0x00,0x00,0xFE,0x03,0xFF,0xE0,0x3F,0x80,0x00,0x00,0x00,0xFE,0x03,0xE3,0xE0,
0x3F,0x80,0x00,0x00,0x00,0xFE,0x03,0xC1,0xE0,0x3F,0x80,0x00,0x00,0x3F,0xFF,0xF3,
0xC1,0xE7,0xFF,0xFE,0x00,0x00,0x7F,0xC7,0xF3,0xC1,0xE7,0xF1,0xFF,0x00,0x00,0xFF,
0xFF,0xF3,0xC1,0xE7,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0xF3,0xC1,0xE7,0xFF,0xFF,0xC0,
0x07,0xFE,0x01,0xF3,0xFF,0xE7,0x80,0x3F,0xF0,0x0F,0xFE,0x01,0xF3,0xFF,0xE7,0x80,
0x3F,0xF8,0x3F,0xFF,0xFF,0xF3,0xFF,0xE7,0xFF,0xFF,0xFC,0x7F,0xFF,0xFF,0xF3,0xFF,
0xE7,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x03,0xC1,0xE0,0x00,0x00,0x00,0x07,0xFF,0xFF,
0xF3,0xC1,0xE7,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xF3,0xC1,0xE7,0xFF,0xFF,0xE0,0x07,
0xFF,0xFF,0xF3,0xC1,0xE7,0xFF,0xFF,0xE0,0x07,0xC3,0xC3,0xF3,0xC1,0xE7,0xC3,0xC1,
0xE0,0x07,0xC3,0xC3,0xF3,0xFF,0xE7,0xC3,0xC1,0xE0,0x07,0xC3,0xC3,0xF3,0xFF,0xE7,
0xC3,0xC1,0xE0,0x07,0xC3,0xC3,0xF3,0xFF,0xE7,0xC3,0xC1,0xE0,0x07,0xFF,0xFF,0xF3,
0xC1,0xE7,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xF3,0x3E,0x67,0xFF,0xFF,0xE0,0x07,0xFF,
0xFF,0xF0,0xFF,0x87,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xF3,0xFF,0xE7,0xFF,0xFF,0xE0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xF3,0xFF,0xE7,0xFF,
0xFF,0xE0,0x07,0xFF,0xFF,0xF3,0xFF,0xE7,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xF3,0xC1,
0xE7,0xFF,0xFF,0xE0,0x07,0xC3,0xC3,0xF3,0xC1,0xE7,0xC3,0xE1,0xE0,0x07,0xC3,0xC3,
0xF3,0xC1,0xE7,0xC3,0xC1,0xE0,0x07,0xC3,0xC3,0xF3,0xC1,0xE7,0xC3,0xC1,0xE0,0x07,
0xC3,0xC3,0xF3,0xC1,0xE7,0xC3,0xC1,0xE0,0x07,0xFF,0xFF,0xF3,0xC1,0xE7,0xFF,0xFF,
0xE0,0x07,0xFF,0xFF,0xF3,0xFF,0xE7,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x03,0xFF,0xE0,
0x00,0x00,0x00,0x07,0xFF,0xFF,0xF3,0xFF,0xE7,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xF3,
0xFF,0xE7,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x03,0x80,0xE0,0x00,0x00,0x00,0x00,0x00,
0x00,0x03,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};


void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(v_location, INPUT);
  servo.attach(motor);
  pinMode(speaker, OUTPUT);
  about();

  range = -1;
  while (1) {
    a = range;
    range = analogRead(v_location) * 21 / 1024 + 1;
    if (range != a) {
      Serial.print("The range of the password: 0-");
      Serial.println(range - 1);
      u8g.firstPage();
      do {
        u8g.setFont(u8g_font_unifont);
        u8g.drawStr(0, 22, "Password Range");
        u8g.drawStr(0, 36, "0-");
        itoa(range - 1, buffer, 10);
        u8g.drawStr(18, 36, buffer);
      } while (u8g.nextPage());
    }
    setting_button = digitalRead(button);
    if (setting_button == 1) {
      delay(1000);
      break;
    }
  }
}

void loop() {

  while (1) {
    location = -1;
    while (setting == 1) {
      a = location;
      location = analogRead(v_location) * 3 / 1024;
      if (location == 0 && a != location) {
        Serial.println("Exit");
        u8g.firstPage();
        do {
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr(0, 22, "Exit");

        } while (u8g.nextPage());
      }
      if (location == 1 && a != location) {
        Serial.println("Change password");
        u8g.firstPage();
        do {
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr(0, 22, "Change Password");

        } while (u8g.nextPage());
      }
      if (location == 2 && a != location) {
        Serial.println("About");
        u8g.firstPage();
        do {
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr(0, 22, "About");

        } while (u8g.nextPage());
      }


      setting_button = digitalRead(button);
      if (setting_button == 1) {
        if (location == 0) {
          delay(1000);
          setting = 0;
          break;
        }
        if (location == 1) {
          delay(1000);
          reset_password = 1;
          setting = 0;
          break;
        }
        if (location == 2) {
          delay(1000);
          about();
          setting = 0;
          break;
        }
      }
    }




    location = -1;
    while (1) {
      a = location;
      location = analogRead(v_location) * (n_password + 1) / 1024;
      if (location < n_password) {
        if (location != a || refresh == 1) {
          refresh = 0;
          Serial.print("Please cofirm the position:");
          Serial.print(location);
          if (v_setting[location] == 0) {
            Serial.println("----------password not set");
            u8g.firstPage();
            do {
              u8g.setFont(u8g_font_unifont);
              u8g.drawStr(0, 22, "Position");
              itoa(location, buffer, 10);
              u8g.drawStr(0, 36, buffer);
              u8g.drawStr(0, 50, "Password Not Set");
            } while (u8g.nextPage());
          } else {
            Serial.println("----------password set");
            u8g.firstPage();
            do {
              u8g.setFont(u8g_font_unifont);
              u8g.drawStr(0, 22, "Position");
              itoa(location, buffer, 10);
              u8g.drawStr(0, 36, buffer);
              u8g.drawStr(0, 50, "Password Set");
            } while (u8g.nextPage());
          }
        }
        setting_button = digitalRead(button);
        if (setting_button == 1) {
          delay(1000);
          break;
        }
        delay(10);
      }
      if (location == n_password) {
        if (reset_password == 0&&check_password==0) {
          delay(10);
          if (location != a || refresh == 1) {
            refresh = 0;
            Serial.println("Setting");
            u8g.firstPage();
            do {
              u8g.setFont(u8g_font_unifont);
              u8g.drawStr(0, 22, "Setting");

            } while (u8g.nextPage());
          }
          setting_button = digitalRead(button);
          if (setting_button == 1) {
            delay(1000);
            i = 0;
            for (a = 0; a <= n_password; a++) {
              if (v_setting[a] == 1) {
                i = 1;
              }
            }
            if (i == 0) {
              Serial.println("Please set at least 1 password.");
              u8g.firstPage();
              do {
                u8g.setFont(u8g_font_unifont);
                u8g.drawStr(0, 22, "Please set ");
                u8g.drawStr(0, 36, "at least ");
                u8g.drawStr(0, 50, "1 password");
              } while (u8g.nextPage());
              delay(1000);
            } else {
              i = 0;
              check_password = 1;
              u8g.firstPage();
              do {
                u8g.setFont(u8g_font_unifont);
                u8g.drawStr(0, 22, "Verifying ");
              } while (u8g.nextPage());
              continue;
            }
          }
        } else {
            if(reset_password == 1){
               Serial.println("You are reseting password.");
               u8g.firstPage();
               do {
                 u8g.setFont(u8g_font_unifont);
                 u8g.drawStr(0, 22, "Reseting Password");
               } while (u8g.nextPage());
              }
              else{
                Serial.println("Verifying");
          u8g.firstPage();
              do {
                u8g.setFont(u8g_font_unifont);
                u8g.drawStr(0, 22, "Verifying");
                } while (u8g.nextPage());
              }
        }
      }
    }




    if ((v_setting[location] == 0 && check_password == 0) || reset_password == 1) {

      while (1 == 1) {
        a = password[location][i];
        password[location][i] = analogRead(A0) * range / 1024;
        if (password[location][i] != a || refresh == 1) {
          refresh = 0;
          Serial.print("Please set password ");
          Serial.print(location);
          Serial.print(" number ");
          Serial.print(i);
          Serial.print(" :");
          Serial.println(password[location][i]);
        }
        u8g.firstPage();
        do {
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr(0, 22, "Set Password");
          u8g.drawStr(48, 36, "Number");
          itoa(location, buffer, 10);
          u8g.drawStr(108, 22, buffer);
          itoa(i, buffer, 10);
          u8g.drawStr(108, 36, buffer);
          itoa(password[location][i], buffer, 10);
          u8g.drawStr(60, 50, buffer);

        } while (u8g.nextPage());
        setting_button = digitalRead(button);
        if (setting_button == 1) {
          i++;
          refresh = 1;
          if (i == 3) {
            reset_password = 0;
            i = 0;
            delay(1000);
            break;
          }
          delay(1000);
        }
        continue;
      }
      v_setting[location] = 1;
      continue;
    }



    if (v_setting[location] == 1 || check_password == 1) {

      i = 0;
      location = -1;
      input[i] = -1;
      while (1) {

        a = location;
        location = analogRead(v_location) * (n_password + 1) / 1024;

        if (v_setting[location] == 0 ) {
          if(location != a){
          Serial.println("password not set");
          u8g.firstPage();
          do {
            u8g.setFont(u8g_font_unifont);
            u8g.drawStr(0, 22, "Password Not Set");

          } while (u8g.nextPage());
        }
        setting_button = digitalRead(button);
          if (setting_button == 1){
            check_password=0;
            break;
          }
        }
        if (v_setting[location] == 1) {
          a = input[i];
          input[i] = analogRead(A0) * range / 1024;
          if (input[i] != a || refresh == 1) {
            Serial.print("Please enter password ");
            Serial.print(location);
            Serial.print(" number ");
            Serial.print(i);
            Serial.print(" :");
            Serial.println(input[i]);
            refresh = 0;
            u8g.firstPage();
            do {
              u8g.setFont(u8g_font_unifont);
              u8g.drawStr(0, 22, "Enter Password");
              u8g.drawStr(48, 36, "Number");
              itoa(location, buffer, 10);
              u8g.drawStr(120, 22, buffer);
              itoa(i, buffer, 10);
              u8g.drawStr(120, 36, buffer);
              itoa(input[i], buffer, 10);
              u8g.drawStr(60, 50, buffer);

            } while (u8g.nextPage());
          }
          setting_button = digitalRead(button);
          if (setting_button == 1) {
            i++;
            refresh = 1;
            if (i == 3) {
              i = 0;
              delay(1000);
              break;
            }
            delay(1000);
            continue;
          }
        }
        delay(10);
      }



      if (input[0] == password[location][0] && input[1] == password[location][1] && input[2] == password[location][2]) {

        correct = 1;
      } else {
        correct = 0;
      }


      if (correct == 1) {
        if (check_password == 1) {
          Serial.print("You are now in setting.");
          setting = 1;
          check_password = 0;
          break;
        } else {
          unlock();
          while (1) {
            setting_button = digitalRead(button);
            if (setting_button == 1) {
              break;
            }
          }
          lock();
          correct = 0;
          continue;
        }
      } else {
        alarm();
      }
    }
  }
}


void unlock() {
  int pos;

  Serial.println("Unlock");
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(0, 22, "Unlock");

  } while (u8g.nextPage());
  for (pos = 0; pos <= 180; pos++) {
    servo.write(pos);
    delay(5);
  }
}
void lock() {
  int pos;
  Serial.println("Lock");
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(0, 22, "Lock");

  } while (u8g.nextPage());
  delay(1000);
  for (pos = 180; pos >= 0; pos--) {
    servo.write(pos);
    delay(5);
  }
}
void alarm() {
  int times = 0;

  Serial.println("Alarm");
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(0, 22, "Wrong Password");

  } while (u8g.nextPage());
  for (times = 0; times <= 10; times++) {
    digitalWrite(speaker, 0);
    delay(100);
    digitalWrite(speaker, 1000);
    delay(100);
  }
  delay(1000);
}
void about(){
  u8g.firstPage();
  do {
    u8g.drawBitmapP(0, 0, 9, 64, base);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(72, 16, "JLHS");
    u8g.drawStr(72, 30, "ARDUINO");
    u8g.drawStr(72, 44, "PRODUCT");
  } while (u8g.nextPage());
  delay(5000);
  u8g.firstPage();
  do {
    u8g.drawBitmapP(0, 0, 9, 64, base);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(72, 16, "Program");
    u8g.drawStr(72, 30, "GTY");
    u8g.drawStr(72, 44, "Case");
    u8g.drawStr(72, 58, "CYH");
  } while (u8g.nextPage());
  delay(5000);
}
