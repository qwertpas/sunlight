## TO DO:

- [x] Add a slider to the web page
- [x] Read values from the slider on the monitor
- [x] Add a slider that can control the LED intensity
- [x] Add individual control sliders for the two LEDs
- [x] Make the sliders more continuous (update in real time as you are moving the slider)
- [x] Combine both the sliders into one (one end of the spectrum is pure orange and the other is pure blue)
- [x] Add a light intensity slider
- [] Make the color temp slider based on the time of the day
- [] Add the auto and manual toggle
- [] program the auto feauture
- [] Add the feature to change password and name for the network 


## Notes:

- Make the slider more continuous (the light should change value as you're sliding it, instead of only reading the last value)
- The slider value is local. If someone else if changing the slider value on the network, it won't update on my phone (needs to be bidirectional)
- We have chosen an arbitrary number ("544") to be the max value of the slider. It is hardcoded currently, but can be changed after. EDIT: It has now been changed
- Adding a logarithmic increase in intensity (instead of linear)
- In the backend, our max power output (60W) corresponds to a value of "30". So eveyrthing is scaled to it
- Added a cutoff number since the light was equivalent to "off" when the intensity was lower than 32% 
- The light is off when the intensity is less that 0.05 intensity for either. So, we added a line in the code that makes the intensities start at 0.05
- The light breaks (starts flickering like crazy) when we try to change things too fast sometimes (might be an exponent thing)
    - sometimes intensity and sometimes color
