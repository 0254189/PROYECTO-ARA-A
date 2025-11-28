#include <Servo.h>

// =========================
//   CONFIGURACION GENERAL
// =========================
const int NUM_LEGS = 4;
const int SERVOS_PER_LEG = 3;
const int NUM_SERVOS = NUM_LEGS * SERVOS_PER_LEG;

const int HIP   = 0;
const int THIGH = 1;
const int KNEE  = 2;

Servo servos[NUM_SERVOS];

int servoPins[NUM_SERVOS] = {
  2, 3, 4,
  5, 6, 7,
  8, 9, 10,
  11, 12, 13
};

// TUS NEUTRALES
int neutralAngle[NUM_SERVOS] = {
  120,130,40,
  55, 55, 30,
  55, 60, 60,
  120,130,20
};

// RANGOS (THIGH FIJO)
int minAngle[NUM_SERVOS] = {
  60,130,0,
  0, 55,0,
  0, 60,0,
  60,130,0
};

int maxAngle[NUM_SERVOS] = {
  180,130,90,
  100,55,90,
  100,60,90,
  180,130,90
};

int currentAngle[NUM_SERVOS];
int velocidad = 20;   // más bajo = más rápido


// =========================
// AUXILIARES
// =========================
int sid(int leg, int joint) {
  return leg * SERVOS_PER_LEG + joint;
}

// Movimiento rápido (no tan granular)
void moveServoFast(int servoId, int target) {
  if (target < minAngle[servoId]) target = minAngle[servoId];
  if (target > maxAngle[servoId]) target = maxAngle[servoId];

  servos[servoId].write(target);
  currentAngle[servoId] = target;
  delay(100);
}


// =========================
// MOVER 2 PATAS A LA VEZ
// =========================
void movePairSimult(int legA, int legB) {

  // IDs
  int AH = sid(legA, HIP);
  int AK = sid(legA, KNEE);

  int BH = sid(legB, HIP);
  int BK = sid(legB, KNEE);

  int AN = neutralAngle[AH];
  int KN_A = neutralAngle[AK];

  int BN = neutralAngle[BH];
  int KN_B = neutralAngle[BK];

  int hipOffset = 25;
  int kneeLift  = 20;
  int kneeDown  = 10;

  // Dirección izquierda / derecha
  int AF = (legA == 0 || legA == 2) ? AN + hipOffset : AN - hipOffset;
  int AB = (legA == 0 || legA == 2) ? AN - hipOffset : AN + hipOffset;

  int BF = (legB == 0 || legB == 2) ? BN + hipOffset : BN - hipOffset;
  int BB = (legB == 0 || legB == 2) ? BN - hipOffset : BN + hipOffset;

  // ===============================
  // 1) LEVANTAR RODILLAS AL MISMO TIEMPO
  // ===============================
  moveServoFast(AK, KN_A + kneeLift);
  moveServoFast(BK, KN_B + kneeLift);

  // ===============================
  // 2) MOVER CADERAS ADELANTE JUNTAS
  // ===============================
  moveServoFast(AH, AF);
  moveServoFast(BH, BF);

  // ===============================
  // 3) BAJAR RODILLAS JUNTAS
  // ===============================
  moveServoFast(AK, KN_A - kneeDown);
  moveServoFast(BK, KN_B - kneeDown);

  // ===============================
  // 4) RETROCESO DE CADERAS
  // ===============================
  moveServoFast(AH, AB);
  moveServoFast(BH, BB);

  // ===============================
  // 5) REGRESAR TODO A NEUTRO
  // ===============================
  moveServoFast(AH, AN);
  moveServoFast(AK, KN_A);

  moveServoFast(BH, BN);
  moveServoFast(BK, KN_B);
}


// =========================
// SETUP
// =========================
void setup() {
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
    currentAngle[i] = neutralAngle[i];
    servos[i].write(neutralAngle[i]);
  }

  delay(800);
}


// =========================
// LOOP
// =========================
void loop() {

  // PAR A: 0 + 3 (cruzados)
  movePairSimult(0, 3);
  delay(200);

  // PAR B: 1 + 2 (cruzados)
  movePairSimult(1, 2);
  delay(200);
}
