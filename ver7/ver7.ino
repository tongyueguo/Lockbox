#include <U8glib.h>
#include <Servo.h>
#include <EEPROM.h>
#include "logo.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
#define button 2
#define motor 3
#define red 4
#define green 5
#define blue 6
#define speaker 12
#define v_location A3
int correct = 0, setting_button = 0, location = 0, i = 0, range = 10, n_password = 3, b_password = 3, check_password = 0, a = 0, setting = 0, reset_password = 0, refresh = 0, address = 0, wrong_times = 0, wrong_lock;
int input[10];
Servo servo;
char buffer[20];
void unlock();
void lock();
void alarm();
void about();
void reset_lockbox();


void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(v_location, INPUT);
  servo.attach(motor);
  pinMode(speaker, OUTPUT);
  digitalWrite(speaker, 1);
  range = EEPROM.read(257);
  wrong_times = EEPROM.read(258);
  wrong_lock = EEPROM.read(259);
  n_password = EEPROM.read(260);
  b_password = EEPROM.read(261);
  about();
  refresh = 1;
  while (EEPROM.read(256) == 0) {
    a = range;
    range = analogRead(v_location) * 21 / 1024 + 1;
    if (range != a || refresh == 1) {
      refresh = 0;
      Serial.print("The range of the password: 0-");
      Serial.println(range - 1);
      u8g.firstPage();
      do {
        u8g.drawBitmapP(40, 7, 6, 22, logo_range);
        u8g.setFont(u8g_font_unifont);
        u8g.drawStr(8, 48, "Password Range");
        u8g.drawStr(48, 62, "0-");
        itoa(range - 1, buffer, 10);
        u8g.drawStr(64, 62, buffer);
      } while (u8g.nextPage());
    }
    setting_button = digitalRead(button);
    if (setting_button == 1) {
      EEPROM.write(257, range);
      delay(1000);
      break;
    }
  }
  while (EEPROM.read(256) == 0) {
    a = n_password;
    n_password= analogRead(v_location) * 6 / 1024 + 1;
    if (n_password != a || refresh == 1) {
      refresh = 0;
      Serial.print("The number of the password: 0-");
      Serial.println(n_password);
      u8g.firstPage();
      do {
        u8g.drawBitmapP(40, 7, 6, 22, logo_range);
        u8g.setFont(u8g_font_unifont);
        u8g.drawStr(0, 48, "Password Numbers");
        itoa(n_password, buffer, 10);
        u8g.drawStr(64, 62, buffer);
      } while (u8g.nextPage());
    }
    setting_button = digitalRead(button);
    if (setting_button == 1) {
      EEPROM.write(260, n_password);
      delay(1000);
      break;
    }
  }
  while (EEPROM.read(256) == 0) {
    a = b_password;
    b_password= analogRead(v_location) * 6 / 1024 + 1;
    if (b_password != a || refresh == 1) {
      refresh = 0;
      Serial.print("The number of the password: 0-");
      Serial.println(b_password);
      u8g.firstPage();
      do {
        u8g.drawBitmapP(40, 7, 6, 22, logo_range);
        u8g.setFont(u8g_font_unifont);
        u8g.drawStr(12, 48, "Password Bits");
        itoa(b_password, buffer, 10);
        u8g.drawStr(64, 62, buffer);
      } while (u8g.nextPage());
    }
    setting_button = digitalRead(button);
    if (setting_button == 1) {
      EEPROM.write(261, b_password);
      delay(1000);
      break;
    }
  }
  EEPROM.write(256, 1);

}

void loop() {
  while (1) {
    if (wrong_times >= 5 && wrong_lock == 1)
    {
      u8g.firstPage();
      do {
        u8g.drawBitmapP(40, 0, 6, 48, logo_lock);
        u8g.setFont(u8g_font_unifont);
        u8g.drawStr(48, 60, "Lock");
      } while (u8g.nextPage());
      while (1) {
        setting_button = digitalRead(button);
        if (setting_button == 1)
        {
          wrong_times = 0;
          break;
        }
      }
      reset_lockbox();
    }
    location = -1;
    while (setting == 1) {
      a = location;
      location = analogRead(v_location) * 5 / 1024;
      if (location == 0 && a != location) {
        Serial.println("Exit");
        u8g.firstPage();
        do {
          u8g.drawBitmapP(40, 0, 6, 48, logo_exit);
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr(48, 60, "Exit");

        } while (u8g.nextPage());
      }
      if (location == 1 && a != location) {
        Serial.println("Reset password");
        u8g.firstPage();
        do {
          u8g.drawBitmapP(40, 0, 6, 48, logo_reset);
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr(8, 60, "Reset Password");

        } while (u8g.nextPage());
      }
      if (location == 2 && a != location) {
        Serial.println("About");
        u8g.firstPage();
        do {
          u8g.drawBitmapP(40, 0, 6, 48, logo_about);
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr(44, 60, "About");

        } while (u8g.nextPage());
      }
      if (location == 3 && a != location) {
        Serial.println("Wrong Lock");
        u8g.firstPage();
        do {
          u8g.drawBitmapP(40, 0, 6, 48, logo_set);
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr(24, 60, "Wrong Lock");

        } while (u8g.nextPage());
      }
      if (location == 4 && a != location) {
        Serial.println("Reset Lockbox");
        u8g.firstPage();
        do {
          u8g.drawBitmapP(40, 0, 6, 48, logo_reset);
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr(12, 60, "Reset Lockbox");

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
        if (location == 3) {
          while (1) {

            a = location;
            location = analogRead(v_location) * 2 / 1024;
            if (location == 0 && a != location)
            {
              wrong_lock = 0;
              EEPROM.write(259, 0);
              u8g.firstPage();
              do {
                u8g.drawBitmapP(0, 8, 6, 48, logo_set);
                u8g.setFont(u8g_font_unifont);
                u8g.drawStr(48, 30, "Wrong Lock");
                u8g.drawStr(56, 44, "Disabled");
              } while (u8g.nextPage());
            }
            if (location == 1 && a != location)
            {
              wrong_lock = 1;
              EEPROM.write(259, 1);
              u8g.firstPage();
              do {
                u8g.drawBitmapP(0, 8, 6, 48, logo_set);
                u8g.setFont(u8g_font_unifont);
                u8g.drawStr(48, 30, "Wrong Lock");
                u8g.drawStr(60, 44, "Enabled");
              } while (u8g.nextPage());
            }
            setting_button = digitalRead(button);
            if (setting_button == 1) {
              setting = 0;
              break;
            }
          }
        }
        if (location == 4) {
          delay(1000);
          reset_lockbox();
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
          if (EEPROM.read(location + 128) == 0) {
            Serial.println("----------password not set");
            u8g.firstPage();
            do {
              u8g.drawBitmapP(0, 8, 6, 48, logo_confirm);
              u8g.setFont(u8g_font_unifont);
              u8g.drawStr(56, 16, "Position");
              u8g.drawStr(56, 44, "Password");
              u8g.drawStr(60, 58, "Not Set");
              itoa(location+1, buffer, 10);
              u8g.drawStr(84, 30, buffer);
            } while (u8g.nextPage());

          } else {
            Serial.println("----------password set");
            u8g.firstPage();
            do {
              u8g.drawBitmapP(0, 8, 6, 48, logo_confirm);
              u8g.setFont(u8g_font_unifont);
              u8g.drawStr(56, 16, "Position");
              u8g.drawStr(56, 44, "Password");
              u8g.drawStr(74, 58, "Set");
              itoa(location+1, buffer, 10);
              u8g.drawStr(84, 30, buffer);
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
        if (reset_password == 0 && check_password == 0) {
          delay(10);
          if (location != a || refresh == 1) {
            refresh = 0;
            Serial.println("Setting");
            u8g.firstPage();
            do {
              u8g.drawBitmapP(40, 0, 6, 48, logo_setting);
              u8g.setFont(u8g_font_unifont);
              u8g.drawStr(36, 60, "Setting");

            } while (u8g.nextPage());
          }
          setting_button = digitalRead(button);
          if (setting_button == 1) {
            delay(1000);
            i = 0;
            for (a = 0; a <= n_password; a++) {
              if (EEPROM.read(a + 128) == 1) {
                i = 1;
              }
            }
            if (i == 0) {
              Serial.println("Please set at least 1 password.");
              u8g.firstPage();
              do {
                u8g.drawBitmapP(40, 0, 6, 48, logo_warning);
                u8g.setFont(u8g_font_unifont);
                u8g.drawStr(8, 60, "Set 1 Password");
              } while (u8g.nextPage());
              delay(1000);
            } else {
              i = 0;
              check_password = 1;
              u8g.firstPage();
              do {
                u8g.drawBitmapP(40, 0, 6, 48, logo_verify);
                u8g.setFont(u8g_font_unifont);
                u8g.drawStr(28, 60, "Verifying");
              } while (u8g.nextPage());
              continue;
            }
          }
        } else {
          if (reset_password == 1) {
            Serial.println("You are reseting password.");
            u8g.firstPage();
            do {
              u8g.drawBitmapP(40, 0, 6, 48, logo_reset);
              u8g.setFont(u8g_font_unifont);
              u8g.drawStr(0, 60, "Reset Password");
            } while (u8g.nextPage());
          }
          else {
            Serial.println("Verifying");
            u8g.firstPage();
            do {
              u8g.drawBitmapP(40, 0, 6, 48, logo_verify);
              u8g.setFont(u8g_font_unifont);
              u8g.drawStr(28, 60, "Verifying");

            } while (u8g.nextPage());
          }
        }
      }
    }




    if ((EEPROM.read(location + 128) == 0 && check_password == 0) || reset_password == 1) {
      while (1) {
        a = input[i];
        input[i] = analogRead(A0) * range / 1024;
        if (input[i] != a || refresh == 1) {
          refresh = 0;
          Serial.print("Please set password ");
          Serial.print(location);
          Serial.print(" number ");
          Serial.print(i);
          Serial.print(" :");
          Serial.println(input[i]);
        }
        u8g.firstPage();
        do {
          u8g.drawBitmapP(0, 8, 6, 48, logo_set);
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr(48, 16, "Set");
          u8g.drawStr(48, 30, "Password");
          u8g.drawStr(48, 44, "Number");
          itoa(location+1, buffer, 10);
          u8g.drawStr(114, 30, buffer);
          itoa(i+1, buffer, 10);
          u8g.drawStr(114, 44, buffer);
          itoa(input[i], buffer, 10);
          u8g.drawStr(84, 58, buffer);

        } while (u8g.nextPage());
        setting_button = digitalRead(button);
        if (setting_button == 1) {
          i++;
          refresh = 1;
          if (i == b_password) {
            reset_password = 0;
            i = 0;
            delay(1000);
            break;
          }
          delay(1000);
        }
        continue;
      }
      EEPROM.write(location + 128, 1);
      for (address = location * b_password,i=0; address < (location + 1)*b_password; address++,i++)
      {
        EEPROM.write(address, input[i]);
      }
      continue;
    }



    if (EEPROM.read(location + 128) == 1 || check_password == 1) {
      i = 0;
      location = -1;
      input[i] = -1;
      while (1) {
        a = location;
        location = analogRead(v_location) * (n_password + 1) / 1024;
        if (EEPROM.read(location + 128) == 0 ) {
          if (location != a) {
            Serial.println("password not set");
            u8g.firstPage();
            do {
              u8g.drawBitmapP(40, 0, 6, 48, logo_warning);
              u8g.setFont(u8g_font_unifont);
              u8g.drawStr(0, 60, "Password Not Set");

            } while (u8g.nextPage());
          }
          setting_button = digitalRead(button);
          if (setting_button == 1) {
            check_password = 0;
            break;
          }
        } else {
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
              u8g.drawBitmapP(0, 8, 6, 48, logo_enter);
              u8g.setFont(u8g_font_unifont);
              u8g.drawStr(48, 16, "Enter");
              u8g.drawStr(48, 30, "Password");
              u8g.drawStr(48, 44, "Number");
              itoa(location+1, buffer, 10);
              u8g.drawStr(114, 30, buffer);
              itoa(i+1, buffer, 10);
              u8g.drawStr(114, 44, buffer);
              itoa(input[i], buffer, 10);
              u8g.drawStr(84, 58, buffer);

            } while (u8g.nextPage());

          }
          setting_button = digitalRead(button);
          if (setting_button == 1) {
            i++;
            refresh = 1;
            if (i == b_password) {
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

      
      for (address = location * b_password, i = 0,correct=1; address < (location + 1)*b_password; address++, i++) {
        if (input[i] != EEPROM.read(address)) {
          correct=0;
        }
      }


      if (correct == 1) {
        wrong_times = 0;
        EEPROM.write(258, wrong_times);
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
          check_password = 0;
          correct = 0;
          continue;
        }
      } else {
        alarm();
        wrong_times = wrong_times + 1;
        EEPROM.write(258, wrong_times);
        check_password = 0;
      }
    }
  }
}


void unlock() {
  int pos;

  Serial.println("Unlock");
  u8g.firstPage();
  do {
    u8g.drawBitmapP(40, 0, 6, 48, logo_unlock);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(40, 60, "Unlock");

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
    u8g.drawBitmapP(40, 0, 6, 48, logo_lock);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(48, 60, "Lock");

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
    u8g.drawBitmapP(40, 0, 6, 48, logo_alarm);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(16, 60, "Bad Password");

  } while (u8g.nextPage());

  for (times = 0; times <= 10; times++) {
    digitalWrite(speaker, 0);
    delay(100);
    digitalWrite(speaker, 1);
    delay(100);
  }
  delay(1000);
}
void about() {
  u8g.firstPage();
  do {
    u8g.drawBitmapP(0, 0, 9, 64, base);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(72, 16, "JLHS");
    u8g.drawStr(72, 30, "ARDUINO");
    u8g.drawStr(72, 44, "PRODUCT");
    u8g.drawStr(72, 58, "LOCKBOX");
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
void reset_lockbox() {
  Serial.println("Reset Lockbox");
  u8g.firstPage();
  do {
    u8g.drawBitmapP(40, 0, 6, 48, logo_reset);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(32, 60, "Reseting");

  } while (u8g.nextPage());
  for (address = 0 ; address < EEPROM.length() ; address++) {
    EEPROM.write(address, 0);
  }
  Serial.println("Completed");
  u8g.firstPage();
  do {
    u8g.drawBitmapP(40, 0, 6, 48, logo_correct);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(28, 60, "Completed");
  } while (u8g.nextPage());
  delay(5000);
  u8g.firstPage();
  do {
    u8g.drawBitmapP(40, 0, 6, 48, logo_about);
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(16, 60, "Please Reboot");
  } while (u8g.nextPage());
  delay(5000);
}
