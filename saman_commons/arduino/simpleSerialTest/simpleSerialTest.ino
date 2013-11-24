
// constants won't change. Used here to 
// set pin numbers:
const int ledPin =  13;      // the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

void setup() {
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);    
  Serial.begin(9600);  
}

void loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
  while(Serial.available()){
    char charRead = Serial.read();
    if(charRead=='a') ledState = HIGH;
    if(charRead=='q') ledState = LOW;
    
    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    
  }
  
}

