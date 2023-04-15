
class Photoresistor
{
private:
    Photoresistor(int PhotoresistorPin)
    {
        bool FirstTime = true;
        pinMode(PhotoresistorPin, INPUT);
    };

public:
    bool PhotoresistorChange(void)
    {
        int PhotoresistSum1;
        int PhotoresistSum2;
        if (FirstTime) // take an average of 10 readings if it is the first time we are running this
        {
            for (int i = 0; i < 10; i++)
            {
                PhotoresistSum1 += analogRead(PHOTORESISTOR_PIN);
                delay(1);
            };
        }
        else
        {
            for (int i = 0; i < 10; i++)
            { // take an average of 10 readings (for comparison) if we are are not running this for the first time.
                PhotoresistSum2 += analogRead(PHOTORESISTOR_PIN);
                delay(1);
            };
        };
        FirstTime = false;
        PhotoresistSum2 = 0;
        if (PhotoresistSum2 > PhotoresistSum1 + LED_ON_THRESHOLD)
        {
            return true;
        }
        else
        {
            return false;
        };
    };
};