/*
Moisture and Temp

Read Moisture and report on OLED :)

Jan 31 2020
*/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(128, 32, &Wire, OLED_RESET);

  int Moistureval;
  int MoistDisplay;
  int Mval;
  int Tempval;
  float tempF;
  float tempC;
  String strMHeading  = "Moist: ";
  String strTHeading  = "Temp : ";
  char strAsterisks[22] = "*********************";
  char strDots[22]      = "....................."; 
  char MoistLine[22]    = "NA";
  bool   Debug = true;

/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 



int substring(char *source, int from, int n, char *target){
    int length,i;
    //get string length 
    for(length=0;source[length]!='\0';length++);
    
    if(from>length){
        printf("Starting index is invalid.\n");
        return 1;
    }
    
    if((from+n)>length){
        //get substring till end
        n=(length-from);
    }
    
    //get substring in target
    for(i=0;i<n;i++){
        target[i]=source[from+i];
    }
    target[i]='\0'; //assign null at last
    
    return 0;    
}


void setup() 
{
  Serial.begin(9600); // open serial port, set the baud rate as 9600 bps

 // Start up the library 
  sensors.begin(); 
 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // If OLED fails, Don't proceed, loop forever
  }
  display.display(); // display the adafruit splash screen
  delay(2000); // Pause for 2 seconds
}

void loop() 
{

  sensors.requestTemperatures(); // Send the command to get temperature readings 

  Moistureval = analogRead(7); //connect Moist sensor to Analog 7
  // Set a limited range for the Moisture Value Mv
  Mval = (Moistureval - 350) / 12.8; 
  MoistDisplay  = Mval;
 if (MoistDisplay > 21) 
    { 
      MoistDisplay = 21;
      if (Debug) {Serial.print("Mv=21");}
    };
  if (MoistDisplay <= 0) 
    { 
      MoistDisplay = 1;
      if (Debug) {Serial.print("Mv=0");}
    };
   MoistDisplay = 22 - MoistDisplay;  // Invert these values
//   strlcpy(MoistLine, strAsterisks, MoistDisplay); 
//   MoistLine = strAsterisks.Substring(0,5);
   substring(strAsterisks,0,MoistDisplay,MoistLine);
   if (Debug) {Serial.print(MoistLine);}
 // Serial.println(val); //print the value to serial port
 // 

 display.clearDisplay();
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(0,0);
 display.println("-- Moisture Sensor --");

// Get the value from the first moisture sensor 
 tempC = sensors.getTempCByIndex(0);
 if (Debug) {Serial.print(tempC);}
 if (Debug) {Serial.print("C  ");}
 tempF = (tempC * 1.8 ) + 32.0;
 Serial.print(tempF);
 //Serial.print(sensors.getTempCByIndex(0));
 Serial.print("F "); 
 if (Debug) {Serial.print(" M  Value: ");} 
 if (Debug) {Serial.print(Moistureval);}
 if (Debug) {Serial.print(" M Display: ");}
 if (Debug) {Serial.print(MoistDisplay);}
 if (Debug) {Serial.print(" Mval: ");}
 if (Debug) {Serial.print(Mval);}
 if (Debug) {Serial.print(" \n");}
// Build & Display the Temp line
String strTDisplayText = strTHeading + tempF + " F";
display.println(strTDisplayText);

// Build & Display the Temp lines
String strMDisplayText = strMHeading + Moistureval;
display.println(strMDisplayText);

display.println(MoistLine);
display.display();  // display the updated screen

  delay(1000);
}
