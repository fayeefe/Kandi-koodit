The codebase for the ESP32 is in the file ArduinoCodes

Measurements are taken every half second and the final value is averaged from three measurements. Example measurement intervals.
1s, 1.5s, 2s -> average
2s, 2.5s, 3s -> average etc.

Diameter calculation is handled by the output function in a float variable called diameter which might need to be tuned at the start of every extrusion.
Do the tuning by changing the last minus parameter, which changes the offset. Usually this needs only -+ 0.2 changes.
Even after the tuning the diameter might not be a perfect 0.00 but 0.01, since the sensor can have some noise at the lowest point.
You can check that the output is correct with calipers.

Start the logging by pressing the button on the breadboard and you can stop it the same way.
The filament thickness sensor could get jammed around 1,47mm so make sure that is fine if the thickness drops there for some reason.

LMK if something goes wrong or you need help with getting the system running :)

The other files are unneccesary that I had for testing plotting dw about them