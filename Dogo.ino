//biblioteki

#include <MotorDriver.h>
#include <Servo.h>

// chuj wie jak to nazwac

Servo myservo;
MotorDriver silnik;

// define'y

#define TYLNY_PRAWY 1
#define TYLNY_LEWY 4
#define PRZEDNI_PRAWY 2
#define PRZEDNI_LEWY 3
#define LEWY_PIR_PIN A4
#define PRAWY_PIR_PIN A3

// zmienne globalne
const int trigPin = A2;
const int echoPin = A1;
long duration;
int distance;
int pozycja_serwa = 0;
long czas = 0;
int lewy_pir = 0;
int prawy_pir = 0;
int tryb = 0;

//funkcje head

void skret_prawo();
void skret_lewo();
int pomiar_odleglosci ();
int serwo_prawo();
int serwo_lewo();

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEWY_PIR_PIN, INPUT);
  pinMode(PRAWY_PIR_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(PRAWY_PIR_PIN), prawy_pir_f, RISING);
  attachInterrupt(digitalPinToInterrupt(LEWY_PIR_PIN), lewy_pir_f, RISING);

  Serial.begin(9600);
  myservo.attach(9);
}

void loop() {
  pozycja_serwa = 90; // pozycja 0
  myservo.write(pozycja_serwa);

  if ((prawy_pir == 1) || (lewy_pir == 1))
  {
    tryb = 1;
  }
  else
{
  tryb = 0;
}

switch (tryb)
  {
    case 0: //obstacle avoiding
      {
        int odleglosc_aktualna = pomiar_odleglosci();

        if (odleglosc_aktualna > 30)
        {
          silnik.motor(TYLNY_PRAWY, FORWARD, 10);
          silnik.motor(TYLNY_LEWY, FORWARD, 10);
          silnik.motor(PRZEDNI_PRAWY, FORWARD, 10);
          silnik.motor(PRZEDNI_LEWY, FORWARD, 10);
        }
        else
        {
          silnik.motor(TYLNY_PRAWY, FORWARD, 0);
          silnik.motor(TYLNY_LEWY, FORWARD, 0);
          silnik.motor(PRZEDNI_PRAWY, FORWARD, 0);
          silnik.motor(PRZEDNI_LEWY, FORWARD, 0);

          int odleglosc_prawo = serwo_lewo();
          int odleglosc_lewo = serwo_prawo();

          if (odleglosc_prawo >= odleglosc_lewo)
          {
            skret_prawo();
          }
          else
          {
            skret_lewo();
          }
        }
        break;
      }//koniec case

    case 1: //dogo
      {
        if ((prawy_pir == 1) && (lewy_pir == 1))
        {
          silnik.motor(TYLNY_PRAWY, FORWARD, 10);
          silnik.motor(TYLNY_LEWY, FORWARD, 10);
          silnik.motor(PRZEDNI_PRAWY, FORWARD, 10);
          silnik.motor(PRZEDNI_LEWY, FORWARD, 10);
        }
        else if ((prawy_pir == 1) && (lewy_pir == 0))
        {
          silnik.motor(TYLNY_LEWY, FORWARD, 10);
          silnik.motor(PRZEDNI_LEWY, FORWARD, 10);
        }
        else if ((prawy_pir == 0) && (lewy_pir == 1))
        {
          silnik.motor(TYLNY_PRAWY, FORWARD, 10);
          silnik.motor(PRZEDNI_PRAWY, FORWARD, 10);
        }
        prawy_pir = 0;
        lewy_pir = 0;
        break;
      }//koniec case
  }//koniec switch

  delay(10);
}

//funkcje body
int pomiar_odleglosci ()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

int serwo_lewo()
{
  for (pozycja_serwa = 90; pozycja_serwa <= 160; pozycja_serwa++)
  {
    myservo.write(pozycja_serwa);
    delay(25);
  }

  int odleglosc_aktualna = pomiar_odleglosci();
  return odleglosc_aktualna;
}

int serwo_prawo()
{
  for (pozycja_serwa = 160; pozycja_serwa >= 20; pozycja_serwa--)
  {
    myservo.write(pozycja_serwa);
    delay(25);
  }

  int odleglosc_aktualna = pomiar_odleglosci();

  //powrot do pozycji 0
  for (pozycja_serwa = 20; pozycja_serwa <= 90; pozycja_serwa++)
  {
    myservo.write(pozycja_serwa);
    delay(25);
  }

  return odleglosc_aktualna;
}

void skret_lewo()
{
  silnik.motor(TYLNY_PRAWY, BACKWARD, 10);
  silnik.motor(PRZEDNI_PRAWY, BACKWARD, 10);
  delay(500);
}

void skret_prawo()
{
  silnik.motor(TYLNY_LEWY, BACKWARD, 10);
  silnik.motor(PRZEDNI_LEWY, BACKWARD, 10);
  delay(500);
}

void lewy_pir_f()
{
  lewy_pir = 1;
}

void prawy_pir_f()
{
  prawy_pir = 1;
}
