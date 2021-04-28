# BIOE-421-FinalProject
Garduino Addition - Using a load cell to estimate water loss

## **Materials:**

<ul>
    <li>Degraw 5kg Load Cell</li>
    <li>HX711 Load Cell Amplifier (combo <a href="https://www.amazon.com/Degraw-Load-Cell-HX711-Combo/dp/B075317R45">link</a> with load cell)</li>
    <li>Arduino Uno</li>
    <li>x4 Male Header Pins (included in load cell combo)</li>
    <li>x4 Male to Female Jumper Cables</li>
    <li>(optional) 3D-printer</li>
</ul>

## **Software Requirements:**

<ul>
	<li>Arduino IDE</li>
    <li>Python 3.8 (or any version that will work with the following packages):</li>
    <ul>
        <li>PySerial</li>
        <li>Matplotlib.pyplot</li>
    </ul>
    <li><a href="https://github.com/bogde/HX711">HX711 Library</a> for Arduino</li>
</ul>



## Helpful Resources:

* Instructables tutorial for wiring load cell and calibration (<a href="https://www.instructables.com/Arduino-Scale-With-5kg-Load-Cell-and-HX711-Amplifi/">link</a>)
  * Github with code used in tutorial (<a href="https://github.com/sparkfun/HX711-Load-Cell-Amplifier">link</a>)
* .stl files for load cell support (<a href = "https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbFhRLVJzbENpa0paR0xpeDVmOWdTNzhTdmZhd3xBQ3Jtc0ttajFQc3JFZTR3emMyN2lxb0RfWXdKZURzdVU3VHJiRDE0dElKN2FuNmdLc0h5Q081d0xFYjBBbGNWb19Sd1d4UEdOdWdHSnZMbzhXQWVOWFlFMUFmejRNZkl2ZENhZ0d3cnU1dGYtX1BVeUxweWZjOA&q=https%3A%2F%2Fwww.thingiverse.com%2Fthing%3A4602226">link</a>)
  * Video that uses the .stl files and uses another library to calibrate (<a href="https://www.youtube.com/watch?v=sxzoAGf1kOo">link</a>)
  * Github for code used in video (<a href="https://github.com/olkal/HX711_ADC">link</a>) (different from the code I used in my project!)
* Python live plotting from Arduino tutorial (<a href="https://highvoltages.co/tutorial/arduino-tutorial/arduino-real-time-plotting-with-python/">link</a>)
  * Github with sample code (<a href="https://github.com/HighVoltages/Python-arduino-realtime-graph">link</a>)



## How to Use:

### ***IMPORTANT NOTE:***  In order to stop the program, you can do either of the following:

* Unplug the Arduino (unsure if it will properly save output data)
* Press a keyboard exception into Python. For Windows, it is typically "Ctrl + C". You will have to wait until the next measurement is taken for the program to quit.

------

1. First, you will need to set up the load cell and amplifier circuit, as shown in the image below from this <a href="https://www.instructables.com/Arduino-Scale-With-5kg-Load-Cell-and-HX711-Amplifi/">instructables tutorial.</a> Personally, I soldered 4 header pins to the GND, DT, SCK, and VCC pins and used jumpers to connect to the Arduino. I soldered the load cell wires directly to the HX711 breakout board since I did not have small enough crimps to use header pins :stuck_out_tongue:

![Wire the Load Cells and HX711](https://content.instructables.com/ORIG/FH7/R58E/J7UR7KPT/FH7R58EJ7UR7KPT.jpg?auto=webp&frame=1&width=1024&fit=bounds&md=a578cb5e055ee72a8411256e2287530d)

2. Next, for best measurement results, I recommend using a casing/support to hold the load cell properly. For this specific load cell, I 3D-printed the <a href="https://www.thingiverse.com/thing:4602226">following support</a>, but you can make your own if you'd like! (I found the 3D printed parts in <a href="https://www.youtube.com/watch?v=sxzoAGf1kOo">this helpful video</a> detailing how to set up the load cell. It also links to sample code in case the code in this repository does not work for you)

3. Now that your load cell is fully connected and supported, it's time to calibrate! Download `LoadCellCalibration.ino`, upload the sketch to your board, and open the Serial Monitor. Do not place any weight on the load cell until readings start to display. Place a known weight (in pounds)  onto the load cell. It seems the load cell displays values well in pounds; if you would like to use SI units, to my knowledge you should do post-processing on the load cell reading. If the displayed weight is lower than the actual weight, type "+" or "a" as many consecutive times as you'd like into the serial monitor to change the calibration factor and increase the displayed weight value. Type "-" or "z" if the displayed weight is higher than the actual weight. For my load cell, a calibration factor of 190,850 worked well. **(Note: if your weight values are negative, change the sign of `calibration_factor`)**

4. Once the load cell has been calibrated, open `PythonPlotting.ino` and change the value of `calibration_factor`. This program will continuously take readings from the load cell at a specified time interval. In this file, `wait_time` is the number of milliseconds between sample measurements.

5. Now open `python_plotting.py`. Change `time_difference` to be the conversion from the Arduino's sampling time to whichever unit you would like your rate values to be. In my case, I set it to be `1/60`, since I am taking measurements once a minute and would like rates to be estimated in terms of hours. Before you run this code, install the PySerial package. Also, determine which serial/COM port will be used by your Arduino and insert is as the first input to `ser = serial.Serial('COM19', 9600)`. For Windows, it will look like `COMX`. For Mac/Linux, it may look like `/dev/ttyXXX`. The second input to the Serial function is the baud rate you will be using. Make sure this matches the baud rate used in your Arduino sketch!

6. You are now set up to begin using the device! Make sure the Arduino has `PythonPlotting.ino` uploaded to it, and that the Serial Monitor is closed. Then, run `python_plotting.py`, and you will see the first messages printed by the Arduino like you would in the Serial Monitor. Now, take as many measurements as you would like (see instructions for stopping the program, or implement a time-based way to stop it)! Once the file is done running, all recorded data will be saved to `weight_data.csv` in the same directory as `python_plotting.py`. This file saves every weight measurement and rate calculation with a timestamp. **NOTE:** Every time you run the program, `weight_data.csv` will be overwritten! Make sure to change its name or move it to another directory if you want to keep the data from that run.

### To stop the program you can:

* Unplug the Arduino (unsure if it will properly save data)
* Press a keyboard exception into Python. For Windows, it is typically "Ctrl + C". You will have to wait until the next measurement is taken for the program to quit.

