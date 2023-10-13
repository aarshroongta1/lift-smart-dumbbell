# LIFT (smart dumbbell tracker)
LIFT (Linking Individuals for Fitness Together) is an IoT-based attachment for weightlifting equipment that provides valuable workout analytics.

## Circuit
<p> The following are the components and sensors used: <br />
    - Arduino MKR WIFI 1010 <br />
    - MPU6050 (accelerometer-cum-gyroscope) <br />
    - OLED SSD1306 (128x64 display) <br />
    - Piezo Buzzer <br /> </p>
    

Here is the circuit diagram:
<img width="884" alt="LIFT circuit diagram" src="https://github.com/aarshroongta1/lift-smart-dumbbell/assets/108404307/fe7865e7-694b-4ec9-afd6-d8219fc50238">

## Code Explanation
Only the X-axis readings from the accelerometer are used for the purpose of counting the basic bicep curls. As the starting orientation determines the angle values recorded, a condition is used to check it based on whether the first angle recorded is positive or negative. <br />

The boolean variables `max_reached` and `min_reached` indicate whether or not the minimum and maximum  angle values are cleared to ensure only full curls are counted.
These threshold values have been decided based on initial experiments with approximately 20 regular gym-goers and will continue to become more accurate with further testing.
