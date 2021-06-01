
#include <Servo.h>

#define button 2
#define motor 3
#define red 4
#define green 5
#define blue 6
#define speaker 12
#define v_location A3
int  correct = 0, setting_button = 0, location = 0, i = 0 , range = 10, n_password = 3, check_password = 0, a = 0, setting = 0, reset_password = 0;
int password[3][3], input[3], v_setting[3] = {0, 0, 0};
Servo servo;

void unlock();
void lock();
void alarm();


void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(v_location, INPUT);
  servo.attach(motor);
  pinMode(speaker, OUTPUT);
  while (1 == 1)
  {
    range = analogRead(v_location) * 21 / 1024 + 1;
    Serial.print("The range of the password: 0-");
    Serial.println(range - 1);
    setting_button = digitalRead(button);
    if (setting_button == 1)
    {
      delay(1000);
      break;
    }
  }
}

void loop() {

  while (1 == 1)
  {
    if (setting == 1)
    {
      while (1)
      {
        location = analogRead(v_location) * 3 / 1024;
        if (location == 0)
        {
          Serial.println("Exit");
        }
        if (location == 1)
        {
          Serial.println("Change password");
        }


        setting_button = digitalRead(button);
        if (setting_button == 1)
        {
          if (location == 0)
          {
            delay(1000);
            setting = 0;
            break;
          }
          if (location == 1)
          {
            delay(1000);
            reset_password = 1;
            setting = 0;
            break;
          }
        }
      }
    }




    while (1)
    {
      location = analogRead(v_location) * (n_password + 1 ) / 1024;
      if (location < n_password)
      {
        Serial.print("Please cofirm the position:");
        Serial.print(location);
        digitalWrite(blue, HIGH);
        digitalWrite(green, HIGH);
        if (v_setting[location] == 0)
        {
          Serial.println("----------password not set");
          digitalWrite(red, LOW);
        }
        else
        {
          Serial.println("----------password set");
          digitalWrite(red, HIGH);
        }
        setting_button = digitalRead(button);
        if (setting_button == 1)
        {
          delay(1000);
          break;
        }
        delay(10);
      }
      if (location == n_password)
      {
        if (reset_password == 0)
        {
          delay(10);
          Serial.println("Setting");
          setting_button = digitalRead(button);
          if (setting_button == 1)
          {
            delay(1000);
            i = 0;
            for (a = 0; a <= n_password; a++)
            {
              if (v_setting[a] == 1)
              {
                i = 1;
              }

            }
            if (i == 0)
            {
              Serial.println("Please set at least 1 password.");
              delay(1000);
            }
            else
            {
              i = 0;
              check_password = 1;
              break;
            }
          }
        }
        else
        {
          Serial.println("You are already in setting.");
        }
      }

    }




    if ((v_setting[location] == 0 && check_password == 0) || reset_password == 1)
    {
      digitalWrite(blue, HIGH);
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      while (1 == 1)
      {
        password[location][i] = analogRead(A0) * range / 1024;
        Serial.print("Please set password ");
        Serial.print(location);
        Serial.print(" number ");
        Serial.print(i);
        Serial.print(" :");
        Serial.println(password[location][i]);
        setting_button = digitalRead(button);
        if (setting_button == 1)
        {
          i++;

          if (i == 3)
          {
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



    if (v_setting[location] == 1 || check_password == 1)
    {

      i = 0;
      while (1)
      {
        location = analogRead(v_location) * n_password / 1024;
        digitalWrite(blue, HIGH);
        digitalWrite(red, LOW);
        digitalWrite(green, LOW);
        if (v_setting[location] == 0)
        {
          Serial.println("password not set");
          digitalWrite(red, LOW);
        }
        if (v_setting[location] == 1)
        {
          digitalWrite(red, LOW);
          input[i] = analogRead(A0) * range / 1024;
          Serial.print("Please enter password ");
          Serial.print(location);
          Serial.print(" number ");
          Serial.print(i);
          Serial.print(" :");
          Serial.println(input[i]);
          setting_button = digitalRead(button);
          if (setting_button == 1)
          {
            i++;
            if (i == 3)
            {
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



      if (input[0] == password[location][0])
      {
        if (input[1] == password[location][1])
        {
          if (input[2] == password[location][2])
          {
            correct = 1;
          } else {
            correct = 0;
          }
        } else {
          correct = 0;
        }
      } else {
        correct = 0;
      }


      if (correct == 1)
      {
        if (check_password == 1)
        { Serial.print("You are now in setting.");
          setting = 1;
          check_password = 0;
          break;
        }
        else {
          unlock();
          while (1)
          {
            setting_button = digitalRead(button);
            if (setting_button == 1)
            {
              break;
            }
          }
          lock();
          correct = 0;
          continue;
        }
      }
      else
      {
        alarm();
      }
    }
  }
}

/*int set_password(int password[3][3])
  {
  int setting_button = 0, i = 0, location = 0;
  location = analogRead(v_location) * 3 / 1024;
  while (1)
  {
    if (i == 0)
    {
      password[location][i] = analogRead(A0) * 10 / 1024;
      Serial.print("Please set the first password:");
      Serial.println(analogRead(A0) * 10 / 1024);
    }
    if (i == 1)
    {
      password[location][i] = analogRead(A1) * 10 / 1024;
      Serial.print("Please set the second password:");
      Serial.println(analogRead(A1) * 10 / 1024);
    }
    if (i == 2)
    {
      password[location][i] = analogRead(A2) * 10 / 1024;
      Serial.print("Please set the third password:");
      Serial.println(analogRead(A2) * 10 / 1024);
    }
    setting_button = digitalRead(button);
    if (setting_button == 1)
    {
      i++;
      delay(1000);
      continue;
    }
    if (i == 3)
    {
      return (0);
    }
    delay(10);
  }

  return (0);
  }

  int check_password(int password[3][3])
  {
  int input[3], setting_button = 0, i = 0, location = 0;
  location = analogRead(v_location);
  while (1)
  {
    if (i == 0)
    {
      input[i] = analogRead(A0) * 10 / 1024;
      Serial.print("Please enter the first password:");
      Serial.println(analogRead(A0) * 10 / 1024);
    }
    if (i == 1)
    {
      input[i] = analogRead(A1) * 10 / 1024;
      Serial.print("Please enter the second password:");
      Serial.println(analogRead(A1) * 10 / 1024);
    }
    if (i == 2)
    {
      input[i] = analogRead(A2) * 10 / 1024;
      Serial.print("Please enter the third password:");
      Serial.println(analogRead(A2) * 10 / 1024);
    }

    setting_button = digitalRead(button);
    if (setting_button == 1)
    {
      i++;
      delay(1000);
      continue;
    }
    if (i == 3)
    {
      delay(1000);
      break;
    }
    delay(10);
  }
  if (input[0] == password[location][0])
  { Serial.print(password[location][0]);
    delay(1000);
    if (input[1] == password[location][1])
    {
      if (input[2] == password[location][2])
      {
        return (1);
      }
    }
  }
  return (0);
  }
*/
void unlock()
{
  int pos;
  digitalWrite(blue, LOW);
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
  Serial.println("Unlock");
  for (pos = 0; pos <= 180; pos ++) {
    servo.write(pos);
    delay(5);
  }

}
void lock()
{
  int pos;
  Serial.println("Lock");
  delay(1000);
  for (pos = 180; pos >= 0; pos --) {
    servo.write(pos);
    delay(5);
  }
  digitalWrite(blue, LOW);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
}
void alarm()
{
  int times = 0;
  digitalWrite(blue, LOW);
  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);
  Serial.println("Alarm");
  for (times = 0; times <= 10; times ++) {
    digitalWrite(speaker, 0);
    delay(100);
    digitalWrite(speaker, 1000);
    delay(100);
  }
  delay(1000);
}
