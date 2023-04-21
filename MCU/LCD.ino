#include <Wire.h>  // include the Wire library for I2C communication
#include <LiquidCrystal_I2C.h>  // include the LiquidCrystal_I2C library for the LCD display

LiquidCrystal_I2C lcd(0x27, 16, 1);  // set the LCD address to 0x27 and the display size to 16x2

void setup() {
  lcd.init();  // initialize the LCD display
  lcd.backlight();  // turn on the backlight
  lcd.autoscroll();  // enable auto-scrolling of the text
  lcd.print("Hello World. This is a long text to test the auto-scrolling feature of the LCD display.");  // print a long text to test the auto-scrolling feature

}

void loop() {
  // the loop function does nothing in this example
  delay(1000);
  lcd.scrollDisplayLeft();
}
