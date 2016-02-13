
#include <math.h>

#define RAD_TO_DEG 57.2957786
#define PI 3.14159265

// This is the power connector for the accelerometer.
// The reason we have plugged one side into an analog pin
// instead of to "power" is because we want a very steady voltage to
// be sent to the accelerometer.
// That way, when we read the value from the other side of the accelerometer,
// we can accurately calculate a voltage drop.
int power = A5;

// Analog read pins
const int xInputPin = A0;
const int yInputPin = A1;
const int zInputPin = A2;

// Analog write pins;
/*const int xOutPin = D0;
const int yOutPin = D1;
const int zOutPin = D2;
*/
// The minimum and maximum values that came from
// the accelerometer while standing still
// You very well may need to change these
int minVal = 1606;
int maxVal = 2488;

// to hold the calculated values
double x;
double y;
double z;

int xSlide;
int ySlide;
int zSlide;

void setup() {

    // we'll use this to measure our output in degrees for the x axis
    // that will light up the LED on an analog slide
    /*pinMode(xOutPin, OUTPUT);
    pinMode(yOutPin, OUTPUT);
    pinMode(zOutPin, OUTPUT);*/

    // Accepting x, y, z input from the accelerometer
    pinMode(xInputPin, INPUT);
    pinMode(yInputPin, INPUT);
    pinMode(zInputPin, INPUT);

    // The pin powering the accelerometer is output (sending out consistent power)
    pinMode(power, OUTPUT);

    // Next, write one pin of the accelerometer to be the max possible,
    // so that we can use this for power.
    digitalWrite(power, HIGH);

    // We are going to declare a Particle.variable() here so
    // that we can access the value of the accelerometer from the cloud.
    Particle.variable("xSlide", &xSlide, INT);
    Particle.variable("ySlide", &ySlide, INT);
    Particle.variable("zSlide", &zSlide, INT);
    // This is saying that when we ask the cloud for "xSlide", this
    // will reference the variable xSlide in this app,
    // which is an integer variable.
}

void loop() {

    // Read the analog values from the accelerometer
    int xRead = analogRead(xInputPin);
    int yRead = analogRead(yInputPin);
    int zRead = analogRead(zInputPin);

    // Convert read values to degrees -90 to 90 - Needed for atan2
    int xAng = getAngle(xRead);
    int yAng = getAngle(yRead);
    int zAng = getAngle(zRead);

    // Calculate 360deg values like so: atan2(-yAng, -zAng)
    // atan2 outputs the value of -pi to pi (radians)
    // We are then converting the radians to degrees
    x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
    y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
    z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

    xSlide = getSlideValueFromDegrees(x);
    ySlide = getSlideValueFromDegrees(y);
    zSlide = getSlideValueFromDegrees(z);
    delay (100);
}

int getAngle(int value) {
    return map(value, minVal, maxVal, -90, 90);
}

int getSlideValueFromDegrees(int degrees) {
    return map(degrees, 1, 360, 0, 255);
}
