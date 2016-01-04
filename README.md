# INTRO

<br>
####Project Goal:
A fully autonomous quadcopter system that uses an Arduino as the "brain". 

<br>This is accomplished by using the quad's main control board (<a href="https://goo.gl/bwyB3Q">example</a>) for the PID balancing, and an Arduino to act as a remote on the quad by outputing PWM signals directly into the control board (where the RC receiver would usually connect). This enables the Arduino to be used purely for autonomous calculations, and not have to sacrifice processing power for PID balancing calculations. By having the Arduino output commands in the form of PWM signals into the control board's RC receiver ports, this enables for a fast and easy switch from manual to autonomous mode by swithing between the Arduino's input, and a RC reciver input via an electronic switch (more details on this system below).

<br>
####Components:
The quad's autonomous system will consist of the following:
><i>The quad is still in development, so some of these items are just ideas and may change. 
><br>Only includes parts for the autonomous system and not the quad its self.</i>


<ul>
    <li><a href="https://www.arduino.cc/en/Main/ArduinoBoardZero">Arduino Zero</a> (an Uno did not have the processing power needed)</li>
    <li><a href="https://goo.gl/u458dS">mpu6050 3 Axis analog gyro sensors & 3 Axis Accelerometer Module</a></li>
    <li>GPS (<a href="https://www.adafruit.com/products/746">example</a>)</li>
    <li>Some sort of electronic switch (i.e. relay or mosfets)</li>
</ul>

<br>
####Setting Up:
<br>
#####Libraries:
Download the "<i>MPU6050</i>" & "<i>I2Cdev</i>" libraries from <a href="https://github.com/jrowberg/i2cdevlib/tree/master/Arduino">here</a>, and install them as Arduino libraries in the Arduino IDE.
>Credit to <a href="https://github.com/jrowberg/i2cdevlib">Jeff Rowberg</a> for the mpu6050 libraries.

<br>
#####Using Visual Studio instead of the Arduino IDE:
<ul>
    <li>Download Visual Studio (<a href="https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx">Link</a>) and make sure C++ is installed durring installation.</li>
    <li>Then follow <a href="http://www.visualmicro.com/page/User-Guide.aspx?doc=First-steps.html">these</a> steps to get Visual Micro installed.</li>
</ul>

<br>
#####Wiring:
Schematic to come soon...

<br>

--------
><a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work by <span xmlns:cc="http://creativecommons.org/ns#" property="cc:attributionName">Demetrius Van Sickle</span> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
