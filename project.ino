#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define BTN_ENTER 3   // D3 (INT1)
#define BTN_NEXT  7   // RIGHT
#define BTN_PREV  6   // LEFT

enum Page : uint8_t { PAGE_START=0, PAGE_GAME=1, PAGE_MENU=2, PAGE_ABOUT=3, PAGE_GAMEOVER=4 };
static Page page = PAGE_START;


const uint16_t RIVER_BG_WIDTH  = 128;
const uint16_t RIVER_BG_HEIGHT = 64;
const uint8_t PROGMEM RIVER_BG_BITMAP[1024] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xC0, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1C, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x34, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x6C, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x9C, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF8, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x02, 0x49, 0x80, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x06, 0x49, 0x92, 0xB7, 0x62, 0x23, 0x75, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x03, 0x49, 0x2A, 0xA5, 0x61, 0x17, 0x50, 0x80, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x05, 0x29, 0x13, 0xA5, 0x77, 0x73, 0x53, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x40, 0x31, 0x02, 0x02, 0x90, 0x80, 0x20, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x07, 0xE0, 0x71, 0xFE, 0xFE, 0xF0, 0xFE, 0x66, 0x38, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x07, 0xF0, 0x79, 0xFE, 0xFE, 0xF0, 0xFE, 0x07, 0xFD, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x07, 0xB0, 0xF8, 0x38, 0x78, 0xF0, 0xE0, 0x41, 0xFE, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xF0, 
  0x00, 0x07, 0xF0, 0xF8, 0x38, 0x78, 0xF0, 0xFE, 0x43, 0xDF, 0xE8, 0x00, 0x00, 0x00, 0x1B, 0xF8, 
  0x00, 0x07, 0xF8, 0xDC, 0x38, 0x78, 0xF0, 0xFE, 0x43, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x4F, 0xFC, 
  0x00, 0x07, 0xB9, 0xDC, 0x38, 0x78, 0xF0, 0xFE, 0x01, 0xFF, 0xF2, 0x03, 0x80, 0x02, 0x3F, 0xFC, 
  0x00, 0x07, 0xB9, 0xFE, 0x38, 0x78, 0xF0, 0xE0, 0x43, 0xFF, 0xFC, 0x87, 0xC0, 0x6F, 0xFF, 0xF8, 
  0x00, 0x07, 0xFB, 0xFE, 0x38, 0x78, 0xFE, 0xFE, 0x43, 0xFE, 0xBE, 0xC7, 0xC1, 0x3F, 0xFF, 0xF0, 
  0x00, 0x07, 0xF3, 0x87, 0x38, 0x78, 0xFE, 0xFE, 0x00, 0xFF, 0xFF, 0x87, 0xC9, 0xFF, 0xFF, 0xC0, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xA7, 0xE7, 0xFF, 0xFF, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xEF, 0xFF, 0xCC, 0x7F, 0xFF, 0xF8, 0x00, 
  0x00, 0x07, 0xCF, 0xDF, 0x1F, 0xC1, 0x06, 0x06, 0x00, 0x3F, 0xFF, 0xE7, 0xFF, 0xFF, 0xE0, 0x00, 
  0x00, 0x07, 0xEF, 0xDF, 0xDF, 0xC3, 0x06, 0x06, 0x00, 0x1F, 0xFF, 0x9F, 0x7F, 0xFF, 0x80, 0x00, 
  0x00, 0x06, 0x07, 0x1D, 0xDE, 0x03, 0x87, 0x0E, 0x00, 0x07, 0xFE, 0xFF, 0x7F, 0xF8, 0x00, 0x00, 
  0x00, 0x07, 0xC7, 0x1F, 0xDF, 0x87, 0x87, 0x9E, 0x00, 0x0B, 0xFF, 0xFF, 0x7F, 0xE0, 0x00, 0x00, 
  0x00, 0x07, 0xE7, 0x1F, 0xDF, 0x87, 0xC7, 0xFE, 0x78, 0x7D, 0xFD, 0xF9, 0xFF, 0x00, 0x00, 0x00, 
  0x00, 0x01, 0xE7, 0x1F, 0x1E, 0x0E, 0xC7, 0xFE, 0x0F, 0xFF, 0xFA, 0xFB, 0xFC, 0x1F, 0xC0, 0x00, 
  0x00, 0x07, 0xE7, 0x1F, 0xDF, 0xCF, 0xE7, 0xFE, 0xFF, 0xFF, 0xFF, 0x5F, 0x37, 0xFE, 0x00, 0x00, 
  0x00, 0x07, 0xE7, 0x1D, 0xDF, 0xDF, 0xE7, 0x6E, 0xFB, 0xFB, 0xDF, 0xEF, 0xFF, 0xF0, 0x00, 0x00, 
  0x00, 0x07, 0x87, 0x1D, 0xFF, 0xDC, 0x77, 0x0E, 0xFF, 0xFF, 0xEF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x60, 0xFF, 0x6F, 0xFB, 0xFB, 0xFE, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xDF, 0xFE, 0x0F, 0xBF, 0xF0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xBF, 0x00, 0x1F, 0xF7, 0xF0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xBC, 0x00, 0x1C, 0x03, 0xF0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 
  0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 
  0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 
  0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 
  0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF8, 
  0x1F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF8, 
  0x7F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF8, 
  0x7F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF8, 
  0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
  0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xE6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xE3, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xE6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE7, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE7, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 
  0xFF, 0xFF, 0xFF, 0xF3, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0xC7, 0xFF, 0xFF, 0xFF
};


// --- Blink state ---
static uint8_t blinkOn = 1;
static unsigned long blinkMs = 0;

// --- Highscore EEPROM ---
static const uint16_t EE_SIG = 0x51C3;
static const int EE_ADDR_SIG_LO = 0;
static const int EE_ADDR_SIG_HI = 1;
static const int EE_ADDR_HS     = 2;
static uint32_t gHighScore = 0;

// --- ENTER press tracking ---
static uint8_t enterPrev = 0;
static unsigned long enterDownAt = 0;
static uint8_t enterLongFired = 0;
static const unsigned long ENTER_LONG_MS = 650;
static const unsigned long ENTER_DEBOUNCE_MS = 30;

// NEXT/PREV edges
static uint8_t nextPrevPressedPrev = 0;
static uint8_t nextNextPressedPrev = 0;

// Input gate to block carry-over holds from Start → Game
static uint8_t gInputGate = 0;

// Menu
static int8_t menuSel = 0;
const char MI0[] PROGMEM = "About";
const char MI1[] PROGMEM = "Reset Highscore";
const char MI2[] PROGMEM = "Back";
const char* const MENU_ITEMS[] PROGMEM = { MI0, MI1, MI2 };

// ---------------- Scrolling river shooter game ----------------
static uint8_t gLeft[64];
static uint8_t gRight[64];

// UI guards
static const uint8_t LEFT_UI_GUARD  = 20;
static const uint8_t RIGHT_GUARD    = 0;
static const uint8_t BANK_MARGIN    = 2;

// Mirror geometry
static const uint8_t BASE_ANCHOR = 21;
static const uint8_t W_MIN       = 34;

static uint8_t AVAIL = SCREEN_WIDTH - LEFT_UI_GUARD - RIGHT_GUARD; // 108
static uint8_t HALF  = 108/2;                                       // 54
static int16_t X_A;
static int16_t X_M;

// --- Spawn timeline anchor ---
static unsigned long gSpawnStartMs = 0;

// Offset/slope state
static int8_t  offset = 0;
static int8_t  sOffset = 0;
static uint8_t segOffset = 0;
static uint8_t rightIsMirror = 1;

// Precomputed limits
static int8_t R1=0, R2=0, R3=0;

// Initial straight phase (2s) + enforcement to hit zero at least every 7s
static unsigned long straightUntilMs = 0;
static const unsigned long STRAIGHT_MS = 2000UL;
static unsigned long lastZeroMs = 0;
static const unsigned long ZERO_ENFORCE_MS = 7000UL;
static uint8_t forcedFirstBreakDone = 0;

// Player
static const uint8_t PLAYER_Y = 56;
static const uint8_t PLAYER_W = 12;
static const uint8_t PLAYER_H = 6;
static int16_t playerX = SCREEN_WIDTH/2;

// Lives + invulnerability (blink)
static uint8_t livesRemaining = 4;
static unsigned long invUntilMs = 0;
static const unsigned long INV_MS = 3000UL;
// Freezes
static const unsigned long RESP_FREEZE_MS = 1500UL; // 1.5s
static uint8_t respawnFreezing = 0;
static unsigned long respawnFreezeUntilMs = 0;
// Final freeze before Game Over
static uint8_t finalFreezing = 0;
static unsigned long finalFreezeUntilMs = 0;

// --- Packed entities ---
struct Bullet { uint8_t x, y, active; };
static const uint8_t MAX_BULLETS = 12;
static Bullet bullets[MAX_BULLETS];
static uint8_t bulletCursor = 0;

struct EBullet { uint8_t x, y; int8_t vy; uint8_t active; };
static const uint8_t MAX_EBULLETS = 5;
static EBullet ebullets[MAX_EBULLETS];

struct Explosion { uint8_t x, y, t; };
static const uint8_t MAX_EXPL = 4;
static Explosion expl[MAX_EXPL];

struct Helicopter { uint8_t x, y; int8_t vx; uint8_t phase; uint8_t active; };
static Helicopter helis[3];

static const uint8_t BOAT_W = 6;
static const uint8_t BOAT_H = 9;
struct Boat { uint8_t x, y; uint8_t active; uint8_t fired; };
static const uint8_t MAX_BOATS = 2;
static Boat boats[MAX_BOATS];
static const uint8_t BOAT_FIRE_WINDOW = 42;

static const uint8_t ROCKET_W = 5;
static const uint8_t ROCKET_H = 7;
struct Rocket { uint8_t x, y; int8_t vy; uint8_t active; };
static const uint8_t MAX_ROCKETS = 3;
static Rocket rockets[MAX_ROCKETS];

enum DropType : uint8_t { DROP_NONE=0, DROP_COIN=1, DROP_FUEL=2 };
struct Drop { uint8_t x, y; int8_t vy; uint8_t t; uint8_t frame; uint8_t type; uint8_t active; };
static const uint8_t MAX_DROPS = 6;
static Drop drops[MAX_DROPS];

// Fuel (30s drain)
static const uint32_t FUEL_MAX_MS = 30000UL;
static uint32_t fuelMs = FUEL_MAX_MS;
static const uint8_t FUEL_X = 0;
static const uint8_t FUEL_W = 6;
static const uint8_t FUEL_Y0 = 10;
static const uint8_t FUEL_H  = 53;

// Score
static uint16_t gScore = 0;

// Timing
static unsigned long lastFrameMs = 0;
static const uint8_t FRAME_MS = 30;

// Game Over: new-record flag
static uint8_t gNewHighScore = 0;

// --- Distance (Km) ---
// Store as tenths of Km to avoid float. Increases by +1 every 1000ms of active play.
static uint32_t gDistTenths = 0;
static unsigned long distAccumMs = 0;
static unsigned long distLastMs = 0;

// Helpers
static inline uint8_t u8max(uint8_t a, uint8_t b){ return (a>b)?a:b; }
static inline uint8_t segLen(){ return (uint8_t)(33 + random(68)); }
static inline int8_t  pickSlopeDir(){ return (random(2)==0)? -1 : +1; }

// Text helpers
static inline void drawCenteredText(const __FlashStringHelper* s, int16_t y){
  PGM_P p = reinterpret_cast<PGM_P>(s);
  int16_t x = (SCREEN_WIDTH - (int)strlen_P(p) * 6) / 2;
  if (x < 0) x = 0;
  display.setCursor(x, y);
  display.print(s);
}
static inline void drawCenteredTextSize(const __FlashStringHelper* s, int16_t y, uint8_t sz){
  PGM_P p = reinterpret_cast<PGM_P>(s);
  int w = (int)strlen_P(p) * 6 * (int)sz;
  int16_t x = (SCREEN_WIDTH - w) / 2;
  if (x < 0) x = 0;
  display.setTextSize(sz);
  display.setCursor(x, y);
  display.print(s);
  display.setTextSize(1);
}

// EEPROM
static inline void saveSignature(){ EEPROM.update(EE_ADDR_SIG_LO, (uint8_t)(EE_SIG & 0xFF)); EEPROM.update(EE_ADDR_SIG_HI, (uint8_t)(EE_SIG >> 8)); }
static inline bool signatureOk(){ uint16_t s = (uint16_t)EEPROM.read(EE_ADDR_SIG_LO) | ((uint16_t)EEPROM.read(EE_ADDR_SIG_HI) << 8); return s == EE_SIG; }
static inline uint32_t eeReadU32(int a){ return (uint32_t)EEPROM.read(a+0) | ((uint32_t)EEPROM.read(a+1)<<8) | ((uint32_t)EEPROM.read(a+2)<<16) | ((uint32_t)EEPROM.read(a+3)<<24); }
static inline void eeWriteU32(int a, uint32_t v){ EEPROM.update(a+0,(uint8_t)(v>>0)); EEPROM.update(a+1,(uint8_t)(v>>8)); EEPROM.update(a+2,(uint8_t)(v>>16)); EEPROM.update(a+3,(uint8_t)(v>>24)); }
static inline void loadHighScore(){ if (!signatureOk()){ gHighScore=0; saveSignature(); eeWriteU32(EE_ADDR_HS,gHighScore);} else { gHighScore=eeReadU32(EE_ADDR_HS);} }
static inline void resetHighScore(){ gHighScore=0; saveSignature(); eeWriteU32(EE_ADDR_HS,gHighScore); }
static inline void commitHighScore(uint32_t s){ if (s>gHighScore){ gHighScore=s; eeWriteU32(EE_ADDR_HS,gHighScore);} }

// Standby
void wakeISR(){}
static void enterStandby(){
  display.clearDisplay();
  drawCenteredTextSize(F("Bye"), 28, 2);
  display.display();
  delay(1000);
  display.ssd1306_command(SSD1306_DISPLAYOFF);
  attachInterrupt(digitalPinToInterrupt(BTN_ENTER), wakeISR, LOW);
  ADCSRA &= ~(1 << ADEN);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable(); noInterrupts(); EIFR = bit(INTF1); interrupts(); sleep_cpu();
  sleep_disable(); detachInterrupt(digitalPinToInterrupt(BTN_ENTER)); ADCSRA |= (1 << ADEN);
  while (digitalRead(BTN_ENTER) == LOW) { delay(5); }
  display.ssd1306_command(SSD1306_DISPLAYON);
  delay(10);
  display.clearDisplay(); display.setTextSize(1); display.setTextColor(SSD1306_WHITE); display.display();
  page = PAGE_START;
}

// -------- Mirror/parallel river generation with constraints --------
static void computeMirrorParams(){
  AVAIL = SCREEN_WIDTH - LEFT_UI_GUARD - RIGHT_GUARD;
  HALF  = AVAIL / 2;
  X_A   = LEFT_UI_GUARD + BASE_ANCHOR;
  X_M   = LEFT_UI_GUARD + HALF + BASE_ANCHOR;

  int16_t leftMin  = LEFT_UI_GUARD + BANK_MARGIN;
  int16_t rightMax = (SCREEN_WIDTH - 1 - RIGHT_GUARD - BANK_MARGIN);
  int16_t r1 = X_A - leftMin;
  int16_t r2 = rightMax - X_M;
  int16_t r3 = ( (X_M - X_A) - (int16_t)W_MIN ) / 2;
  if (r1 < 1) r1 = 1; if (r1 > 31) r1 = 31;
  if (r2 < 1) r2 = 1; if (r2 > 31) r2 = 31;
  if (r3 < 1) r3 = 1; if (r3 > 31) r3 = 31;
  R1 = (int8_t)r1; R2 = (int8_t)r2; R3 = (int8_t)r3;
}

static inline void beginFlat(){ sOffset = 0; segOffset = segLen(); }
static inline void beginSlopeFromHere(){
  sOffset = pickSlopeDir(); segOffset = 255;
  if (offset == 0){ rightIsMirror = (random(2)==0); }
}
static inline void maybeStartSlopeAfterFlat(){ if (sOffset==0 && segOffset==0){ beginSlopeFromHere(); } }

static inline void advanceOffset(){
  maybeStartSlopeAfterFlat();
  if (sOffset != 0){
    int8_t posMax, negMax;
    if (rightIsMirror){ posMax = R3; negMax = (R1 < R2) ? R1 : R2; }
    else { posMax = R2; negMax = R1; }
    int16_t next = (int16_t)offset + (int16_t)sOffset;
    if ((offset > 0 && sOffset < 0 && next <= 0) || (offset < 0 && sOffset > 0 && next >= 0)){
      offset = 0; beginFlat(); lastZeroMs = millis(); return;
    }
    if (sOffset > 0 && next > posMax){ offset = posMax; beginFlat(); return; }
    if (sOffset < 0 && next < -negMax){ offset = -negMax; beginFlat(); return; }
    offset = (int8_t)next;
  } else {
    if (segOffset > 0) segOffset--;
  }
}

static void riverInit(){
  computeMirrorParams();
  offset = 0; sOffset = 0; segOffset = 1; rightIsMirror = 1; forcedFirstBreakDone = 0;
  for (uint8_t y=0; y<64; ++y){ gLeft[y] = (uint8_t)X_A; gRight[y] = (uint8_t)X_M; }
  lastZeroMs = millis();
}

static void riverStep(){
  unsigned long now = millis();
  for (int y=63; y>0; --y){ gLeft[y] = gLeft[y-1]; gRight[y] = gRight[y-1]; }
  if ((long)(straightUntilMs - now) > 0){
    gLeft[0]  = (uint8_t)X_A; gRight[0] = (uint8_t)X_M; lastZeroMs = now; return;
  }
  if ((long)(now - lastZeroMs) >= (long)ZERO_ENFORCE_MS && offset != 0){
    if (sOffset == 0){
      sOffset = (offset > 0) ? -1 : +1;
      segOffset = 255;
      rightIsMirror = 1;
    } else {
      if ((offset > 0 && sOffset > 0) || (offset < 0 && sOffset < 0)) sOffset = -sOffset;
    }
  }
  if (!forcedFirstBreakDone && offset == 0 && sOffset==0){ beginSlopeFromHere(); forcedFirstBreakDone = 1; }
  advanceOffset();
  int16_t xl = X_A + offset;
  int16_t xr = rightIsMirror ? (X_M - offset) : (X_M + offset);

  int16_t pl = gLeft[1];
  int16_t pr = gRight[1];
  if (xl > pl + 1) xl = pl + 1;
  if (xl < pl - 1) xl = pl - 1;
  if (xr > pr + 1) xr = pr + 1;
  if (xr < pr - 1) xr = pr - 1;

  gLeft[0]  = (uint8_t)xl;
  gRight[0] = (uint8_t)xr;

  if (offset == 0) lastZeroMs = now;
}

static void drawRiver(){
  display.drawPixel(gLeft[0],  0, SSD1306_WHITE);
  display.drawPixel(gRight[0], 0, SSD1306_WHITE);
  for (uint8_t y=1; y<64; ++y){
    display.drawLine(gLeft[y-1],  y-1, gLeft[y],  y, SSD1306_WHITE);
    display.drawLine(gRight[y-1], y-1, gRight[y], y, SSD1306_WHITE);
  }
}

// --- Helicopters ---
static void helisInit(){ for (uint8_t i=0;i<3;i++){ helis[i].active=0; helis[i].phase=0; } }
static void helisStep(){
  for (uint8_t i=0;i<3;i++){
    if (!helis[i].active) continue;
    helis[i].y += 1;
    if (helis[i].y > 63){ helis[i].active=0; continue; }
    uint8_t l = gLeft[helis[i].y], r = gRight[helis[i].y];
    int16_t minX = l + 2, maxX = r - 2 - 8;
    if (helis[i].x <= minX) helis[i].vx = 1;
    if (helis[i].x >= maxX) helis[i].vx = -1;
    helis[i].x = (uint8_t)((int16_t)helis[i].x + (int16_t)helis[i].vx);
    helis[i].phase ^= 1;
  }
}
static void drawHeliLeft(const Helicopter& h){
  uint8_t ry = (h.y > 0) ? (h.y - 1) : 0;
  if (h.y > 0){
    if ((h.phase & 1) == 0){ for (uint8_t i=0;i<7;i++) display.drawPixel(h.x + i, ry, SSD1306_WHITE); }
    else { for (uint8_t i=0;i<3;i++) display.drawPixel(h.x + 2 + i, ry, SSD1306_WHITE); }
  }
  display.drawPixel(h.x + 3, h.y + 0, SSD1306_WHITE);
  display.drawPixel(h.x + 2, h.y + 1, SSD1306_WHITE);
  display.drawPixel(h.x + 3, h.y + 1, SSD1306_WHITE);
  display.drawPixel(h.x + 4, h.y + 1, SSD1306_WHITE);
  display.drawPixel(h.x + 7, h.y + 1, SSD1306_WHITE);
  for (uint8_t i=0;i<7;i++) display.drawPixel(h.x + 1 + i, h.y + 2, SSD1306_WHITE);
}
static void drawHeliRight(const Helicopter& h){
  uint8_t ry = (h.y > 0) ? (h.y - 1) : 0;
  if (h.y > 0){
    if ((h.phase & 1) == 0){ for (uint8_t i=1;i<=7;i++) display.drawPixel(h.x + i, ry, SSD1306_WHITE); }
    else { for (uint8_t i=3;i<=5;i++) display.drawPixel(h.x + i, ry, SSD1306_WHITE); }
  }
  display.drawPixel(h.x + 4, h.y + 0, SSD1306_WHITE);
  display.drawPixel(h.x + 0, h.y + 1, SSD1306_WHITE);
  display.drawPixel(h.x + 3, h.y + 1, SSD1306_WHITE);
  display.drawPixel(h.x + 4, h.y + 1, SSD1306_WHITE);
  display.drawPixel(h.x + 5, h.y + 1, SSD1306_WHITE);
  for (uint8_t i=0;i<7;i++) display.drawPixel(h.x + i, h.y + 2, SSD1306_WHITE);
}
static void drawHelis(){
  for (uint8_t i=0;i<3;i++){
    if (!helis[i].active) continue;
    if (helis[i].vx > 0) drawHeliRight(helis[i]); else drawHeliLeft(helis[i]);
  }
}
static void helisAnimTick(){
  for (uint8_t i=0;i<3;i++){
    if (!helis[i].active) continue;
    helis[i].phase ^= 1;
  }
}

// --- Player Bullets ---
static void bulletsInit(){ for (uint8_t i=0;i<MAX_BULLETS;i++){ bullets[i].active=0; } bulletCursor=0; }
static void bulletSpawn(){
  uint8_t idx = bulletCursor;
  for (uint8_t k=0;k<MAX_BULLETS;k++){
    if (!bullets[idx].active) break;
    idx = (uint8_t)((idx + 1) % MAX_BULLETS);
  }
  bulletCursor = (uint8_t)((idx + 1) % MAX_BULLETS);
  bullets[idx].active = 1;
  bullets[idx].x = (uint8_t)playerX;

  int16_t by = (int16_t)getPlayerY() - 2;
  if (by >= SCREEN_HEIGHT){ bullets[idx].active = 0; return; } // Not visible yet; skip firing
  bullets[idx].y = (uint8_t)by;
}
static void bulletsStep(uint8_t shootEdge){
  if (shootEdge){ bulletSpawn(); }
  for (uint8_t i=0;i<MAX_BULLETS;i++){
    if (!bullets[i].active) continue;
    if (bullets[i].y < 3){ bullets[i].active=0; continue; }
    bullets[i].y -= 3;
    uint8_t y = bullets[i].y;
    uint8_t l = gLeft[y];
    uint8_t r = gRight[y];
    if (bullets[i].x <= l || bullets[i].x >= r){ bullets[i].active=0; continue; }
  }
}
static void drawBullets(){
  for (uint8_t i=0;i<MAX_BULLETS;i++){
    if (!bullets[i].active) continue;
    display.drawFastVLine(bullets[i].x, bullets[i].y-2, 4, SSD1306_WHITE);
  }
}

// --- Enemy Bullets ---
static void ebulletsInit(){ for (uint8_t i=0;i<MAX_EBULLETS;i++){ ebullets[i].active=0; } }
static void ebulletSpawn(uint8_t x, uint8_t y, int8_t vy){
  for (uint8_t i=0;i<MAX_EBULLETS;i++){
    if (!ebullets[i].active){
      ebullets[i].active=1; ebullets[i].x=x; ebullets[i].y=y; ebullets[i].vy=vy; return;
    }
  }
}
static void ebulletsStep(){
  for (uint8_t i=0;i<MAX_EBULLETS;i++){
    if (!ebullets[i].active) continue;
    int16_t ny = (int16_t)ebullets[i].y + (int16_t)ebullets[i].vy;
    if (ny >= SCREEN_HEIGHT){ ebullets[i].active=0; continue; }
    if (ny < 0){ ebullets[i].active=0; continue; }
    ebullets[i].y = (uint8_t)ny;
    uint8_t y = ebullets[i].y;
    uint8_t l = gLeft[y];
    uint8_t r = gRight[y];
    if (ebullets[i].x <= l || ebullets[i].x >= r){ ebullets[i].active=0; continue; }
  }
}
static void drawEnemyBullets(){
  for (uint8_t i=0;i<MAX_EBULLETS;i++){
    if (!ebullets[i].active) continue;
    display.drawFastVLine(ebullets[i].x, ebullets[i].y, 3, SSD1306_WHITE);
  }
}

// --- Explosions ---
static void explInit(){ for (uint8_t i=0;i<MAX_EXPL;i++){ expl[i].t=0; } }
static void explSpawn(uint8_t x, uint8_t y){
  for (uint8_t i=0;i<MAX_EXPL;i++){
    if (expl[i].t==0){ expl[i].x=x; expl[i].y=y; expl[i].t=1; return; }
  }
  expl[0].x=x; expl[0].y=y; expl[0].t=1;
}
static void explStep(){
  for (uint8_t i=0;i<MAX_EXPL;i++){
    if (expl[i].t==0) continue;
    expl[i].t++;
    if (expl[i].t > 5){ expl[i].t=0; }
  }
}
static void drawExpl(){
  for (uint8_t i=0;i<MAX_EXPL;i++){
    if (expl[i].t==0) continue;
    uint8_t r = (expl[i].t<=4)? expl[i].t : 4;
    uint8_t x = expl[i].x, y = expl[i].y;
    display.drawPixel(x, y, SSD1306_WHITE);
    display.drawFastHLine(x - r, y, 2*r+1, SSD1306_WHITE);
    display.drawFastVLine(x, y - r, 2*r+1, SSD1306_WHITE);
    for (int8_t d=-((int8_t)r); d<=((int8_t)r); d++){
      display.drawPixel(x + d, y + d, SSD1306_WHITE);
      display.drawPixel(x + d, y - d, SSD1306_WHITE);
    }
  }
}

// ---- Collisions & helpers ----
static inline uint8_t rectHit(int16_t ax,int16_t ay,uint8_t aw,uint8_t ah,int16_t bx,int16_t by,uint8_t bw,uint8_t bh){
  return !(bx > ax+aw-1 || bx+bw-1 < ax || by > ay+ah-1 || by+bh-1 < ay);
}
static inline uint8_t shoreTouch(){
  uint8_t l = gLeft[PLAYER_Y], r = gRight[PLAYER_Y];
  int16_t px0 = playerX - 5, px1 = playerX + 6;
  return (px0 <= l || px1 >= r);
}
static void bulletHitsHelis(){
  for (uint8_t b=0;b<MAX_BULLETS;b++){
    if (!bullets[b].active) continue;
    for (uint8_t i=0;i<3;i++){
      if (!helis[i].active) continue;
      if ( !( (helis[i].x   > bullets[b].x+1) || (helis[i].x+7 < bullets[b].x-1) ||
              ((int16_t)helis[i].y-1 > (int16_t)bullets[b].y+2) || ((int16_t)helis[i].y+2 < (int16_t)bullets[b].y-2) ) ){
        helis[i].active=0;
        bullets[b].active=0;
        gScore += 10;
        explSpawn(helis[i].x + 4, helis[i].y + 1);
        dropSpawnMaybeAtCenter(helis[i].x + 4, helis[i].y + 1);
        break;
      }
    }
  }
}
static int8_t heliHitIndex(){
  int16_t px0 = playerX - 5;
  int16_t py0 = PLAYER_Y - (PLAYER_H - 1);
  for (uint8_t i=0;i<3;i++){
    if (!helis[i].active) continue;
    if (rectHit(px0, py0, PLAYER_W, PLAYER_H, helis[i].x, (int16_t)helis[i].y-1, 8,4)){
      return (int8_t)i;
    }
  }
  return -1;
}
static uint8_t enemyBulletHitsPlayer(){
  int16_t px0 = playerX - 5;
  int16_t py0 = PLAYER_Y - (PLAYER_H - 1);
  for (uint8_t i=0;i<MAX_EBULLETS;i++){
    if (!ebullets[i].active) continue;
    if (rectHit(px0, py0, PLAYER_W, PLAYER_H, ebullets[i].x, ebullets[i].y, 1,3)){
      ebullets[i].active=0;
      return 1;
    }
  }
  return 0;
}

// ----- Boat logic -----
static const uint8_t BOAT_ROWS[BOAT_H] PROGMEM = {
  0x1E, 0x3F, 0x21, 0x2D, 0x2D, 0x21, 0x21, 0x12, 0x0C
};
static void boatsInit(){ for(uint8_t i=0;i<MAX_BOATS;i++){ boats[i].active=0; boats[i].fired=0; } }
static void boatsStep(){
  for (uint8_t i=0;i<MAX_BOATS;i++){
    if (!boats[i].active) continue;
    boats[i].y += 1;
    if (boats[i].y >= SCREEN_HEIGHT){ boats[i].active=0; continue; }
    if (!boats[i].fired && boats[i].y <= (uint8_t)(BOAT_FIRE_WINDOW - BOAT_H)){
      if (random(20) == 0){
        uint8_t bx = boats[i].x + (BOAT_W/2);
        uint8_t by = boats[i].y + BOAT_H;
        ebulletSpawn(bx, by, +2);
        boats[i].fired = 1;
      }
    }
  }
}
static void drawBoatAt(uint8_t x, uint8_t y){
  for (uint8_t r=0;r<BOAT_H;r++){
    uint8_t yy = y + r;
    if (yy >= SCREEN_HEIGHT) break;
    uint8_t m = pgm_read_byte(&BOAT_ROWS[r]);
    for (uint8_t c=0;c<BOAT_W;c++){
      if (m & (1 << (BOAT_W-1 - c))){
        uint8_t xx = x + c;
        if (xx < SCREEN_WIDTH) display.drawPixel(xx, yy, SSD1306_WHITE);
      }
    }
  }
}
static void drawBoats(){
  for (uint8_t i=0;i<MAX_BOATS;i++){
    if (!boats[i].active) continue;
    drawBoatAt(boats[i].x, boats[i].y);
  }
}
static void bulletHitsBoats(){
  for (uint8_t b=0;b<MAX_BULLETS;b++){
    if (!bullets[b].active) continue;
    for (uint8_t i=0;i<MAX_BOATS;i++){
      if (!boats[i].active) continue;
      if (rectHit(boats[i].x, boats[i].y, BOAT_W, BOAT_H, (int16_t)bullets[b].x-1, (int16_t)bullets[b].y-2, 3,4)){
        boats[i].active=0;
        bullets[b].active=0;
        gScore += 15;
        explSpawn(boats[i].x + BOAT_W/2, boats[i].y + BOAT_H/2);
        dropSpawnMaybeAtCenter(boats[i].x + BOAT_W/2, boats[i].y + BOAT_H/2);
        break;
      }
    }
  }
}
static uint8_t playerHitsBoat(){
  int16_t px0 = playerX - 5;
  int16_t py0 = PLAYER_Y - (PLAYER_H - 1);
  for (uint8_t i=0;i<MAX_BOATS;i++){
    if (!boats[i].active) continue;
    if (rectHit(px0, py0, PLAYER_W, PLAYER_H, boats[i].x, boats[i].y, BOAT_W, BOAT_H)){
      explSpawn(boats[i].x + BOAT_W/2, boats[i].y + BOAT_H/2);
      boats[i].active=0;
      return 1;
    }
  }
  return 0;
}

// ----- Rocket logic -----
static const uint8_t ROCKET_ROWS[ROCKET_H] PROGMEM = {
  0x15, 0x1F, 0x0A, 0x0A, 0x0A, 0x0E, 0x04
};
static void rocketsInit(){ for(uint8_t i=0;i<MAX_ROCKETS;i++){ rockets[i].active=0; rockets[i].vy=2; } }
static void rocketsStep(){
  for (uint8_t i=0;i<MAX_ROCKETS;i++){
    if (!rockets[i].active) continue;
    int16_t ny = (int16_t)rockets[i].y + (int16_t)rockets[i].vy;
    if (ny >= SCREEN_HEIGHT){ rockets[i].active=0; continue; }
    rockets[i].y = (uint8_t)ny;
  }
}
static void drawRocketAt(uint8_t x, uint8_t y){
  for (uint8_t r=0;r<ROCKET_H;r++){
    uint8_t yy = y + r;
    if (yy >= SCREEN_HEIGHT) break;
    uint8_t m = pgm_read_byte(&ROCKET_ROWS[r]);
    for (uint8_t c=0;c<ROCKET_W;c++){
      if (m & (1 << (ROCKET_W-1 - c))){
        uint8_t xx = x + c;
        if (xx < SCREEN_WIDTH) display.drawPixel(xx, yy, SSD1306_WHITE);
      }
    }
  }
}
static void drawRockets(){
  for (uint8_t i=0;i<MAX_ROCKETS;i++){
    if (!rockets[i].active) continue;
    drawRocketAt(rockets[i].x, rockets[i].y);
  }
}
static void bulletHitsRockets(){
  for (uint8_t b=0;b<MAX_BULLETS;b++){
    if (!bullets[b].active) continue;
    for (uint8_t i=0;i<MAX_ROCKETS;i++){
      if (!rockets[i].active) continue;
      if (rectHit(rockets[i].x, rockets[i].y, ROCKET_W, ROCKET_H, (int16_t)bullets[b].x-1, (int16_t)bullets[b].y-2, 3,4)){
        rockets[i].active=0;
        bullets[b].active=0;
        gScore += 12;
        explSpawn(rockets[i].x + ROCKET_W/2, rockets[i].y + ROCKET_H/2);
        dropSpawnMaybeAtCenter(rockets[i].x + ROCKET_W/2, rockets[i].y + ROCKET_H/2);
        break;
      }
    }
  }
}
static uint8_t playerHitsRocket(){
  int16_t px0 = playerX - 5;
  int16_t py0 = PLAYER_Y - (PLAYER_H - 1);
  for (uint8_t i=0;i<MAX_ROCKETS;i++){
    if (!rockets[i].active) continue;
    if (rectHit(px0, py0, PLAYER_W, PLAYER_H, rockets[i].x, rockets[i].y, ROCKET_W, ROCKET_H)){
      explSpawn(rockets[i].x + ROCKET_W/2, rockets[i].y + ROCKET_H/2);
      rockets[i].active=0;
      return 1;
    }
  }
  return 0;
}

// ----- Drops (coin / fuel) -----
static const uint8_t BM_COIN0[8] PROGMEM = { 0x18, 0x66, 0x42, 0x81, 0x81, 0x42, 0x66, 0x18 };
static const uint8_t BM_COIN1[8] PROGMEM = { 0x10, 0x28, 0x28, 0x44, 0x44, 0x28, 0x28, 0x10 };
static const uint8_t BM_COIN2[8] PROGMEM = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
static const uint8_t* const BM_COIN_FRAMES[3] PROGMEM = { BM_COIN0, BM_COIN1, BM_COIN2 };
static const uint8_t BM_FUEL[8] PROGMEM = { 0x7C, 0x82, 0xBA, 0xA2, 0xBA, 0xA2, 0x82, 0x7C };

static inline void dropsInit(){ for (uint8_t i=0;i<MAX_DROPS;i++) drops[i].active=0; }
static inline uint8_t pickDropType(){ return (random(6)==0) ? DROP_FUEL : DROP_COIN; }

static void dropSpawnAtCenter(uint8_t cx, uint8_t cy, uint8_t type){
  for (uint8_t i=0;i<MAX_DROPS;i++){
    if (!drops[i].active){
      drops[i].active = 1;
      drops[i].type   = type;
      drops[i].x      = cx - 4;
      drops[i].y      = cy - 4;
      drops[i].vy     = 1;
      drops[i].t      = 0;
      drops[i].frame  = 0;
      return;
    }
  }
  drops[0].active=1; drops[0].type=type; drops[0].x=cx-4; drops[0].y=cy-4; drops[0].vy=1; drops[0].t=0; drops[0].frame=0;
}
static inline void dropSpawnMaybeAtCenter(uint8_t cx, uint8_t cy){
  dropSpawnAtCenter(cx, cy, pickDropType());
}

static void dropsStep(){
  for (uint8_t i=0;i<MAX_DROPS;i++){
    if (!drops[i].active) continue;
    int16_t ny = (int16_t)drops[i].y + (int16_t)drops[i].vy;
    if (ny > (int16_t)(SCREEN_HEIGHT-8)) { drops[i].active=0; continue; }
    if (ny < -8){ drops[i].active=0; continue; }
    drops[i].y = (uint8_t)ny;

    uint8_t cy = drops[i].y + 4;
    uint8_t l = gLeft[(cy>63)?63:cy];
    uint8_t r = gRight[(cy>63)?63:cy];
    uint8_t cx = drops[i].x + 4;
    if (cx <= l || cx >= r){ drops[i].active=0; continue; }

    drops[i].t++;
    if (drops[i].type == DROP_COIN){
      if ((drops[i].t % 3) == 0){
        if (drops[i].frame == 0) drops[i].frame = 1;
        else if (drops[i].frame == 1) drops[i].frame = 2;
        else drops[i].frame = 1;
      }
    }

    int16_t px0 = playerX - 5;
    int16_t py0 = PLAYER_Y - (PLAYER_H - 1);
    if (rectHit(px0, py0, PLAYER_W, PLAYER_H,
                (int16_t)drops[i].x + 1, (int16_t)drops[i].y + 1, 6, 6)){
      if (drops[i].type == DROP_COIN){ gScore += 5; } else { fuelMs = FUEL_MAX_MS; }
      drops[i].active = 0;
    }
  }
}
static void drawDrops(){
  for (uint8_t i=0;i<MAX_DROPS;i++){
    if (!drops[i].active) continue;
    if (drops[i].type == DROP_COIN){
      const uint8_t* bmp = (const uint8_t*)pgm_read_ptr(&BM_COIN_FRAMES[drops[i].frame]);
      display.drawBitmap(drops[i].x, drops[i].y, bmp, 8, 8, SSD1306_WHITE);
    } else {
      display.drawBitmap(drops[i].x, drops[i].y, BM_FUEL, 8, 8, SSD1306_WHITE);
    }
  }
}
static void dropsAnimTick(){
  for (uint8_t i=0;i<MAX_DROPS;i++){
    if (!drops[i].active) continue;
    drops[i].t++;
    if (drops[i].type == DROP_COIN){
      if ((drops[i].t % 3) == 0){
        if (drops[i].frame == 0) drops[i].frame = 1;
        else if (drops[i].frame == 1) drops[i].frame = 2;
        else drops[i].frame = 1;
      }
    }
  }
}

// UI
static void drawPlayerSpriteBigAt(int16_t cx, int16_t baseY){
  const uint16_t rows[6] = { 0x060, 0x7FE, 0x0FFF, 0x060, 0x0F0, 0x1F8 };
  for (int8_t r = 0; r < 6; ++r){
    uint16_t m = rows[r];
    int16_t y = baseY - 5 + r;
    for (int8_t c = 0; c < 12; ++c){
      if (m & (1 << (11 - c))){
        int16_t x = cx - 5 + c;
        if ((unsigned)x < SCREEN_WIDTH && (unsigned)y < SCREEN_HEIGHT)
          display.drawPixel(x, y, SSD1306_WHITE);
      }
    }
  }
}
static void drawHudLifeAt(int16_t cx, int16_t baseY){
  const uint8_t rows[4] = { 0b00100, 0b11111, 0b00100, 0b01110 };
  for (int8_t r = 0; r < 4; ++r){
    uint8_t m = rows[r];
    int16_t y = baseY - 3 + r;
    for (int8_t c = 0; c < 5; ++c){
      if (m & (1 << (4 - c))){
        int16_t x = cx - 2 + c;
        if ((unsigned)x < SCREEN_WIDTH && (unsigned)y < SCREEN_HEIGHT)
          display.drawPixel(x, y, SSD1306_WHITE);
      }
    }
  }
}
static void drawPlayer(){
  if (respawnFreezing || finalFreezing) return;
  uint8_t inv = (millis() < invUntilMs);
  if (!inv || blinkOn){
    drawPlayerSpriteBigAt(playerX, (int16_t)getPlayerY());
  }
}

// Intro rise (first second only)
static unsigned long introRiseStartMs = 0;
static const unsigned long INTRO_RISE_MS = 1000UL;

// current Y of player during intro rise (then sticks to PLAYER_Y)
static inline int16_t getPlayerY(){
  unsigned long now = millis();
  if ((long)(now - introRiseStartMs) < (long)INTRO_RISE_MS){
    int16_t y0 = SCREEN_HEIGHT + PLAYER_H;   // start: below visible area
    int16_t y1 = PLAYER_Y;                   // destination: normal resting position
    unsigned long t = now - introRiseStartMs;
    int32_t dy = (int32_t)y0 - (int32_t)y1;
    return (int16_t)(y0 - (dy * (int32_t)t) / (int32_t)INTRO_RISE_MS);
  }
  return (int16_t)PLAYER_Y;
}


static void drawLivesHUD(){
  uint8_t n = (livesRemaining > 0) ? (uint8_t)(livesRemaining - 1) : 0;
  if (n > 3) n = 3;
  int16_t cx = SCREEN_WIDTH - 3;
  for (uint8_t i=0;i<n;i++){
    int16_t by = (int16_t)SCREEN_HEIGHT - 2 - (int16_t)i * 6;
    drawHudLifeAt(cx, by);
  }
}
static void drawFuel(){
  display.drawRect(FUEL_X, FUEL_Y0, FUEL_W, FUEL_H, SSD1306_WHITE);
  uint8_t innerH = (FUEL_H > 2) ? (FUEL_H - 2) : 0;
  uint16_t h = (uint32_t)innerH * fuelMs / FUEL_MAX_MS;
  display.fillRect(FUEL_X+1, FUEL_Y0 + 1 + (innerH - h), (FUEL_W>2)?(FUEL_W-2):0, h, SSD1306_WHITE);
  display.setCursor(FUEL_X + FUEL_W + 2, FUEL_Y0); display.print(F("F"));
  int16_t eY = FUEL_Y0 + FUEL_H + 2; if (eY > (SCREEN_HEIGHT - 8)) eY = SCREEN_HEIGHT - 8;
  display.setCursor(FUEL_X + FUEL_W + 2, eY); display.print(F("E"));
}

// --- NEW: draw distance at top-right as "X.YKm" ---
static void drawDistanceHUD(){
  char buf[10];
  uint32_t km_i = gDistTenths / 10U;
  uint8_t  km_t = (uint8_t)(gDistTenths % 10U);
  // Ensure buffer big enough: up to "9999.9Km" fits in 9 chars incl. NUL
  snprintf(buf, sizeof(buf), "%lu.%uKm", (unsigned long)km_i, (unsigned)km_t);
  int16_t x = SCREEN_WIDTH - (int16_t)strlen(buf) * 6;
  if (x < 0) x = 0;
  display.setCursor(x, 0);
  display.print(buf);
}

// --- Decision cadence ---
static unsigned long gSpawnNextMs = 0;
static inline unsigned long spawnJitter(){ return 800UL + (unsigned long)random(401); } // 800..1200ms

// Game control
static void startNewGame(){
  gScore = 0;
  playerX = u8max(LEFT_UI_GUARD + 5, SCREEN_WIDTH/2);
  bulletsInit();
  ebulletsInit();
  explInit();
  helisInit();
  boatsInit();
  rocketsInit();
  dropsInit();
  riverInit();
  fuelMs = FUEL_MAX_MS;
  livesRemaining = 4;
  invUntilMs = 0;
  respawnFreezing = 0; finalFreezing = 0;
  gNewHighScore = 0;
  lastFrameMs = millis();
  straightUntilMs = lastFrameMs + STRAIGHT_MS;
  lastZeroMs = lastFrameMs;
  gSpawnStartMs = millis();
  gSpawnNextMs = lastFrameMs + spawnJitter();
  introRiseStartMs = millis(); 

  // reset distance
  gDistTenths = 0;
  distAccumMs = 0;
  distLastMs = millis();
}

// --- Game Over routing/page ---
static void gotoGameOver(){
  gNewHighScore = (gScore > gHighScore);
  if (gNewHighScore) commitHighScore(gScore);
  page = PAGE_GAMEOVER;
}
static void pageGameOver(uint8_t nextEdge, uint8_t prevEdge){
  if (nextEdge || prevEdge){ page = PAGE_START; return; }
  display.clearDisplay();
  if (gNewHighScore){ drawCenteredText(F("HIGHSCORE"), 8); }
  int16_t yGO = (SCREEN_HEIGHT - 16) / 2;
  drawCenteredTextSize(F("GAME OVER"), yGO, 2);
  int16_t spaceTop = yGO + 16;
  int16_t spaceBottom = SCREEN_HEIGHT;
  int16_t mid = (spaceTop + spaceBottom) / 2;
  int16_t yScore = mid - 4;
  char tbuf[24];
  snprintf(tbuf, sizeof(tbuf), "Score: %u", (unsigned)gScore);
  display.setCursor((SCREEN_WIDTH - (int)strlen(tbuf)*6)/2, yScore);
  display.print(tbuf);
  display.display();
}

// === Unified Spawns (weights unchanged) ===
static const uint8_t W_HELI   = 50;
static const uint8_t W_BOAT   = 35;
static const uint8_t W_ROCKET = 15;

static inline uint8_t heliHasFree(){ for (uint8_t i=0;i<3;i++) if (!helis[i].active) return 1; return 0; }
static inline uint8_t boatHasFree(){ for (uint8_t i=0;i<MAX_BOATS;i++) if (!boats[i].active) return 1; return 0; }
static inline uint8_t rocketHasFree(){ for (uint8_t i=0;i<MAX_ROCKETS;i++) if (!rockets[i].active) return 1; return 0; }

static uint8_t trySpawnHeli(){
  for (uint8_t i=0;i<3;i++){
    if (!helis[i].active){
      uint8_t ySpawn = 1;
      uint8_t l = gLeft[ySpawn], r = gRight[ySpawn];
      if (r > l + 10){
        int16_t minX = l + 2, maxX = r - 2 - 8;
        if (maxX >= minX){
          helis[i].x = (uint8_t)(minX + (int16_t)random((int)(maxX - minX + 1)));
          helis[i].y = ySpawn;
          helis[i].vx = (random(2)==0) ? 1 : -1;
          helis[i].phase = 0;
          helis[i].active = 1;
          return 1;
        }
      }
      break;
    }
  }
  return 0;
}
static uint8_t trySpawnBoat(){
  for (uint8_t i=0;i<MAX_BOATS;i++){
    if (!boats[i].active){
      uint8_t ySpawn = 0;
      uint8_t l = gLeft[ySpawn], r = gRight[ySpawn];
      if ((int16_t)r - (int16_t)l >= (int16_t)BOAT_W + 4){
        int16_t minX = l + 2, maxX = r - 2 - BOAT_W;
        uint8_t cx = (uint8_t)(minX + (int16_t)random((int)(maxX - minX + 1)));
        boats[i].x = cx; boats[i].y = ySpawn;
        boats[i].active = 1; boats[i].fired = 0;
        return 1;
      }
      break;
    }
  }
  return 0;
}
static uint8_t trySpawnRocket(){
  for (uint8_t i=0;i<MAX_ROCKETS;i++){
    if (!rockets[i].active){
      uint8_t ySpawn = 0;
      uint8_t l = gLeft[ySpawn], r = gRight[ySpawn];
      if ((int16_t)r - (int16_t)l >= (int16_t)ROCKET_W + 4){
        int16_t minX = l + 2, maxX = r - 2 - ROCKET_W;
        uint8_t cx = (uint8_t)(minX + (int16_t)random((int)(maxX - minX + 1)));
        rockets[i].x = cx; rockets[i].y = ySpawn;
        rockets[i].vy = 2;
        rockets[i].active = 1;
        return 1;
      }
      break;
    }
  }
  return 0;
}
static inline float spawnProbFromScore(uint16_t score){
  float p = 0.50f + 0.0002f * (float)score;
  if (p > 0.95f) p = 0.95f;
  if (p < 0.0f)  p = 0.0f;
  return p;
}
static void spawnEnemyTick(){
  unsigned long now = millis();
  if ((long)(now - gSpawnNextMs) < 0) return;
  gSpawnNextMs = now + spawnJitter();
  unsigned long t = now - gSpawnStartMs;
  if (t < 2500UL) return;
  float p = spawnProbFromScore(gScore);
  long  p1000 = (long)(p * 1000.0f + 0.5f);
  if (p1000 < 0) p1000 = 0; if (p1000 > 1000) p1000 = 1000;
  if (random(1000) >= p1000) return;
  uint8_t wH = heliHasFree()   ? W_HELI   : 0;
  uint8_t wB = boatHasFree()   ? W_BOAT   : 0;
  uint8_t wR = rocketHasFree() ? W_ROCKET : 0;
  if (t < 6000UL){ wB = 0; wR = 0; }
  else { if (gScore <= 300U) wR = 0; }
  for (uint8_t attempt=0; attempt<3; ++attempt){
    uint16_t sum = (uint16_t)wH + (uint16_t)wB + (uint16_t)wR;
    if (sum == 0) return;
    uint16_t r = (uint16_t)random(sum);
    if (r < wH){ if (trySpawnHeli()) return; wH = 0; }
    else if (r < (uint16_t)wH + (uint16_t)wB){ if (trySpawnBoat()) return; wB = 0; }
    else { if (trySpawnRocket()) return; wR = 0; }
  }
}

// --- Page Game ---
static void pageGameUpdateAndDraw(uint8_t shootEdge, uint8_t leftHeld, uint8_t rightHeld){
  // Gate: ignore carry-over holds from Start until both are released
  if (gInputGate){
    if (!leftHeld && !rightHeld) gInputGate = 0;  // drop gate only after full release
    leftHeld = 0;
    rightHeld = 0;
  }
  unsigned long now = millis();
  if (now - lastFrameMs >= FRAME_MS){
    if (respawnFreezing || finalFreezing){
      // During freeze: update only anims; ensure distance timer doesn't accumulate later
      explStep();
      helisAnimTick();
      dropsAnimTick();
      distLastMs = now; // cut distance time across freeze windows
    } else {
      unsigned long dt = now - lastFrameMs;
      lastFrameMs = now;

      // Distance accumulation (active time only)
      unsigned long ddt = now - distLastMs;
      distLastMs = now;
      distAccumMs += ddt;
      while (distAccumMs >= 1000UL){
        distAccumMs -= 1000UL;
        if (gDistTenths < 600000UL) gDistTenths++; // arbitrary large cap
      }

      riverStep();
      spawnEnemyTick();
      helisStep();
      boatsStep();
      rocketsStep();
      bulletsStep(shootEdge);
      ebulletsStep();
      dropsStep();   
      bulletHitsHelis();
      bulletHitsBoats();
      bulletHitsRockets();
      explStep();

      if (leftHeld)  { playerX -= 2; }
      if (rightHeld) { playerX += 2; }

      uint8_t lRow = gLeft[PLAYER_Y], rRow = gRight[PLAYER_Y];
      int16_t minCx = (int16_t)lRow + 5;
      int16_t maxCx = (int16_t)rRow - 6;
      if (minCx < LEFT_UI_GUARD + 1) minCx = LEFT_UI_GUARD + 1;
      if (maxCx > SCREEN_WIDTH - 2)  maxCx = SCREEN_WIDTH - 2;
      if (playerX < minCx) playerX = minCx;
      if (playerX > maxCx) playerX = maxCx;

      if (dt >= fuelMs) fuelMs = 0; else fuelMs -= (uint32_t)dt;
    }
  }

  if (!respawnFreezing && !finalFreezing){
    int8_t hidx = heliHitIndex();
    uint8_t heliHit = (hidx >= 0);
    if (heliHit){
      explSpawn(helis[hidx].x + 4, helis[hidx].y + 1);
      helis[hidx].active = 0;
    }
    uint8_t boatHit   = playerHitsBoat();
    uint8_t rocketHit = playerHitsRocket();
    uint8_t ebHit     = enemyBulletHitsPlayer();
    uint8_t shoreHit  = shoreTouch();

    uint8_t inv = (millis() < invUntilMs);
    if (!inv && (heliHit || shoreHit || boatHit || rocketHit || ebHit)){
      if (livesRemaining > 0){
        livesRemaining--;
        fuelMs = FUEL_MAX_MS;
        explSpawn((uint8_t)playerX, PLAYER_Y);
        bulletsInit();
        ebulletsInit();

        if (livesRemaining == 0){
          finalFreezing = 1;
          finalFreezeUntilMs = millis() + RESP_FREEZE_MS;
        } else {
          invUntilMs = millis() + INV_MS;
          respawnFreezing = 1;
          respawnFreezeUntilMs = millis() + RESP_FREEZE_MS;
        }
      }
    }
  }

  // Fuel-out acts like taking a hit: consume a life if any; else final freeze → GO
  if (fuelMs == 0 && !respawnFreezing && !finalFreezing){
    if (livesRemaining > 0){
      livesRemaining--;
      fuelMs = FUEL_MAX_MS;                    // like damage: refill fuel
      explSpawn((uint8_t)playerX, PLAYER_Y);   // explosion effect
      bulletsInit();                           // reset bullets
      ebulletsInit();

      if (livesRemaining == 0){
        finalFreezing = 1;
        finalFreezeUntilMs = millis() + RESP_FREEZE_MS;
      } else {
        invUntilMs = millis() + INV_MS;        // invulnerable after respawn
        respawnFreezing = 1;
        respawnFreezeUntilMs = millis() + RESP_FREEZE_MS;
      }
    }
  }


  display.clearDisplay();

  if (respawnFreezing && (long)(millis() - respawnFreezeUntilMs) >= 0){
    uint8_t lRow = gLeft[PLAYER_Y], rRow = gRight[PLAYER_Y];
    int16_t minCx = (int16_t)lRow + 5;
    int16_t maxCx = (int16_t)rRow - 6;
    if (minCx < LEFT_UI_GUARD + 1) minCx = LEFT_UI_GUARD + 1;
    if (maxCx > SCREEN_WIDTH - 2)  maxCx = SCREEN_WIDTH - 2;
    int16_t cx = ((int16_t)lRow + (int16_t)rRow) / 2;
    if (cx < minCx) cx = minCx;
    if (cx > maxCx) cx = maxCx;
    playerX = cx;

    invUntilMs = millis() + INV_MS;
    respawnFreezing = 0;
    respawnFreezeUntilMs = 0;
  }

  if (finalFreezing && (long)(millis() - finalFreezeUntilMs) >= 0){
    finalFreezing = 0;
    finalFreezeUntilMs = 0;
    gotoGameOver();
    return;
  }

  drawRiver();
  drawHelis();
  drawBoats();
  drawRockets();
  drawEnemyBullets();
  drawBullets();
  drawDrops();
  drawPlayer();
  drawFuel();
  drawLivesHUD();
  drawExpl();

  // Left-top: Score
  display.setCursor(0,0);
  display.print(F("S:"));
  display.print(gScore);

  // Right-top: Distance (new)
  drawDistanceHUD();

  display.display();
}

// --------------- Pages ---------------
static inline void drawHS(){ display.setCursor(0, 1); display.print(F("HS: ")); display.print(gHighScore); }
static void pageStart(uint8_t nextEdge, uint8_t prevEdge, uint8_t enterShort, uint8_t enterLong){
  if (enterLong){ page = PAGE_MENU; menuSel = 0; display.clearDisplay(); display.display(); return; }
  if (enterShort){ enterStandby(); return; }
  if (nextEdge || prevEdge){ gInputGate = 1; page = PAGE_GAME; startNewGame(); return; }
  display.clearDisplay();
  display.drawBitmap(0, 0, RIVER_BG_BITMAP, RIVER_BG_WIDTH, RIVER_BG_HEIGHT, SSD1306_WHITE);
  drawHS();
  if (blinkOn) drawCenteredText(F("press any key"), 48);
  display.display();
}
static void pageGame(){}
static void pageMenu(uint8_t nextEdge, uint8_t prevEdge, uint8_t selectEdge){
  if (nextEdge) menuSel = (menuSel + 1) % 3;
  if (prevEdge) menuSel = (menuSel + 3 - 1) % 3;
  if (selectEdge){
    if (menuSel == 0){ page = PAGE_ABOUT; }
    else if (menuSel == 1){ resetHighScore(); page = PAGE_START; }
    else { page = PAGE_START; }
    return;
  }
  display.clearDisplay();
  int16_t y0 = 20;
  for (uint8_t i=0;i<3;i++){
    display.setCursor(16, y0 + i*12);
    const char* p = (const char*)pgm_read_ptr(&MENU_ITEMS[i]);
    display.print((const __FlashStringHelper*)p);
  }
  display.setCursor(4, y0 + menuSel*12);
  display.print('>');
  display.display();
}
static void pageAbout(uint8_t anyEdge){
  display.clearDisplay();
  drawCenteredText(F("Created By"), 26);
  drawCenteredText(F("Ali Pournasseh"), 40);
  display.display();
  if (anyEdge){ page = PAGE_MENU; }
}

// --------------- Setup/Loop ---------------
void setup(){
  randomSeed(1);

  pinMode(BTN_ENTER, INPUT_PULLUP);
  pinMode(BTN_NEXT,  INPUT_PULLUP);
  pinMode(BTN_PREV,  INPUT_PULLUP);

  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) for(;;);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();

  loadHighScore();
}
void loop(){
  unsigned long now = millis();

  uint8_t enterNow = (digitalRead(BTN_ENTER) == LOW);
  uint8_t nextNow  = (digitalRead(BTN_NEXT)  == LOW);
  uint8_t prevNow  = (digitalRead(BTN_PREV)  == LOW);

  uint8_t enterShort = 0, enterLong = 0, enterEdge = 0;

  if (!enterPrev && enterNow){
    enterDownAt = now; enterLongFired = 0; enterEdge = 1;
  } else if (enterPrev && enterNow){
    if (!enterLongFired && (now - enterDownAt) >= ENTER_LONG_MS){
      enterLong = 1; enterLongFired = 1;
    }
  } else if (enterPrev && !enterNow){
    unsigned long dur = now - enterDownAt;
    if (!enterLongFired && dur >= ENTER_DEBOUNCE_MS && dur < ENTER_LONG_MS){
      enterShort = 1;
    }
  }
  enterPrev = enterNow;

  uint8_t nextEdge = (!nextNextPressedPrev && nextNow);
  uint8_t prevEdge = (!nextPrevPressedPrev && prevNow);
  nextNextPressedPrev = nextNow;
  nextPrevPressedPrev = prevNow;

  // Fast blink during invulnerability (~140ms), normal 500ms otherwise
  unsigned long blinkPeriod = (millis() < invUntilMs) ? 140UL : 500UL;
  if (now - blinkMs >= blinkPeriod){ blinkMs = now; blinkOn ^= 1; }

  switch (page){
    case PAGE_START:    pageStart(nextEdge, prevEdge, enterShort, enterLong); break;
    case PAGE_GAME:     pageGameUpdateAndDraw(enterEdge, prevNow, nextNow); break;
    case PAGE_MENU:     pageMenu(nextEdge, prevEdge, (enterShort||enterLong)); break;
    case PAGE_ABOUT:    pageAbout(enterShort||enterLong||nextEdge||prevEdge); break;
    case PAGE_GAMEOVER: pageGameOver(nextEdge, prevEdge); break;
  }
}