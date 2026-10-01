# AnimationRecoder1.0

Stop-motion capture tool from the PIPS:lab years (2013), updated in 2026 for current openFrameworks. Built for two 1920x1080 screens side by side: the operator view on the left, the output on the right (press `f` to go fullscreen across both).

## Modes

- **1 Capture**: live camera with the last frame laid over it as an onion skin. Space captures a frame.
- **2 Check**: thumbnails of every frame. Hover to see a frame full size on the output screen, click to remove it.
- **3 Playback**: loops the frames on the output screen. `-` and `=` change the speed.

Other keys: backspace removes the last frame, `o` toggles the onion skin, `[` `]` change its opacity, arrow keys size and space the thumbnails, `v` switches camera, `h` shows the help.

Every captured frame is saved as a PNG in `bin/data/captures/<date and time>/`.

## Build

No addons needed. Generate the project with projectGenerator.
