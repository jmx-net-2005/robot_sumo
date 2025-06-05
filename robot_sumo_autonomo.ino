// Pines del motor A
const int AIN1 = 4;
const int AIN2 = 5;
const int PWMA = 9;

// Pines del motor B
const int BIN1 = 6;
const int BIN2 = 7;
const int PWMB = 10;

// Standby común
const int STBY = 8;

// Sensor ultrasónico
const int trigPin = 12;
const int echoPin = 11;

// Sensores infrarrojos (DO)
const int sensorIzq = 2;
const int sensorDer = 3;

// Distancia umbral (cm)
const int UMBRAL_CM = 50;

void setup() {
  // Motores
  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT); pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT); pinMode(PWMB, OUTPUT);

  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);  // Activar controlador

  // Sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Sensores infrarrojos
  pinMode(sensorIzq, INPUT);
  pinMode(sensorDer, INPUT);

  Serial.begin(9600);
}

void loop() {
  // Leer sensores infrarrojos
  int valIzq = digitalRead(sensorIzq);  // 0: blanco, 1: negro
  int valDer = digitalRead(sensorDer);

  // Leer sensor ultrasónico
  long duracion;
  int distancia;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duracion = pulseIn(echoPin, HIGH);
  distancia = duracion * 0.034 / 2;

  // Imprimir datos
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.print(" cm | IR IZQ: ");
  Serial.print(valIzq);
  Serial.print(" | IR DER: ");
  Serial.println(valDer);

  // ---------------------- LÓGICA DE MOVIMIENTO ------------------------

if (valIzq == 0) {
  // ✅ IR izquierdo blanco → ambos motores retroceden 500 ms
  digitalWrite(AIN1, LOW); digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW); digitalWrite(BIN2, HIGH);
  analogWrite(PWMA, 150); analogWrite(PWMB, 150);
  delay(100);
}
else if (valDer == 0) {
  // ✅ IR derecho blanco → solo motor A retrocede, B adelante
  digitalWrite(AIN1, HIGH); digitalWrite(AIN2, LOW); // motor A reversa
  digitalWrite(BIN1, HIGH); digitalWrite(BIN2, LOW); // motor B adelante
  analogWrite(PWMA, 200); analogWrite(PWMB, 200);
}
else if (distancia < UMBRAL_CM && valDer == 1 && valIzq == 1) {
  // ✅ Obstáculo cerca + ambos sensores IR en negro → avanzar
  digitalWrite(AIN1, HIGH); digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH); digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 180); analogWrite(PWMB, 180);
}
else if (distancia >= UMBRAL_CM) {
  // ✅ No hay obstáculo → motor A en reversa, B adelante
  digitalWrite(AIN1, LOW); digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH); digitalWrite(BIN2, LOW);
  analogWrite(PWMA, 100); analogWrite(PWMB, 100);
}
else {
  // Caso no contemplado → detener motores
  digitalWrite(AIN1, LOW); digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW); digitalWrite(BIN2, LOW);
}

delay(100);

}
