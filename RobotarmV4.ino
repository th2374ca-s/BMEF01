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

//Axelrotation
float axrMin;
float axrMax;

//Armbågerotation
float abMax;
float abMin;

//Handledböj
float hlbMax;
float hlbMin;

//Handledrot
float hlrMax;
float hlrMin;

//Gripklo
float gpMax;
float gpMin;

// Om den har blivit kaliberad
int calibrateCheck = 0;

// Knapp
int buttonState = 0; //Läser av knapp
int const buttonPin = 2;

// Analogportar i en lista
int analogPins[] = {A5, A4, A3, A1, A2, A0};

// Olika states
int state;
const int sleeve = 1;
const int arm = 2;
const int startUp = 3;
const int calibrate = 4;
  //Felsökning
const int analogReading = 8;

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

    // Ändra position på armen från data som tas i sleeve
    case arm:
      Serial.println("CASE ARM");     
      while(true) {
        gripklo.write(map(analogRead(A0), gpMin, gpMax, 139, 50));
        handledRot.write(map(analogRead(A1), hlrMin, hlrMax, 0, 179));
        handledBoj.write(map(analogRead(A2), hlbMin, hlbMax, 60, 130));
        armboge.write(map(analogRead(A3), abMin, abMax, 87, 12));
        axelRot.write(map(analogRead(A4), axrMin, axrMax, 51, 151));
        axelBoj.write(map(analogRead(A5), axbMin, axbMax, 8, 99));
        Serial.print("Ny Position");
        if (digitalRead(buttonPin) == HIGH) {
            printAngle();
            delay(1000);                    
          }
      }
      state = calibrate;

    break;

    // kalibera armen min och max läge för varje led
    case calibrate:
      Serial.println("CASE CALIBRATE");
      
      Serial.println("Nu ska armen kaliberas");
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

// Väntar knapp intryck
void waitForButton(int pin) {
  while(digitalRead(pin) == LOW) {
        delay(100);
      }
}

// Skriver ut vinklar för alla servon
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
