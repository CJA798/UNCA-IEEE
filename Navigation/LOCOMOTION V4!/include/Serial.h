
// macros //
#include <stdio.h>
#include <string.h>
using namespace std;

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
#define INPUT_QUALIFIER "put: "
#define MESSAGE_LENGTH 5
char buffer[BUFFER_SIZE - 1];
String Message;
int Command = 0;
char SerialState = RECEIVING;
int rd = 0, wr, n; // rd = read, wr = write, n = number of bytes in buffer
const int led_pin = 13;
int led_on_time = 0;
// USBSerial Class==========================================================================================================
class USBSerialMaster
{
public:
    int SerialWriteCounter = 0;
    USBSerialMaster()
    {
        // string Message;
        SOFTWARE_SERIAL.begin(SOFTWARE_SERIAL_BAUD);
        // HARDWARE_SERIAL.begin(HARDWARE_SERIAL_BAUD);
        digitalWrite(RESET_PIN, HIGH);
        pinMode(RESET_PIN, OUTPUT);
    };
    void SerialProcess(void)
    {
        if (Serial.available() == 0)
        {
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
        //  HARDWARE_SERIAL.write(buffer, MessageSize);
        Serial.print("Message: ");
        Serial.println(Message);
        if (Message.indexOf(INPUT_QUALIFIER))
        {
            int pos = Message.lastIndexOf('\n');
            Command = Message[pos - 1];
            Serial.print("Command: ");
            Serial.println(Command);
        }
        Message = "";
    };
};
