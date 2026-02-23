#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// important constants
const int NUM_PINS = 16;
const int MIN_BRIGHTNESS = 100;
const int MAX_BRIGHTNESS = 4000;

// create PWM object at address 0x40 with default i2c pins
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// global array to track pin levels
int levels[NUM_PINS];

// lighting functions
void one_LED(Adafruit_PWMServoDriver pwm, int levels[], int pin0, int speed);
void two_LED(Adafruit_PWMServoDriver pwm, int levels[], int pin0, int pin1, int speed);
void three_LED(Adafruit_PWMServoDriver pwm, int levels[], int pin0, int pin1, int pin2, int speed);
void spiral_LED(Adafruit_PWMServoDriver pwm, int levels[], int speed);

// setup function, runs once
void setup()
{
  // probably necessary
  Serial.begin(9600);

  // initialize PWM controller
  pwm.begin();

  // set PWM frequency to 1000Hz to reduce flicker
  pwm.setPWMFreq(1000);

  // set seed for random
  randomSeed(1803119713);

  // set all LEDs to 6.25% duty cycle
  for(int i = 0; i < NUM_PINS; i++)
  {
    pwm.setPWM(i, 0, MIN_BRIGHTNESS);
    levels[i] = MIN_BRIGHTNESS;
  }
}

// loop function, manages lighting
void loop()
{
  // select 3 random pins
  int pin0 = random(0, NUM_PINS);
  int pin1 = random(0, NUM_PINS);
  while(pin1 == pin0)
  {
    pin1 = random(0, NUM_PINS);
  }
  int pin2 = random(0, NUM_PINS);
  while(pin2 == pin0 || pin2 == pin1)
  {
    pin2 = random(0, NUM_PINS);
  }

  // generate random number to set transition speed
  int speed = random(3, 6);

  // generate random number to pick logic branch
  int branch = random(0, 100);

  // light control branches
  if(branch == 0)
  {
    spiral_LED(pwm, levels, speed);
  }
  else
  {
    one_LED(pwm, levels, pin0, speed);
  }
}

/*
void two_LED(Adafruit_PWMServoDriver pwm, int pin0, int pin1, int speed)
{

}

void three_LED(Adafruit_PWMServoDriver pwm, int pin0, int pin1, int pin2, int speed)
{

}
*/

// change one LED
void one_LED(Adafruit_PWMServoDriver pwm, int levels[], int pin0, int speed)
{
  if(levels[pin0] == MAX_BRIGHTNESS)
  {
    for(int i = 0; i < (MAX_BRIGHTNESS - MIN_BRIGHTNESS)/50; i++)
    {
      pwm.setPWM(pin0, 0, levels[pin0] - 50);
      levels[pin0] -= 50;
      delay(speed * 4);
    }
  }
  else
  {
    for(int i = 0; i < (MAX_BRIGHTNESS - MIN_BRIGHTNESS)/50; i++)
    {
      pwm.setPWM(pin0, 0, levels[pin0] + 50);
      levels[pin0] += 50;
      delay(speed * 4);
    }
  }
}

void two_LED(Adafruit_PWMServoDriver pwm, int pin0, int pin1, int speed)
{

}

// good thing that you shouldnt fuck up
void spiral_LED(Adafruit_PWMServoDriver pwm, int levels[], int speed)
{
  // gradually increase all LEDs to max
  for(int i = 0; i < NUM_PINS; i++)
  {
    for(int j = 0; j < (MAX_BRIGHTNESS - levels[i]) / 50; j++)
    {
      pwm.setPWM(i, 0, levels[i] + 50);
      levels[i] += 50;
      delay(speed * 4);
    }
  }

  // instantly decrease all LEDS to min
  for(int i = 0; i < NUM_PINS; i++)
  {
    pwm.setPWM(i, 0, MIN_BRIGHTNESS);
    levels[i] = MIN_BRIGHTNESS;
    delay(speed * 50);
  }
}