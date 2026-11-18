#include <DHT.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// ===== PINNID =====
const int PIN_BTN = 2;  // nupp -> GND
const int PIN_LED = 8;  // D8 -> 220 Ω takisti -> LED -> GND
const int PIN_DHT = A0; // DHT22 andmepin (DATA)
const int PIN_BL = 9;   // LCD taustvalgus (PWM) -> I2C-adapteri LED-džämperi viik

#define DHTTYPE DHT22
DHT dht(PIN_DHT, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2); // kui ekraan on tühi, proovi 0x3F

const float T_LIMIT = 22.0; // temperatuuri piir (°C)

const unsigned long READ_INTERVAL =
    300000UL; // mõõtmise intervall: 5 min (nõue 11)
const unsigned long RETRY_INTERVAL =
    5000UL; // anduri vea korral proovi uuesti 5 s pärast
const unsigned long HUM_TIMEOUT =
    30000UL; // niiskuse kuvamise aeg enne tagasipöördumist
const unsigned long DEBOUNCE_MS = 50UL; // nupu põrke filtreerimine
const unsigned long BLINK_MS = 300UL; // LED-i vilkumise intervall veaolukorras

// ===== TAUSTVALGUS =====
const byte BL_DIM = 51;   // tavaline heledus: 20 % (51 / 255)
const byte BL_FULL = 255; // heledus pärast nupuvajutust: 100 %
const unsigned long BL_TIMEOUT =
    30000UL; // kui kaua on taustvalgus pärast vajutust täisheledusel

// ===== LOGI =====
const int LOG_SLOTS = 512;     // logi kohtade arv (1024 baiti / 2) ≈ 42 h
const uint16_t EMPTY = 0xFFFF; // tühja koha tähis
const byte LOG_EVERY =
    1;               // salvesta iga N-s mõõtmine (1 = iga 5 min järel)
int logHead = 0;     // järgmise kirje asukoht
byte logCounter = 0; // mõõtmiste loendur logimiseks

float temperature = 0;     // viimane temperatuur
float humidity = 0;        // viimane õhuniiskus
bool sensorOk = false;     // kas andur töötab
bool showHumidity = false; // kas ekraanil on niiskus

unsigned long lastRead = 0;      // viimase mõõtmise aeg
unsigned long humStart = 0;      // niiskuse kuvamise algus
unsigned long lastBtnChange = 0; // nupu viimase muutuse aeg
unsigned long lastBlink = 0;     // LED-i viimase vilkumise aeg
bool lastBtnState = HIGH;        // nupu eelmine olek
bool blinkState = false;         // LED-i vilkumise olek
unsigned long blStart = 0;       // täisheleduse algus
bool blFull = false;             // kas taustvalgus on täisheledusel

// ================= EEPROM-LOGI =================
// Loeb ühe logikirje EEPROM-ist
uint16_t readSlot(int i) {
  uint16_t v;
  EEPROM.get(i * 2, v);
  return v;
}

// Leiab esimese tühja koha logis (kust jätkata kirjutamist)
void findHead() {
  logHead = 0;
  for (int i = 0; i < LOG_SLOTS; i++) {
    if (readSlot(i) == EMPTY) {
      logHead = i;
      return;
    }
  }
}

// Salvestab temperatuuri logisse (kodeeritud 16-bitiseks arvuks)
void logTemp(float t) {
  float x = (t + 40.0) * 10.0; // nihe -40 °C ja täpsus 0,1 °C
  if (x < 0)
    x = 0;
  if (x > 1200)
    x = 1200;
  uint16_t v = (uint16_t)(x + 0.5);
  EEPROM.put(logHead * 2, v);
  logHead = (logHead + 1) % LOG_SLOTS; // ringpuhver
  EEPROM.put(logHead * 2, EMPTY);      // märgib logi lõpu
}

// Väljastab logi Serial Monitori (vanimast uusimani)
void dumpLog() {
  Serial.println(F("sek_tagasi,temp_C"));
  unsigned long step =
      READ_INTERVAL / 1000 * LOG_EVERY; // sekundeid kirjete vahel
  for (int k = 1; k < LOG_SLOTS; k++) {
    int idx = (logHead + k) % LOG_SLOTS;
    uint16_t v = readSlot(idx);
    if (v == EMPTY)
      continue;
    Serial.print((unsigned long)(LOG_SLOTS - 1 - k) * step);
    Serial.print(",");
    Serial.println(v / 10.0 - 40.0, 1);
  }
  Serial.println(F("--- lopp ---"));
}

// Kustutab kogu logi
void clearLog() {
  for (unsigned int i = 0; i < EEPROM.length(); i++)
    EEPROM.update(i, 0xFF);
  logHead = 0;
  Serial.println(F("logi kustutatud"));
}

// ================= ANDUR JA EKRAAN =================
// Loeb DHT22 andmed, juhib LED-i ja salvestab logisse
void readSensors() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // kui andmed on vigased, märgi andur mittetöötavaks
  if (isnan(t) || isnan(h)) {
    sensorOk = false;
    Serial.println(F("DHT22 viga"));
    return;
  }
  sensorOk = true;
  temperature = t;
  humidity = h;

  // LED põleb, kui temperatuur on alla piiri
  digitalWrite(PIN_LED, temperature < T_LIMIT ? HIGH : LOW);

  // logimine iga LOG_EVERY mõõtmise järel
  logCounter++;
  if (logCounter >= LOG_EVERY) {
    logCounter = 0;
    logTemp(temperature);
  }
}

// Uuendab LCD ekraani sisu
void updateDisplay() {
  lcd.setCursor(0, 0);
  if (!sensorOk) { // anduri viga
    lcd.print("Anduri viga     ");
    lcd.setCursor(0, 1);
    lcd.print("Kontrolli DHT22 ");
    return;
  }
  if (showHumidity) { // niiskuse vaade
    lcd.print("Ohuniiskus:     ");
    lcd.setCursor(0, 1);
    lcd.print(humidity, 1);
    lcd.print(" %RH         ");
  } else { // temperatuuri vaade
    lcd.print("Temperatuur:    ");
    lcd.setCursor(0, 1);
    lcd.print(temperature, 1);
    lcd.print((char)223); // kraadimärk
    lcd.print("C          ");
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_BTN, INPUT_PULLUP); // sisemine pull-up, nupp ühendab GND-ga
  pinMode(PIN_LED, OUTPUT);

  dht.begin();
  lcd.init();
  lcd.backlight(); // adapteri taustvalguse lüliti jääb sisse, heledust juhib PIN_BL
  pinMode(PIN_BL, OUTPUT);
  analogWrite(PIN_BL, BL_DIM);

  findHead();  // leia koht logis
  delay(2000); // DHT22 vajab pärast sisselülitamist aega
  readSensors();
  lastRead = millis();
  updateDisplay();
}

void loop() {
  unsigned long now = millis();

  // käsud Serial Monitorist: d = näita logi, c = kustuta logi
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'd')
      dumpLog();
    if (c == 'c')
      clearLog();
  }

  // nupu töötlemine (põrke filtreerimisega)
  bool btn = digitalRead(PIN_BTN);
  if (btn != lastBtnState && (now - lastBtnChange) > DEBOUNCE_MS) {
    lastBtnChange = now;
    lastBtnState = btn;
    if (btn == LOW) {               // nupp on vajutatud
      showHumidity = !showHumidity; // vaheta temperatuur/niiskus
      if (showHumidity)
        humStart = now;
      updateDisplay();

      // taustvalgus täisheledusele
      analogWrite(PIN_BL, BL_FULL);
      blFull = true;
      blStart = now;
    }
  }

  // taustvalgus tagasi 20 % peale, kui nuppu pole 30 s vajutatud
  if (blFull && (now - blStart >= BL_TIMEOUT)) {
    blFull = false;
    analogWrite(PIN_BL, BL_DIM);
  }

  // automaatne tagasipöördumine temperatuuri vaatele 30 s pärast
  if (showHumidity && (now - humStart >= HUM_TIMEOUT)) {
    showHumidity = false;
    updateDisplay();
  }

  // perioodiline mõõtmine (vea korral proovitakse kiiremini uuesti)
  unsigned long interval = sensorOk ? READ_INTERVAL : RETRY_INTERVAL;
  if (now - lastRead >= interval) {
    lastRead = now;
    readSensors();
    updateDisplay();
  }

  // anduri vea korral LED vilgub
  if (!sensorOk && (now - lastBlink >= BLINK_MS)) {
    lastBlink = now;
    blinkState = !blinkState;
    digitalWrite(PIN_LED, blinkState);
  }
}