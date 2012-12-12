// Hand Scanner Program for Arduino Mega ATMega1280
// Coded By: Jean-Luc Desroches
// Last Modification Date: November 14, 2012
// Credits: DHT11 code scripts by Camel, from DFRobot Wiki site

#include <Wire.h> 
#include <LiquidCrystal_I2C.h> // include control library for LCD
#define dht11_pin 54 //Define pin location for dht11 temperature and humidity sensor
#define distPin A10
#define slider1 A6
#define slider2 A7


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27(digital pin 20) for a 16 chars and 2 line display
int distVal, tempVal, humidVal, distance, val1, val2, slideVal1, slideVal2, slideMenu, dht11_humidity, dht11_temp;  //declare all integer variables used within the program
String stringLine1, stringLine2, stringCode;  //declare all string variables within the program
boolean leave; // define a boolean to control exit commands within while loops








// Begin Set-up
void setup()
{
  // Run activation sequence
  activate();
  // Initialize variables
  distVal = 450;
  tempVal = 900;
  humidVal = 930;
  leave=false;
  
  // Camel's script to initialize DHT11 temperature and humidity sensor
  pinMode(dht11_pin, OUTPUT);
  digitalWrite(dht11_pin, HIGH);
  // End Camel's code
  
  Serial.begin(9600);
}
// End Set-up










//Run a decorative activation sequence on the LCD screen
void activate(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);                   
  lcd.print("Scanner");
  lcd.setCursor(0,1);
  lcd.print("Activating");
  delay(350);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Scanner");
  lcd.setCursor(0,1);
  lcd.print("Activating.");
  delay(350);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Scanner");
  lcd.setCursor(0,1);
  lcd.print("Activating..");
  delay(350);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Scanner");
  lcd.setCursor(0,1);
  lcd.print("Activating...");
  delay(350);
  lcd.clear();
  delay(150);
  lcd.setCursor(0,0);
  lcd.print("Scanner");
  lcd.setCursor(0,1);
  lcd.print("Active");
  delay(1000);
  lcd.clear();
}
// End Activation sequence








//Camel's DHT11 script
byte read_dht11_dat()
{
  byte i = 0;
  byte result=0;
  for(i=0; i< 8; i++)
  {
    while (!digitalRead(dht11_pin));
    delayMicroseconds(30);
    if (digitalRead(dht11_pin) != 0 )
      bitSet(result, 7-i);
    while (digitalRead(dht11_pin));
  }
  return result;
}
// End of Camel's script







// Read laser range data script from arduino playground
float read_gp2d12_range(byte A10) {
	int tmp;

	tmp = analogRead(A10);
	if (tmp < 3)
		return -1; // invalid value

	return (6787.0 /((float)tmp - 3.0)) - 4.0;
// End Sensor scan
} 








int slider() {
  //Get slider values
  slideVal1=analogRead(slider1);      //Read slider value from analog 6
  slideVal2=analogRead(slider2);      //Read slider value from analog 7
  slideMenu=((slideVal1+slideVal2)/(2));  //Find average of readings
  return (slideVal1);
  //
}







void wait(int count, int top, int bottom){
  while (count < 500){
    count=count+10;
    slideMenu=slider();
    if (slideMenu >= top || slideMenu < bottom) {
      return;
    }
    delay(10);
  }
}





void distscanner() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Distance:");
  distance = read_gp2d12_range(distPin);
  lcd.setCursor(0,1);
  stringLine1 = " cm";
  stringCode = distance + stringLine1;
  lcd.print(stringCode);
  wait(0,distVal,0);
    
}







void humidscanner() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Humidity:");

  //DHT11 scripting by Camel to collect temperature and humidity data   
  byte dht11_dat[5];   
  byte dht11_in;
  byte i;// start condition
  digitalWrite(dht11_pin, LOW);
  delay(18);
  digitalWrite(dht11_pin, HIGH);
  delayMicroseconds(1);
  pinMode(dht11_pin, INPUT);
  delayMicroseconds(200);     
  for (i=0; i<5; i++)
    {  dht11_dat[i] = read_dht11_dat();}  //recieved 40 bits data. Details are described in datasheet
  pinMode(dht11_pin, OUTPUT);
  digitalWrite(dht11_pin, HIGH);
  // End of Camel's script
    
  lcd.setCursor(0,1);
  stringLine1 = " %";
  stringCode = dht11_dat[0] + stringLine1;
  lcd.print(stringCode);
  wait(0,humidVal,distVal);
    
}






void tempscanner() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperature:");
  byte dht11_dat[5];   
  byte dht11_in;
  byte i;// start condition

  //DHT11 scripting by Camel to collect temperature and humidity data   
  digitalWrite(dht11_pin, LOW);
  delay(18);
  digitalWrite(dht11_pin, HIGH);
  delayMicroseconds(1);
  pinMode(dht11_pin, INPUT);
  delayMicroseconds(40);     
  for (i=0; i<5; i++)
    {  dht11_dat[i] = read_dht11_dat();}  //recieved 40 bits data. Details are described in datasheet
  pinMode(dht11_pin, OUTPUT);
  digitalWrite(dht11_pin, HIGH);
  // End of Camel's script
  
  lcd.setCursor(0,1);
  stringLine1 = "C";
  stringCode = dht11_dat[2]+ stringLine1;
  lcd.print(stringCode);
  wait(0,tempVal,humidVal);

}






//Begin Main loop
void loop()
{
  //Get slider values
  slideMenu = slider();
  Serial.print(slideMenu);
  Serial.print("\n");
  delay(1000);
  //
  
  // if slider between 0 and 256 execute distance scanning code
  if (slideMenu <=distVal) {
    distscanner();
    slideMenu = 0;
  }
  
  // End distance sensor script
  
  // if slider between 512 and 256 execute humidity scanning code
  else if (slideMenu <=humidVal && slideMenu > distVal) {
    humidscanner();
    slideMenu = 0;
  }
  //end of humidity sensing script
  
  // if slider between 768 and 512 execute humidity scanning code
  else if (slideMenu <=tempVal && slideMenu > humidVal) {
    tempscanner();
    slideMenu = 0;
  }
  //end of temperature sensing script
}
// End main loop

