#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// define important constants
const int NUM_PINS = 16;          // number of pins in project 1-16
const int INCREMENT = 50;         // minimum brightness adjustment
const int MIN_BRIGHTNESS = 100;   // must be less than max and multiple of increment
const int MAX_BRIGHTNESS = 4000;  // must be greater than min and multiple of increment

// create PWM object at address 0x40 with default i2c pins for atmega328p
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// create global array to track pin levels
int levels[NUM_PINS];

// declare lighting functions
void single_LED(Adafruit_PWMServoDriver pwm, int levels[], int pin, int speed);
void spiral_LED(Adafruit_PWMServoDriver pwm, int levels[], int speed);

// setup function, runs once
void setup()
{
  // probably necessary
  Serial.begin(9600);

  // initialize PWM controller
  pwm.begin();

  // set PWM frequency to 1000Hz to reduce flicker
  pwm.setPWMFreq(1600);

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
  // select a random pin
  int pin = random(0, NUM_PINS);

  // generate random number to set transition speed
  int speed = random(3, 6);

  // generate random number to pick logic branch
  int branch = random(0, 500);

  // light control branches
  if(branch == 0)
  {
    spiral_LED(pwm, levels, speed);
  }
  else
  {
    single_LED(pwm, levels, pin, speed);
  }
}

// change single LED
void single_LED(Adafruit_PWMServoDriver pwm, int levels[], int pin, int speed)
{
  // decrease random amount from max brightness
  if(levels[pin] == MAX_BRIGHTNESS)
  {
    int range = (MAX_BRIGHTNESS - MIN_BRIGHTNESS)/INCREMENT;
    int travel = random(0, range);

    for(int i = 0; i < travel; i++)
    {
      pwm.setPWM(pin, 0, levels[pin] - INCREMENT);
      levels[pin] -= INCREMENT;
      delay(speed * 4);
    }
  }
  // increase random amount from min brightness
  else if(levels[pin] == MIN_BRIGHTNESS)
  {
    int range = (MAX_BRIGHTNESS - MIN_BRIGHTNESS)/INCREMENT;
    int travel = random(0, range);

    for(int i = 0; i < travel; i++)
    {
      pwm.setPWM(pin, 0, levels[pin] + INCREMENT);
      levels[pin] += INCREMENT;
      delay(speed * 4);
    }
  }
  // decrease or increase random amount from current brightness
  else
  {
    // decrease
    if(random(0,2) == 0)
    {
      int range = (levels[pin] - MIN_BRIGHTNESS)/INCREMENT;
      int travel = random(0, range);

      for(int i = 0; i < travel; i++)
      {
        pwm.setPWM(pin, 0, levels[pin] - INCREMENT);
        levels[pin] -= INCREMENT;
        delay(speed * 4);
      }
    }
    // increase
    else
    {
      int range = (MAX_BRIGHTNESS - levels[pin])/INCREMENT;
      int travel = random(0, range);

      for(int i = 0; i < travel; i++)
      {
        pwm.setPWM(pin, 0, levels[pin] + INCREMENT);
        levels[pin] += INCREMENT;
        delay(speed * 4);
      }
    }
  }
}

// gently increase all LEDs to max, then decrease to min
void spiral_LED(Adafruit_PWMServoDriver pwm, int levels[], int speed)
{
  // gradually increase all LEDs to max
  for(int i = 0; i < NUM_PINS; i++)
  {
    for(int j = 0; j < (MAX_BRIGHTNESS - levels[i]) / INCREMENT; j++)
    {
      pwm.setPWM(i, 0, levels[i] + INCREMENT);
      levels[i] += INCREMENT;
      delay(speed * 4);
    }
  }
  // instantly decrease all LEDS to min
  for(int i = 0; i < NUM_PINS; i++)
  {
    pwm.setPWM(i, 0, MIN_BRIGHTNESS);
    levels[i] = MIN_BRIGHTNESS;
    delay(speed * INCREMENT);
  }
}