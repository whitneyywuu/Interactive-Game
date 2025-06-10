Game Concept: The game begins when Button A is pressed, triggering a green NeoPixel flash and a beep. During gameplay, a random LED lights up in red, blue, or yellow. The player must respond correctly—press Button A for red, Button B for blue, or shake the board for yellow—within 1.5 seconds. Correct responses earn a point and play a happy sound, while incorrect or delayed actions trigger a buzz and no points. The game has three levels: Easy (1.5s delay), Medium (1.0s), and Hard (0.5s with sound distractions), advancing after every 10 correct responses. The game ends after 3 mistakes, playing a sad sound, and displaying the final score in the serial monitor along with how many mistakes you made. To win, get up to 50 points, and you'll hear a happy tune. Players can restart the game by pressing Button A.

🔌 Inputs

| Sensor          | Function                     | Raw Range   | Notes / Mapping                    |
|------------------|------------------------------|-------------|------------------------------------|
| Button A         | Press for red LED response   | Press/Not   | `CircuitPlayground.leftButton()`   |
| Button B         | Press for blue LED response  | Press/Not   | `CircuitPlayground.rightButton()`  |
| Accelerometer    | Shake for yellow LED response| > 1.5g      | Shake aggressively                 |

 🔊 Outputs

| Output     | Function                   | Raw Range      | Notes / Mapping          |
|------------|----------------------------|----------------|---------------------------|
| NeoPixels  | Display target color       | RGB (0–225)    | `setPixelColor()`         |
| Speaker    | Feedback (right or wrong)  | Sound tones    | `playTone()`              |
