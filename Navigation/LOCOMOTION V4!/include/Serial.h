
// macros //
#include <stdio.h>
#include <string.h>
using namespace std;
bool debug = false;
#include <Ports.h>
#define RECEIVING 'R'
#define TRANSMITTING 'T'
// #define HARDWARE_SERIAL Serial1
#define SOFTWARE_SERIAL Serial
#define SOFTWARE_SERIAL_BAUD 115200
#define HARDWARE_SERIAL_BAUD 115200
#define BUFFER_SIZE 90
#define EXECUTING 'E'
#define SAFE_BUFFER_SIZE 80 // Use this to avoid buffer overflow(2 bits for padding)
#define INPUT_QUALIFIER "input: "
#define MESSAGE_LENGTH 8
#define DISSCONNECTED 'D'
#define CONNECTED 'C'
char buffer[BUFFER_SIZE - 1];
String Message;
char Command = 0;
char SerialState = DISSCONNECTED;
int rd = 0, wr, n; // rd = read, wr = write, n = number of bytes in buffer
const int led_pin = 13;
int led_on_time = 0;
bool FirstTime = false;
// USBSerial Class==========================================================================================================
class USBSerialMaster
{
public:
    int SerialWriteCounter = 0;
    USBSerialMaster()
    {
        SOFTWARE_SERIAL.begin(SOFTWARE_SERIAL_BAUD);
        digitalWrite(RESET_PIN, HIGH);
        pinMode(RESET_PIN, OUTPUT);
    };
    void SerialProcess(void)
    {
        if (Serial.available() == 0)
        {
            SerialState = DISSCONNECTED;
            return;

        }; // Turn on LED if data is being received.
        Message = Serial.readStringUntil('\n');
        delay(100);
        if (millis() - led_on_time > 3)
        {
            digitalWrite(led_pin, LOW);
        }; // Turn off LED if it has been on for too long.
        Serial.flush();
        delay(100);
        if (debug == true)
        {
            Serial.print("Message: ");
            Serial.println(Message);
        };

        Command = Message[MESSAGE_LENGTH - 1];
        if (debug == true)
        {
            Serial.print("Command: ");
            Serial.println(Command);
        };

        Message = "";
        SerialState = CONNECTED;
    };
};
