//ATCOMMAND CODE
//void setup() {
// Serial.begin(9600);
// Serial1.begin(38400);
// Serial.println("pro macro redy");
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  if (Serial1.available())
//  {
//    Serial.write(Serial1.read());
//  }
//
//  if (Serial.available())
//  {
//    Serial1.write(Serial.read());
//  }
//
//}

  #include "Keyboard.h"
  
  char next = KEY_RIGHT_ARROW;
  char previous = KEY_LEFT_ARROW;
  char shift = KEY_LEFT_SHIFT;
  char f5 = KEY_F5;
  char escape = KEY_ESC;
  bool fsflag = true;
  

  void setup()
  {
    Serial.begin(115200);
    Serial1.begin(38400);
    Keyboard.begin();
    Serial.println("reciever redy");
  }

  void enter_full_screen()
  {
    if (fsflag)
    {
      Keyboard.press(shift);
      Keyboard.press(f5);
      delay(50);
      Keyboard.releaseAll();
      fsflag = false;
      Serial.println("entering fs");
      return;
    }
    if (!fsflag)
    {
      Keyboard.press(escape);
      delay(50);
      Keyboard.releaseAll();
      fsflag = true;
      Serial.println("Exiting fs");
      return;
    }
  }

  void loop ()
  {
    if (Serial1.available())
    {
      String text = Serial1.readStringUntil('\n');
      text.trim();
      Serial.println(text);
      if (text == "NEXT")
      {
       Keyboard.press(next); 
       delay(100);
       Keyboard.releaseAll();
       Serial.println("NEXT");    
      }
      if (text == "PREVIOUS")
      {
       Keyboard.press(previous);
       delay(50);
       Keyboard.releaseAll();
       Serial.println("NEXT");      
      }
      if (text == "FULLSCREEN")
      {
       Serial.println("ff success");
       enter_full_screen();
      }
      if (text == "NUL")
      {
       Keyboard.releaseAll();
      }
      delay(50);
    } 
  }
