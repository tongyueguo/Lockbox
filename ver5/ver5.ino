
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
int correct = 0, setting_button = 0, location = 0, i = 0, range = 10, n_password = 3, check_password = 0, a = 0, setting = 0, reset_password = 0;
int password[3][3], input[3], v_setting[3] = { 0, 0, 0 };
Servo servo;
char buffer[20];
void unlock();
void lock();
void alarm();


void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(v_location, INPUT);
  servo.attach(motor);
  pinMode(speaker, OUTPUT);
  range=-1;
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
    location=-1;
    while (setting == 1) {
      a = location;
      location = analogRead(v_location) * 3 / 1024;
      if (location == 0 && a != location) {
        Serial.println("Exit");
      }
      if (location == 1 && a != location) {
        Serial.println("Change password");
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
      }
    }




      location=-1;
    while (1) {
      a = location;

      location = analogRead(v_location) * (n_password + 1) / 1024;
      if (location < n_password) {
        if (location != a) {

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

        if (reset_password == 0) {
          delay(10);
          if (location != a) {
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
            break;
          }
        }
      } 
      else {
        Serial.println("You are already in setting.");
      }
    }
    }




    if ((v_setting[location] == 0 && check_password == 0) || reset_password == 1) {
        
      while (1 == 1) {
        a = password[location][i];
        password[location][i] = analogRead(A0) * range / 1024;
        if (password[location][i] != a) {
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
          u8g.drawStr( 0, 22, "Set Password");
          u8g.drawStr( 48, 36, "Number");
          itoa(location,buffer,10);
          u8g.drawStr(108,22,buffer);
          itoa(i,buffer,10);
          u8g.drawStr(108,36,buffer);
          itoa(password[location][i],buffer,10);
          u8g.drawStr(60,50,buffer);
          
        } while( u8g.nextPage() );
        setting_button = digitalRead(button);
        if (setting_button == 1) {
          i++;

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
      location=-1;
      input[i]=-1;
      while (1) {
        
        a = location;
        location = analogRead(v_location) * n_password / 1024;

        if (v_setting[location] == 0 && location != a) {
          Serial.println("password not set");
          u8g.firstPage();
        do {
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr( 0, 22, "Password Not Set");
         
        } while( u8g.nextPage() );
        }
        if (v_setting[location] == 1) {
          a=input[i];
          input[i] = analogRead(A0) * range / 1024;
          if (input[i] != a) {
            Serial.print("Please enter password ");
            Serial.print(location);
            Serial.print(" number ");
            Serial.print(i);
            Serial.print(" :");
            Serial.println(input[i]);
            u8g.firstPage();
        do {
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr( 0, 22, "Enter Password");
          u8g.drawStr( 48, 36, "Number");
          itoa(location,buffer,10);
          u8g.drawStr(120,22,buffer);
          itoa(i,buffer,10);
          u8g.drawStr(120,36,buffer);
          itoa(input[i],buffer,10);
          u8g.drawStr(60,50,buffer);
          
        } while( u8g.nextPage() );
          }
          setting_button = digitalRead(button);
          if (setting_button == 1) {
            i++;
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
          u8g.drawStr( 0, 22, "Unlock");
         
        } while( u8g.nextPage() );
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
          u8g.drawStr( 0, 22, "Lock");
         
        } while( u8g.nextPage() );
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
          u8g.drawStr( 0, 22, "Wrong Password");
         
        } while( u8g.nextPage() );
  for (times = 0; times <= 10; times++) {
    digitalWrite(speaker, 0);
    delay(100);
    digitalWrite(speaker, 1000);
    delay(100);
  }
  delay(1000);
}
