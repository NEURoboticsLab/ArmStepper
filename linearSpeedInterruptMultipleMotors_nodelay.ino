
#define X_DIR_PIN          2
#define X_STEP_PIN         3
//#define X_ENABLE_PIN       

#define Y_DIR_PIN          4
#define Y_STEP_PIN         5
//#define Y_ENABLE_PIN       

#define Z_DIR_PIN          6
#define Z_STEP_PIN         7
//#define Z_ENABLE_PIN       

#define A_DIR_PIN          8
#define A_STEP_PIN         9
//#define A_ENABLE_PIN       

#define B_DIR_PIN          10
#define B_STEP_PIN         11
//#define B_ENABLE_PIN       

#define C_DIR_PIN          12
#define C_STEP_PIN         13
//#define C_ENABLE_PIN       

#define X_STEP_HIGH             PORTD |=  0b00001000;
#define X_STEP_LOW              PORTD &= ~0b00001000;

#define Y_STEP_HIGH             PORTD |=  0b00100000;
#define Y_STEP_LOW              PORTD &= ~0b00100000;

#define Z_STEP_HIGH             PORTD |=  0b10000000;
#define Z_STEP_LOW              PORTD &= ~0b10000000;

#define A_STEP_HIGH             PORTB |=  0b00000010;
#define A_STEP_LOW              PORTB &= ~0b00000010;

#define B_STEP_HIGH             PORTB |=  0b00001000;
#define B_STEP_LOW              PORTB &= ~0b00001000;

#define C_STEP_HIGH             PORTB |=  0b00100000;
#define C_STEP_LOW              PORTB &= ~0b00100000;

#define TIMER1_INTERRUPTS_ON    TIMSK1 |=  (1 << OCIE1A);
#define TIMER1_INTERRUPTS_OFF   TIMSK1 &= ~(1 << OCIE1A);

#define DELAY_VALUE 500

struct stepperInfo {
  // externally defined parameters
  float acceleration;
  volatile unsigned int minStepInterval;   // ie. max speed, smaller is faster
  void (*dirFunc)(int);
  void (*stepFunc)();

  // derived parameters
  unsigned int c0;                // step interval for first step, determines acceleration
  long stepPosition;              // current position of stepper (total of all movements taken so far)

  // per movement variables (only changed once per movement)
  volatile int dir;                        // current direction of movement, used to keep track of position
  volatile unsigned int totalSteps;        // number of steps requested for current movement
  volatile bool movementDone = false;      // true if the current movement has been completed (used by main program to wait for completion)
  volatile unsigned int rampUpStepCount;   // number of steps taken to reach either max speed, or half-way to the goal (will be zero until this number is known)

  // per iteration variables (potentially changed every interrupt)
  volatile unsigned int n;                 // index in acceleration curve, used to calculate next interval
  volatile float d;                        // current interval length
  volatile unsigned long di;               // above variable truncated
  volatile unsigned int stepCount;         // number of steps completed in current movement
};

unsigned long previousMicros = 0;
const long interval = 8;
bool stepState = false;

void xStep() {
    unsigned long currentMicros = micros();
    int i;
    for(i=0;i<=2;i++){
      if (currentMicros - previousMicros >= interval) {
        previousMicros = currentMicros;
  
        if (stepState == false) {
          X_STEP_HIGH;
          stepState = true;
        } else {
          X_STEP_LOW;
          stepState = false;
        }
      }
     }
}
void xDir(int dir) {
  digitalWrite(X_DIR_PIN, dir);
}

void yStep() {
  unsigned long currentMicros = micros();
  int i;
  for(i=0;i<=2;i++){
      if (currentMicros - previousMicros >= interval) {
        previousMicros = currentMicros;
  
        if (stepState == false) {
          Y_STEP_HIGH;
          stepState = true;
        } else {
          Y_STEP_LOW;
          stepState = false;
        }
      }
     }
}
void yDir(int dir) {
  digitalWrite(Y_DIR_PIN, dir);
}

void zStep() {
  unsigned long currentMicros = micros();
  int i;
  for(i=0;i<=2;i++){
      if (currentMicros - previousMicros >= interval) {
        previousMicros = currentMicros;
  
        if (stepState == false) {
          Z_STEP_HIGH;
          stepState = true;
        } else {
          Z_STEP_LOW;
          stepState = false;
        }
      }
     }
}
void zDir(int dir) {
  digitalWrite(Z_DIR_PIN, dir);
}

void aStep() {
  unsigned long currentMicros = micros();
  int i;
  for(i=0;i<=2;i++){
      if (currentMicros - previousMicros >= interval) {
        previousMicros = currentMicros;
  
        if (stepState == false) {
          A_STEP_HIGH;
          stepState = true;
        } else {
          A_STEP_LOW;
          stepState = false;
        }
      }
     }
}
void aDir(int dir) {
  digitalWrite(A_DIR_PIN, dir);
}

void bStep() {
  unsigned long currentMicros = micros();
  int i;
  for(i=0;i<=2;i++){
      if (currentMicros - previousMicros >= interval) {
        previousMicros = currentMicros;
  
        if (stepState == false) {
          B_STEP_HIGH;
          stepState = true;
        } else {
          B_STEP_LOW;
          stepState = false;
        }
      }
     }
}
void bDir(int dir) {
  digitalWrite(B_DIR_PIN, dir);
}

void cStep() {
  unsigned long currentMicros = micros();
  int i;
  for(i=0;i<=2;i++){
      if (currentMicros - previousMicros >= interval) {
        previousMicros = currentMicros;
  
        if (stepState == false) {
          C_STEP_HIGH;
          stepState = true;
        } else {
          C_STEP_LOW;
          stepState = false;
        }
      }
     }
}
void cDir(int dir) {
  digitalWrite(C_DIR_PIN, dir);
}

void resetStepperInfo( stepperInfo& si ) {
  si.n = 0;
  si.d = 0;
  si.di = 0;
  si.stepCount = 0;
  si.rampUpStepCount = 0;
  si.totalSteps = 0;
  si.stepPosition = 0;
  si.movementDone = false;
}

#define NUM_STEPPERS 6

volatile stepperInfo steppers[NUM_STEPPERS];

void setup() {

  pinMode(X_STEP_PIN,   OUTPUT);
  pinMode(X_DIR_PIN,    OUTPUT);
//  pinMode(X_ENABLE_PIN, OUTPUT);

  pinMode(Y_STEP_PIN,   OUTPUT);
  pinMode(Y_DIR_PIN,    OUTPUT);
//  pinMode(Y_ENABLE_PIN, OUTPUT);

  pinMode(Z_STEP_PIN,   OUTPUT);
  pinMode(Z_DIR_PIN,    OUTPUT);
 // pinMode(Z_ENABLE_PIN, OUTPUT);

  pinMode(A_STEP_PIN,   OUTPUT);
  pinMode(A_DIR_PIN,    OUTPUT);
//  pinMode(A_ENABLE_PIN, OUTPUT);

  pinMode(B_STEP_PIN,   OUTPUT);
  pinMode(B_DIR_PIN,    OUTPUT);
//  pinMode(B_ENABLE_PIN, OUTPUT);

  pinMode(C_STEP_PIN,   OUTPUT);
  pinMode(C_DIR_PIN,    OUTPUT);
 // pinMode(C_ENABLE_PIN, OUTPUT);

//  digitalWrite(X_ENABLE_PIN, LOW);
//  digitalWrite(Y_ENABLE_PIN, LOW);
//  digitalWrite(Z_ENABLE_PIN, LOW);
//  digitalWrite(A_ENABLE_PIN, LOW);
//  digitalWrite(B_ENABLE_PIN, LOW);
//  digitalWrite(C_ENABLE_PIN, LOW);

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 1000;                             // compare value
  TCCR1B |= (1 << WGM12);                   // CTC mode
  TCCR1B |= ((1 << CS11) | (1 << CS10));    // 64 prescaler
  interrupts();

  steppers[0].dirFunc = bDir;
  steppers[0].stepFunc = bStep;
  steppers[0].acceleration = 1000;
  steppers[0].minStepInterval = 100;

  steppers[1].dirFunc = aDir;
  steppers[1].stepFunc = aStep;
  steppers[1].acceleration = 4000;
  steppers[1].minStepInterval = 100;

  steppers[2].dirFunc = cDir;
  steppers[2].stepFunc = cStep;
  steppers[2].acceleration = 1000;
  steppers[2].minStepInterval = 100;

  steppers[3].dirFunc = xDir;
  steppers[3].stepFunc = xStep;
  steppers[3].acceleration = 4000;
  steppers[3].minStepInterval = 100;

  steppers[4].dirFunc = yDir;
  steppers[4].stepFunc = yStep;
  steppers[4].acceleration = 1000;
  steppers[4].minStepInterval = 100;

  steppers[5].dirFunc = zDir;
  steppers[5].stepFunc = zStep;
  steppers[5].acceleration = 4000;
  steppers[5].minStepInterval = 450;
}

void resetStepper(volatile stepperInfo& si) {
  si.c0 = si.acceleration;
  si.d = si.c0;
  si.di = si.d;
  si.stepCount = 0;
  si.n = 0;
  si.rampUpStepCount = 0;
  si.movementDone = false;
}

volatile byte remainingSteppersFlag = 0;

void prepareMovement(int whichMotor, int steps) {
  volatile stepperInfo& si = steppers[whichMotor];
  si.dirFunc( steps < 0 ? HIGH : LOW );
  si.dir = steps > 0 ? 1 : -1;
  si.totalSteps = abs(steps);
  resetStepper(si);
  remainingSteppersFlag |= (1 << whichMotor);
}

volatile byte nextStepperFlag = 0;

volatile int ind = 0;
volatile unsigned int intervals[100];

void setNextInterruptInterval() {

  bool movementComplete = true;

  unsigned int mind = 999999;
  for (int i = 0; i < NUM_STEPPERS; i++) {
    if ( ((1 << i) & remainingSteppersFlag) && steppers[i].di < mind ) {
      mind = steppers[i].di;
    }
  }

  nextStepperFlag = 0;
  for (int i = 0; i < NUM_STEPPERS; i++) {
    if ( ! steppers[i].movementDone )
      movementComplete = false;

    if ( ((1 << i) & remainingSteppersFlag) && steppers[i].di == mind )
      nextStepperFlag |= (1 << i);
  }

  if ( remainingSteppersFlag == 0 ) {
    OCR1A = 65500;
  }

  OCR1A = mind;
}

ISR(TIMER1_COMPA_vect)
{
  unsigned int tmpCtr = OCR1A;

  OCR1A = 65500;

  for (int i = 0; i < NUM_STEPPERS; i++) {

    if ( ! ((1 << i) & remainingSteppersFlag) )
      continue;

    if ( ! (nextStepperFlag & (1 << i)) ) {
      steppers[i].di -= tmpCtr;
      continue;
    }

    volatile stepperInfo& s = steppers[i];

    if ( s.stepCount < s.totalSteps ) {
      s.stepFunc();
      s.stepCount++;
      s.stepPosition += s.dir;
      if ( s.stepCount >= s.totalSteps ) {
        s.movementDone = true;
        remainingSteppersFlag &= ~(1 << i);
      }
    }

    if ( s.rampUpStepCount == 0 ) {
      s.n++;
      s.d = s.d - (2 * s.d) / (4 * s.n + 1);
      if ( s.d <= s.minStepInterval ) {
        s.d = s.minStepInterval;
        s.rampUpStepCount = s.stepCount;
      }
      if ( s.stepCount >= s.totalSteps / 2 ) {
        s.rampUpStepCount = s.stepCount;
      }
    }
    else if ( s.stepCount >= s.totalSteps - s.rampUpStepCount ) {
      s.d = (s.d * (4 * s.n + 1)) / (4 * s.n + 1 - 2);
      s.n--;
    }

    s.di = s.d; // integer
  }

  setNextInterruptInterval();

  TCNT1  = 0;
}

void runAndWait() {
  setNextInterruptInterval();
  while ( remainingSteppersFlag );
}

void loop() {

  TIMER1_INTERRUPTS_ON

  for (int i = 0; i < 4; i++) {
    for (int k = 0; k < NUM_STEPPERS; k++) {
      prepareMovement( k,  200 );
      runAndWait();
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int k = 0; k < NUM_STEPPERS; k++) {
      prepareMovement( k,  200 );
    }
    runAndWait();
  }
  
  for (int i = 0; i < NUM_STEPPERS; i++)
    prepareMovement( i, 400 );
  runAndWait();
  for (int i = 0; i < NUM_STEPPERS; i++)
    prepareMovement( i, -400 );
  runAndWait();
  for (int i = 0; i < NUM_STEPPERS; i++)
    prepareMovement( i, 200 );
  runAndWait();
  for (int i = 0; i < NUM_STEPPERS; i++)
    prepareMovement( i, -200 );
  runAndWait();
  for (int i = 0; i < NUM_STEPPERS; i++)
    prepareMovement( i, 600 );
  runAndWait();
  for (int i = 0; i < NUM_STEPPERS; i++)
    prepareMovement( i, -600 );
  runAndWait();

  while (true);

}
