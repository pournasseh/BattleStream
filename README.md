# BattleStream (Arduino Game)

A top-down scrolling shooter for Arduino Nano with a 128x64 I2C OLED (SSD1306).

This is an original hobby project and is not affiliated with or endorsed by any third-party game or brand. It’s part of a small collection of games for Arduino Nano (recommended board: “Arduino Nano Super Mini”).

## Requirements
- Arduino IDE or Arduino CLI
- Target board: Arduino Nano (ATmega328P)
- 128x64 SSD1306 OLED via I2C (default address 0x3C)
- Libraries:
  - Adafruit GFX Library
  - Adafruit SSD1306
  - Wire (bundled)
  - EEPROM (bundled on AVR)

Install libraries from Arduino IDE:
- Sketch → Include Library → Manage Libraries
- Search and install “Adafruit GFX Library” and “Adafruit SSD1306”

## Wiring (Nano)
- SSD1306 OLED (I2C):
  - SDA → A4
  - SCL → A5
  - VCC → 5V
  - GND → GND
- Buttons (using internal pull-ups):
  - `BTN_ENTER` on D3
  - `BTN_PREV` on D6 (move left)
  - `BTN_NEXT` on D7 (move right)

Note: If you use a different board, adjust I2C pins accordingly.

## Repo Layout
- `project.ino`: main game sketch
- `src/`: assets (e.g., images). `splash.jpg` is illustrative and not directly used by the sketch.

## Build & Upload
1) Open `project.ino` in Arduino IDE.
2) Tools:
   - Board: Arduino Nano
   - Processor: ATmega328P (Old Bootloader if your Nano clone requires it)
   - Port: select your COM port
3) Ensure required libraries are installed.
4) Click Upload.

Notes:
- I2C address is `0x3C`. If your display differs, change it in `display.begin(...)`.
- High score is stored in EEPROM. You can reset it from the in-game menu.

Tip (Arduino IDE folder prompt):
- Arduino IDE may prompt to rename the sketch folder to match `project.ino`. You can accept the prompt (folder name becomes `project`), or open the `.ino` directly and keep your folder if you prefer.

## Controls
- Hold left/right: move boat
- Press ENTER (in-game): shoot
- Long-press ENTER on the start screen: open menu
- Short-press ENTER on the start screen: enter standby/sleep; press ENTER again to wake
- Fuel drains over time; collect Fuel drops to refill
- HUD: top-left = score, top-right = distance in Km (X.YKm)

## Git Quickstart
From PowerShell in the project folder:

```bash
cd "E:\@ Valut\Arduino\Game - BattleStream\BattleStream"
git init
git add .
git commit -m "Initial commit: BattleStream Arduino Nano game"
git branch -M main
git remote add origin https://github.com/USERNAME/REPO_NAME.git
git push -u origin main
```

If needed, configure Git identity:
```bash
git config --global user.name "Your Name"
git config --global user.email "you@example.com"
```

## License
MIT — see `LICENSE`.


