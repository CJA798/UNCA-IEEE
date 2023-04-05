
/*
void setMovedBot();
void setColumn();

void setRecycling();
void setPond();
void getColumn(double* dest);
void getPond(double* dest);
void getRecycling(double* dest);
*/
#define FILLED (500)
#define MAX_ROTATION (360)
#include "math.h"

class Map
{
private:
    int columns[2][2];   // Positions of the columns
    int pond[2];         // X, Y Position of the inside of the pond.
    int recycling[2][2]; // Two by two of the positions in recycling
    int position[3];     // X, Y, Theta

    /**/
public:
    Map()
    {

        for (int i = 0; i < 2; i++)
        {
            pond[i] = position[i] = 0;
            for (int j = 0; j < 2; j++)
            {
                columns[i][j] = recycling[i][j] = 0;
            }
        }
        position[2] = 0;
    }

    /**/
    void setColumn()
    {
        if (columns[0][0] == 0)
        {
            columns[0][0] = position[0];
            columns[0][1] = position[1];
        }
        else if (columns[1][0] == 0 && position[0] < columns[0][0] + 16)
        { // Second check is to see if we are trying to store the same column twice.
            columns[1][0] = position[0];
            columns[1][1] = position[1];
        }
    }

    void setMovedBot(int xPos, int yPos, int theta)
    {
        position[0] += xPos;
        position[1] += yPos;
        position[2] += theta;

        if (position[2] > MAX_ROTATION)
        {                                             // Can be changed for radians
            position[2] = position[2] - MAX_ROTATION; // Resets theta if we've gone a full 360 degrees.
                                                      // Assuming that the bot facing to the right is 0 degrees
        }
    }

    void setRecycling()
    {
        if (recycling[0][0] == 0)
        {
            recycling[0][0] = position[0];
            recycling[0][1] = position[1];
        }
        else if (recycling[1][0] == 0 && recycling[0][1] + 15 < position[1])
        {
            recycling[1][0] = position[0];
            recycling[1][1] = position[1];
        }
    }

    void setPond()
    {
        pond[0] = position[0];
        pond[1] = position[1];
    }

    void getColumn(double *dest)
    {

        if (columns[0][0] != FILLED && columns[0][1] != FILLED)
        { // We are considering that 0 if "Filled" for now.
            // Maybe we use a new integer that can't be reached to be considered as "Filled" then we can use zero as not filled, and we create
            //  a helper function that can put the bot in a general area to search for the circle.

            dest[0] = sqrt(pow(columns[0][0] - position[0], 2) + pow(columns[0][1] - position[1], 2));
            dest[1] = ((double)columns[0][1] - (double)position[1]) / ((double)columns[0][0] - (double)position[0]);
            dest[1] = thetaHelper(dest[1], columns[0][0] - position[0], columns[0][1] - position[1]);

            columns[0][0] = FILLED;
            columns[0][1] = FILLED; // Setting this column to "Filled"
        }
        else if (columns[1][0] != FILLED && columns[1][1] != FILLED)
        { // First Column is filled
            dest[0] = sqrt((pow(columns[1][0] - position[0], 2) + pow(columns[1][1] - position[0], 2)));
            dest[1] = (columns[1][1] - position[1]) / (columns[1][0] - position[0]);

            dest[1] = thetaHelper(dest[1], columns[1][0] - position[0], columns[1][1] - position[1]);

            columns[1][0] = FILLED;
            columns[1][1] = FILLED; // Setting this column to "Filled"
        }
    }

    void getPond(double *dest)
    {

        if (pond[0] != FILLED && pond[1] != FILLED)
        { // We are considering that 0 if "Filled" for now.
            // Maybe we use a new integer that can't be reached to be considered as "Filled" then we can use zero as not filled, and we create
            //  a helper function that can put the bot in a general area to search for the circle.

            dest[0] = sqrt((pow(pond[0] - position[0], 2) + pow(pond[1] - position[1], 2)));
            dest[1] = ((double)pond[1] - (double)position[1]) / ((double)pond[0] - (double)position[0]);

            dest[1] = thetaHelper(dest[1], pond[0] - position[0], pond[1] - position[1]); // Finds the angle that needs to be turned for the bot.

            pond[0] = FILLED;
            pond[1] = FILLED; // Setting this column to "Filled"
        }
    }

    void getRecycling(double *dest)
    {

        if (recycling[0][0] != FILLED && recycling[0][1] != FILLED)
        { // We are considering that 0 if "Filled" for now.
            // Maybe we use a new integer that can't be reached to be considered as "Filled" then we can use zero as not filled, and we create
            //  a helper function that can put the bot in a general area to search for the circle.

            dest[0] = sqrt((pow(recycling[0][0] - position[0], 2) + pow(recycling[0][1] - position[1], 2)));
            dest[1] = ((double)recycling[0][1] - (double)position[1]) / ((double)recycling[0][0] - (double)position[0]);

            dest[1] = thetaHelper(dest[1], recycling[0][0] - position[0], recycling[0][1] - position[1]); // Finds the angle that needs to be turned for the bot.

            recycling[0][0] = 0;
            recycling[0][1] = 0; // Setting this column to "Filled"
        }
        else if (recycling[0][0] != FILLED && recycling[0][1] != FILLED)
        {
            dest[0] = sqrt((pow(recycling[1][0] - position[0], 2) + pow(recycling[1][1] - position[1], 2)));
            dest[1] = ((double)recycling[1][1] - (double)position[1]) / ((double)recycling[1][0] - (double)position[0]);

            dest[1] = thetaHelper(dest[1], recycling[1][0] - position[0], recycling[1][1] - position[1]); // Finds the angle that needs to be turned for the bot.

            recycling[1][0] = FILLED;
            recycling[1][1] = FILLED; // Setting this column to "Filled"
        }
    }

private:
    double thetaHelper(double invTan, int xDist, int yDist)
    {
        if (yDist < 0 && xDist < 0)
        {                                                             // Quad 3 Adjustment
            invTan = atan(invTan) * (180 / M_PI) + 180 - position[2]; // Adjusts for the current theta of the bot
                                                                      // Check to see if we want the position to be reset from 0 or if we want to go from where we are.
        }
        if (yDist > 0 && xDist < 0)
        { // Quad 2 Adjustment
            invTan = atan(invTan) * (180 / M_PI) - 180 - position[2];
        }
        if (yDist == 0 && xDist < 0)
        { // 180 or 0 adjustment
            invTan = 180 - position[2];
        }
        if (yDist == 0 && xDist > 0)
        {
            invTan = -1 * position[2];
        }
        if (yDist < 0 && xDist == 0)
        { // 90 or 270 adjustment
            invTan = 90 - position[2];
        }
        if (yDist == 0 && xDist < 0)
        {
            invTan = 270 - position[2];
        }
        return invTan;
    }
};
