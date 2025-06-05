Game Description 








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
