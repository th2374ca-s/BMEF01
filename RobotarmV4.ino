#include <Servo.h>
// Länk till bibliotektet
// https://www.arduino.cc/reference/en/libraries/servo/

/* Numering på tejpbitar
gripklo: 6
handledrot: 4
handledböj: 5
armbåge: 3
axelrot: 2
axelböj: 1
*/

// Servo motorer för varje led
Servo gripklo;
Servo handledRot;
Servo handledBoj;
Servo armboge;
Servo axelRot;
Servo axelBoj;

// Vinklar för leder (inklusive startvinkel)
float gripkloAngle;
float handledRotAngle;
float handledBojAngle;
float armbogeAngle;
float axelRotAngle;
float axelBojAngle;

// Variabler för potenimetre. min, max och viloläge för armen
//Axelböj
float axbMin;
float axbMax; 
float axbVilo;

//Axelrotation
float axrMin;
float axrMax;
float axrVilo;

//Armbågerotation
float abMax;
float abMin;
float abVilo;

//Handledböj
float hlbMax;
float hlbMin;
float hlbVilo;

//Handledrot
float hlrMax;
float hlrMin;
float hlrVilo;

//Gripklo
float gpMax;
float gpMin;
float gpVilo;

// Om den har blivit kaliberad
int calibrateCheck = 0;

// Knapp
int buttonState = 0; //Läser av knapp
int const buttonPin = 2;
//int knapp = 0;

// Analogportar i en lista
int analogPins[] = {A5, A4, A3, A1, A2, A0};

// Olika states
int state;
const int sleeve = 1;
const int arm = 2;
const int startUp = 3;
const int calibrate = 4;
const int calibrateVilo = 40;
  //Felsökning
const int analogReading = 8;
const int stopProgram = 9;

void setup() {

  pinMode(buttonPin, INPUT);
  gripklo.attach(11);
  handledRot.attach(10);
  handledBoj.attach(9);
  armboge.attach(6);
  axelRot.attach(5);
  axelBoj.attach(3);
  Serial.begin(9600);

  state = calibrate;


}

void loop() {
  switch(state){
    
     // Hämtar data från sleeven och gör om det till användbar data till armen
    // case sleeve:
    //   Serial.println("CASE SLEEVE");
    //   gripkloAngle = map(analogRead(A0), gpMin, gpMax, 0, 179);
    //   handledRotAngle = map(analogRead(A1), hlrMin, hlrMax, 0, 179);
    //   handledBojAngle = map(analogRead(A2), hlbMin, hlbMax, 0, 179);
    //   armbogeAngle = map(analogRead(A3), abMin, abMax, 0, 179);
    //   axelRotAngle = map(analogRead(A4), axrMin, axrMax, 0, 179);
    //   axelBojAngle = map(analogRead(A5), axbMin, axbMax, 0, 179);
    //   delay(10);

    //   //handledRotAngle = map(analogRead(A1), x, y, z, w);
    //   //handledBojAngle = map(analogRead(A2), x, y, z, w);
    //   //armbogeAngle = map(analogRead(A3), x, y, z, w);
    //   //axelRotAngle = map(analogRead(A4), x, y, z, w);
    //   //axelBojAngle = map(analogRead(A5), x, y, z, w);
    
    //   state = arm;
    //   break;

    // Ändra position på armen från data som tas i sleeve
    case arm:
      Serial.println("CASE ARM");
      while(true) {
        gripklo.write(map(analogRead(A0), gpMin, gpMax, 0, 179));
        handledRot.write(map(analogRead(A1), hlrMin, hlrMax, 0, 179));
        handledBoj.write(map(analogRead(A2), hlbMin, hlbMax, 0, 179));
        armboge.write(map(analogRead(A3), abMin, abMax, 87, 12));
        axelRot.write(map(analogRead(A4), axrMin, axrMax, 51, 151));
        axelBoj.write(map(analogRead(A5), axbMin, axbMax, 8, 99));
        //gripklo.write(map((float)constrain(analogRead(A0), gpMin, gpMax), gpMin, gpMax, 0, 179));
        //handledRot.write(map((float)constrain(analogRead(A1), hlrMin, hlrMax), hlrMin, hlrMax, 0, 179));
        //handledBoj.write(map((float)constrain(analogRead(A2), hlbMin, hlbMax), hlbMin, hlbMax, 0, 179));
        //armboge.write(map((float)constrain(analogRead(A3), abMin, abMax), abMin, abMax, 0, 179));
        //axelRot.write(map((float)constrain(analogRead(A4), axrMin, axrMax), axrMin, axrMax, 0, 179));
        //axelBoj.write(map((float)constrain(analogRead(A5), axbMin, axbMax), axbMin, axbMax, 0, 179));
        if (digitalRead(buttonPin) == HIGH) {
            printAngle();
            delay(1000);                    
          }
      }
      state = calibrate;

    break;

    // case startUp:
    //   // Sätter armen i neutralt läge.
    //   Serial.println("CASE STARTUP");
    //   gripklo.write(45);
    //   handledRot.write(45);
    //   handledBoj.write(45);
    //   armboge.write(45);
    //   axelRot.write(45);
    //   axelBoj.write(45);
    
    //   state = sleeve;
    //   break;

    case calibrate:
      Serial.println("CASE CALIBRATE");

      int part = 0;
      Serial.println("Ställ sleeve i viloläge");
        
      waitForButton(buttonPin);

      axbVilo = analogRead(A5);
      axrVilo = analogRead(A4);
      abVilo = analogRead(A3);
      hlbVilo = analogRead(A2);
      hlrVilo = analogRead(A1);
      gpVilo = analogRead(A0);
      
      Serial.println("Vilo-läge tagen");
      delay(1000);
      
      Serial.println("Nästa är Axelböj");
      calibrateServo(axbMin, axbMax, 1);
      Serial.println("Axelböj tagen");

      Serial.println("Nästa Axelrot");
      calibrateServo(axrMin, axrMax, 2);
      Serial.println("Axelrot tagen");

      Serial.println("Nästa armbåge");
      calibrateServo(abMin, abMax, 3);
      Serial.println("Armbåge tagen");

      Serial.println("Nästa handledRot");
      calibrateServo(hlrMin, hlrMax, 4);
      Serial.println("Handledrot tagen");

      Serial.println("Nästa handledböj");
      calibrateServo(hlbMin, hlbMax, 5);
      Serial.println("Handledböj tagen");

      Serial.println("Nästa gripklo");
      calibrateServo(gpMin, gpMax, 6);
      Serial.println("Gripklo tagen");

      Serial.println("Kalibrering klar");

      state = arm; 

      break;

    case analogReading:
      // Använder för att bestäma värden som används för att mapa vinklarna
      Serial.println("CASE ANALOGREADING");
      while(true){
        Serial.println("************************");
        Serial.println("UPPDATERAD INFO");
        Serial.print("Gripklo: ");
        Serial.println(analogRead(A0));

        Serial.print("Handled rot: ");
        Serial.println(analogRead(A1));

        Serial.print("Handled böj: ");
        Serial.println(analogRead(A2));

        Serial.print("Armbåge: ");
        Serial.println(analogRead(A3));

        Serial.print("Axelrot: ");
        Serial.println(analogRead(A4));

        Serial.print("Axelböj: ");
        Serial.println(analogRead(A5));
        Serial.println("SLUT MED INFO");
        delay(2000);
      }
      break;
  }
}

// Tar min och max värde från en potentiometer och lägger det i annan variabel.
void calibrateServo(float & min, float & max, int nbrPot){
  int analogPin = analogPins[nbrPot-1];
  int check = 0;
  int mode = 0;

  Serial.println("Ställ i minläge");
  waitForButton(buttonPin);
  min = analogRead(analogPin);
  Serial.println("Minläge taget");
  delay(1000);

  Serial.println("Ställ i maxläge");
  waitForButton(buttonPin);
  max = analogRead(analogPin);
  Serial.println("Maxläge taget");
  delay(1000);
}

void waitForButton(int pin) {
  while(digitalRead(pin) == LOW) {
        //Serial.println("Hej");
        //Serial.println(digitalRead(pin), DEC);
        delay(100);
      }
}
void printAngle() {
  Serial.print("gripklo: ");
  Serial.print(gripklo.read());

  Serial.println("");  
  
  Serial.print("handledRot: ");
  Serial.print(handledRot.read());

  Serial.println("");    
  
  Serial.print("handledBoj: ");
  Serial.print(handledBoj.read());

  Serial.println("");  

  Serial.print("armboge: ");
  Serial.print(armboge.read());

  Serial.println("");    

  Serial.print("axelRot: ");
  Serial.print(axelRot.read());

  Serial.println("");  

  Serial.print("axelBoj: ");
  Serial.print(axelBoj.read());
  
  Serial.println("");  
  Serial.println("");  
}
