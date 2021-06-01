#include <stdio.h>
#define button 2
#define v_location A3
int v_setting = 0, correct = 0, setting_button = 0, location = 0, i = 0;
int password[3][3], input[3];


int set_password(int password[3][3]);
int check_password(int password[3][3]);
void unlock();
void lock();
void alarm();


void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(v_location, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
}

void loop() {

  while (1) {
    if (v_setting == 0)
    {
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
          break;
          delay(1000);
        }
        delay(10);
      }

      v_setting = 1;
      continue;
    }



     
    if (v_setting == 1)
    {
      location = analogRead(v_location);
      i = 0;
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
      }else {
          correct = 0;
        }


      if (correct == 1)
      {
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
        correct=0;
        continue;
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
  Serial.println("Unlock");
}
void lock()
{
  Serial.println("Lock");
  delay(1000);
}
void alarm()
{
  Serial.println("Alarm");
  delay(1000);
}
