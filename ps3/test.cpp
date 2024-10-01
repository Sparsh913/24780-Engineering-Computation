#include <iostream>
#include <math.h>
using namespace std;

int main(void)
{
    // Data table representing climb rates
    double table[]=
    {
        830.0, 770.0, 705.0, 640.0,
        720.0, 655.0, 595.0, 535.0,
        645.0, 585.0, 525.0, 465.0,
        530.0, 475.0, 415.0, 360.0,
        420.0, 365.0, 310.0, 250.0,
        310.0, 255.0, 200.0, 145.0
    };

    double temp_array[4] = {-20, 0, 20, 40};    // Temperature in degrees Celsius (x-axis)
    double altitude_array[6] = {0, 2000, 4000, 6000, 8000, 10000}; // Altitude in feet (y-axis)

    int altitude = 0;
    int temp = 0;
    
    // Input loop to gather valid altitude and temperature values
    while (true) 
    {
        cout << "Enter the altitude (0ft to 10000ft) and temperature (-20C to 40C): " << endl;
        cin >> altitude >> temp;

        // Check if the inputs are within the valid range
        if (altitude < 0 || altitude > 10000 || temp < -20 || temp > 40) 
        {
            cout << "Invalid input. Please enter altitude between 0 and 10000 ft, and temperature between -20 and 40 C." << endl;
            continue; // Ask for input again
        }
        else 
        {
            break; // Valid input, exit the loop
        }
    }

    // Calculate the index for the table
    double altitudeIndex = (double) altitude / 2000; // 0, 1, 2, 3, 4, 5 (based on 2000ft intervals)
    double tempIndex = ((double) temp + 20) / 20;   // 0, 1, 2, 3 (based on -20C to 40C in 20-degree intervals)

    // Calculate lower and upper bounds for both altitude and temperature
    int lowerAltitudeIndex = floor(altitudeIndex);
    int upperAltitudeIndex = lowerAltitudeIndex + 1; // Next altitude level
    int lowerTempIndex = floor(tempIndex);
    int upperTempIndex = lowerTempIndex + 1;         // Next temperature level
    
    // Clamp the upper bounds to ensure they stay within the array limits
    if (upperAltitudeIndex > 5) upperAltitudeIndex = 5, lowerAltitudeIndex = 4; // Ensure lower index is also clamped
    if (upperTempIndex > 3) upperTempIndex = 3, lowerTempIndex = 2; // Ensure lower index is also clamped

    // Retrieve the climb rate values from the table for interpolation
    double f00 = table[lowerAltitudeIndex * 4 + lowerTempIndex]; // Bottom-left value
    double f01 = table[lowerAltitudeIndex * 4 + upperTempIndex]; // Bottom-right value
    double f10 = table[upperAltitudeIndex * 4 + lowerTempIndex]; // Top-left value
    double f11 = table[upperAltitudeIndex * 4 + upperTempIndex]; // Top-right value

    // Get the corresponding temperature and altitude values at the bounding indices
    double x1 = temp_array[lowerTempIndex];
    double x2 = temp_array[upperTempIndex];
    double y1 = altitude_array[lowerAltitudeIndex];
    double y2 = altitude_array[upperAltitudeIndex];

    // If the input is at the exact boundary, no need for interpolation
    if (x1 == x2 && y1 == y2) {
        // Direct lookup
        cout << "Expected Climb Rate = " << f00 << " ft/min" << endl;
        return 0;
    }

    // Interpolation in x direction
    double r1 = (x2 - temp) / (x2 - x1) * f00 + (temp - x1) / (x2 - x1) * f01;
    double r2 = (x2 - temp) / (x2 - x1) * f10 + (temp - x1) / (x2 - x1) * f11;

    // Interpolation in y direction
    double interpolatedValue = (y2 - altitude) / (y2 - y1) * r1 + (altitude - y1) / (y2 - y1) * r2;

    cout << "Expected Climb Rate = " << interpolatedValue << " ft/min" << endl;

    return 0;
}
