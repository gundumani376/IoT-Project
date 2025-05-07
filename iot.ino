#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
const int trig1 = 3;
const int echo1 = 5;
const int trig2 = 4;
const int echo2 = 6;
const int buzzerPin = 10;
static const int RXPin = 4, TXPin = 3; // GPS TX → Arduino RX (pin 4), GPS RX → Arduino TX (pin 3)
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);
void setup() {
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  Serial.begin(115200);
  gpsSerial.begin(GPSBaud);
  Serial.println("Initializing GPS Tracker...");
}
long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}
void loop() {
   while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
    if (gps.location.isUpdated()) {
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);

      Serial.print("Speed (km/h): ");
      Serial.println(gps.speed.kmph());
      Serial.print("Satellites: ");
      Serial.println(gps.satellites.value());
      Serial.print("Accuracy (HDOP): ");
      Serial.println(gps.hdop.hdop());
      Serial.print("Timestamp: ");
      if (gps.time.isValid()) {
        Serial.print(gps.time.hour());
        Serial.print(":");
        Serial.print(gps.time.minute());
        Serial.print(":");
        Serial.println(gps.time.second());
      } else {
        Serial.println("Invalid");
      }
      Serial.println("-----------");
    }
  }
  long dist1 = getDistance(trig1, echo1);
  long dist2 = getDistance(trig2, echo2);
  long distance = min(dist1, dist2); // Use the nearer object
  if (distance < 100) {
    int freq = map(distance, 5, 100, 1000, 200);       // Higher freq when closer
    freq = constrain(freq, 200, 1000);
    int delayTime = map(distance, 5, 100, 100, 600);   // Faster beeping when closer
    tone(buzzerPin, freq);
    delay(100);           
    noTone(buzzerPin);
    delay(delayTime);
  } else {
    noTone(buzzerPin);
    delay(300);
  }
  Serial.print("Dist1: "); Serial.print(dist1);
  Serial.print(" cm | Dist2: "); Serial.print(dist2);
  Serial.print(" cm | Used: "); Serial.print(distance);
  Serial.print(" cm | Freq: "); Serial.print(map(distance, 5, 100, 1000, 200));
  Serial.print(" Hz | Delay: "); Serial.println(map(distance, 5, 100, 100, 600));
}
void setup() {
}
