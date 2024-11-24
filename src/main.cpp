#include <Arduino.h>

/*  MIT License

    Copyright (c) 2024 Kostelidis.dev

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

// Platform Information Properties (Read-only)
#define PLATFORM_NAME "PedestrianSense"
#define PLATFORM_VERSION "1.0.0"
#define PLATFORM_BUILD_DATE __DATE__ ", " __TIME__
#define PLATFORM_DEVELOPER "Iordanis Kostelidis <iordkost@ihu.gr>"
#define PLATFORM_UNIVERSITY "International Hellenic University"
#define PLATFORM_DEPARTMENT "Department of Computer, Informatics and Telecommunications Engineering"
#define PLATFORM_ACADEMIC_PROGRAM "MSc in Robotics"
//

// System Properties (Read-only)
#define SERIAL_BAUD 9600
#define BUZZER_SPEAKER_FREQUENCY_HZ 523
#define VEHICLE_GREEN_LIGHT_PIN 2
#define VEHICLE_YELLOW_LIGHT_PIN 3
#define VEHICLE_RED_LIGHT_PIN 4
#define BUZZER_SPEAKER_PIN 5
#define PEDESTRIAN_GREEN_LIGHT_PIN 6
#define PEDESTRIAN_RED_LIGHT_PIN 7
#define ULTRASONIC_TRIGGER_A_PIN 8
#define ULTRASONIC_TRIGGER_B_PIN 9
#define ULTRASONIC_ECHO_A_PIN 10
#define ULTRASONIC_ECHO_B_PIN 11
#define ULTRASONIC_DISTANCE_THRESHOLD 5
#define VEHICLE_RED_DURATION 60
#define VEHICLE_YELLOW_DURATION 10
#define VEHICLE_GREEN_DURATION UINTMAX_MAX
#define PEDESTRIAN_GREEN_LIGHT_DELAY 2
//

constexpr char VEHICLE_RED = 'R';
constexpr char VEHICLE_YELLOW = 'Y';
constexpr char VEHICLE_GREEN = 'G';

void setupSerial();

void setupTrafficLights();

void setupBuzzer();

void setupUltrasonic();

void setupVehicleLights();

void setupPedestrianLights();

bool isPedestrianDetected();

void setTrafficLightMode(char mode);

char getNextTrafficLightMode(char currentMode);

uintmax_t getDurationOfMode(char currentMode);

void trafficLights();

void setVehicleLights(unsigned int red, unsigned int yellow, unsigned int green);

void setPedestrianLights(unsigned int red, unsigned int green);

void playBuzzer();

void stopBuzzer();

unsigned long getDistanceFromUltrasonicDuration(unsigned long duration);

unsigned long getUltrasonicDurationBySensor(unsigned int trigger, unsigned int echo);

unsigned long getUltrasonicDistanceBySensor(unsigned int trigger, unsigned int echo);

void printPlatformInfo();

void delaySeconds(unsigned long s);

void setup()
{
    setupSerial();
    setupTrafficLights();
    setupBuzzer();
    setupUltrasonic();
    printPlatformInfo();
}

void loop()
{
    trafficLights();
}

void setupSerial()
{
    Serial.begin(SERIAL_BAUD);
}

void setupTrafficLights()
{
    setupVehicleLights();
    setupPedestrianLights();
}

void setupBuzzer()
{
    pinMode(BUZZER_SPEAKER_PIN, OUTPUT);
}

void setupUltrasonic()
{
    constexpr unsigned int ultrasonicTriggerPins[] = {
        ULTRASONIC_TRIGGER_A_PIN,
        ULTRASONIC_TRIGGER_B_PIN
    };
    constexpr unsigned int ultrasonicEchoPins[] = {
        ULTRASONIC_ECHO_A_PIN,
        ULTRASONIC_ECHO_B_PIN
    };
    for (const unsigned int ultrasonicTriggerPin : ultrasonicTriggerPins)
    {
        pinMode(ultrasonicTriggerPin, OUTPUT);
    }
    for (const unsigned int ultrasonicEchoPin : ultrasonicEchoPins)
    {
        pinMode(ultrasonicEchoPin, INPUT);
    }
}

void setupVehicleLights()
{
    constexpr unsigned int vehicleTrafficLightPins[] = {
        VEHICLE_RED_LIGHT_PIN,
        VEHICLE_YELLOW_LIGHT_PIN,
        VEHICLE_GREEN_LIGHT_PIN
    };
    for (const unsigned int vehicleTrafficLightPin : vehicleTrafficLightPins)
    {
        pinMode(vehicleTrafficLightPin, OUTPUT);
    }
}

void setupPedestrianLights()
{
    constexpr unsigned int pedestrianTrafficLightPins[] = {
        PEDESTRIAN_RED_LIGHT_PIN,
        PEDESTRIAN_GREEN_LIGHT_PIN
    };
    for (const unsigned int pedestrianTrafficLightPin : pedestrianTrafficLightPins)
    {
        pinMode(pedestrianTrafficLightPin, OUTPUT);
    }
}

bool isPedestrianDetected()
{
    const unsigned long distanceA = getUltrasonicDistanceBySensor(
        ULTRASONIC_TRIGGER_A_PIN,
        ULTRASONIC_ECHO_A_PIN
    );
    const unsigned long distanceB = getUltrasonicDistanceBySensor(
        ULTRASONIC_TRIGGER_B_PIN,
        ULTRASONIC_ECHO_B_PIN
    );

    if (distanceA <= ULTRASONIC_DISTANCE_THRESHOLD)
    {
        return true;
    }

    if (distanceB <= ULTRASONIC_DISTANCE_THRESHOLD)
    {
        return true;
    }

    return false;
}

void setTrafficLightMode(const char mode)
{
    if (mode == VEHICLE_RED)
    {
        setVehicleLights(HIGH, LOW, LOW);
        delaySeconds(PEDESTRIAN_GREEN_LIGHT_DELAY);
        setPedestrianLights(LOW, HIGH);
        return;
    }

    if (mode == VEHICLE_YELLOW)
    {
        setVehicleLights(LOW, HIGH, LOW);
        setPedestrianLights(HIGH, LOW);
        return;
    }

    setPedestrianLights(HIGH, LOW);
    delaySeconds(PEDESTRIAN_GREEN_LIGHT_DELAY);
    setVehicleLights(LOW, LOW, HIGH);
}

char getNextTrafficLightMode(const char currentMode)
{
    if (currentMode == VEHICLE_GREEN)
    {
        return VEHICLE_YELLOW;
    }

    if (currentMode == VEHICLE_YELLOW)
    {
        return VEHICLE_RED;
    }

    return VEHICLE_GREEN;
}

uintmax_t getDurationOfMode(const char currentMode)
{
    if (currentMode == VEHICLE_RED)
    {
        return VEHICLE_RED_DURATION;
    }

    if (currentMode == VEHICLE_YELLOW)
    {
        return VEHICLE_YELLOW_DURATION;
    }

    return VEHICLE_GREEN_DURATION;
}

[[noreturn]] void trafficLights()
{
    uintmax_t currentTime = 0;
    char currentMode = VEHICLE_GREEN;
    uintmax_t currentModeDuration = getDurationOfMode(currentMode);
    setTrafficLightMode(currentMode);

    bool pedestrianDetected = false;

    while (true)
    {
        const bool pedestrianDetectedFromSensor = isPedestrianDetected();

        if (pedestrianDetected && pedestrianDetectedFromSensor)
        {
            playBuzzer();
            if (currentMode == VEHICLE_RED)
            {
                currentTime = 0;
            }
            if (currentMode == VEHICLE_YELLOW)
            {
            }
            if (currentMode == VEHICLE_GREEN)
            {
                currentTime = currentModeDuration;
            }
        }
        else
        {
            stopBuzzer();
        }

        pedestrianDetected = pedestrianDetectedFromSensor;

        if (currentTime == currentModeDuration)
        {
            currentMode = getNextTrafficLightMode(currentMode);
            currentModeDuration = getDurationOfMode(currentMode);
            setTrafficLightMode(currentMode);
            currentTime = 0;
        }

        delaySeconds(1);
        currentTime += 1;
    }
}

void setVehicleLights(const unsigned int red, const unsigned int yellow, const unsigned int green)
{
    digitalWrite(VEHICLE_RED_LIGHT_PIN, red);
    digitalWrite(VEHICLE_YELLOW_LIGHT_PIN, yellow);
    digitalWrite(VEHICLE_GREEN_LIGHT_PIN, green);
}

void setPedestrianLights(const unsigned int red, const unsigned int green)
{
    digitalWrite(PEDESTRIAN_RED_LIGHT_PIN, red);
    digitalWrite(PEDESTRIAN_GREEN_LIGHT_PIN, green);
}

void playBuzzer()
{
    tone(BUZZER_SPEAKER_PIN, BUZZER_SPEAKER_FREQUENCY_HZ);
}

void stopBuzzer()
{
    noTone(BUZZER_SPEAKER_PIN);
}

unsigned long getDistanceFromUltrasonicDuration(const unsigned long duration)
{
    return duration / 29 / 2;
}

unsigned long getUltrasonicDurationBySensor(const unsigned int trigger, const unsigned int echo)
{
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
    return pulseIn(echo, HIGH);
}

unsigned long getUltrasonicDistanceBySensor(const unsigned int trigger, const unsigned int echo)
{
    return getDistanceFromUltrasonicDuration(
        getUltrasonicDurationBySensor(
            trigger, echo
        )
    );
}

void printPlatformInfo()
{
    Serial.println(PLATFORM_NAME);
    Serial.print(" version\t\t");
    Serial.println(PLATFORM_VERSION);
    Serial.print(" build date\t\t");
    Serial.println(PLATFORM_BUILD_DATE);
    Serial.print(" developer\t\t");
    Serial.println(PLATFORM_DEVELOPER);
    Serial.print(" university\t\t");
    Serial.println(PLATFORM_UNIVERSITY);
    Serial.print(" department\t\t");
    Serial.println(PLATFORM_DEPARTMENT);
    Serial.print(" academic program\t");
    Serial.println(PLATFORM_ACADEMIC_PROGRAM);
}

void delaySeconds(const unsigned long s)
{
    delay(s * 1000);
}
