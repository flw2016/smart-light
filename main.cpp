#include <Arduino.h>

const int LED_PIN_RED = 23;
const int LED_PIN_YELLOW = 22;
const int LED_PIN_GREEN = 21;
const int BUTTON = 19;

int state = 0;
int blink = 6;

unsigned long lastTime = 0;

volatile bool emergency = false;

void IRAM_ATTR button_isr() {
    emergency = true; // Просто ставим флаг тревоги
}


void setup() {
    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_YELLOW, OUTPUT);
    pinMode(LED_PIN_GREEN, OUTPUT);
  
  // Гасим всё при старте
    digitalWrite(LED_PIN_RED, LOW);
    digitalWrite(LED_PIN_YELLOW, LOW);
    digitalWrite(LED_PIN_GREEN, LOW);
    pinMode(BUTTON, INPUT_PULLUP);
    // "Повесить" функцию button_isr на пин 19. Вызывать, когда сигнал упадет (FALLING).
    attachInterrupt(digitalPinToInterrupt(BUTTON), button_isr, FALLING);
}

void loop() {
    unsigned long currentTime = millis();
    if (emergency) {
        emergency = false;  // Сбросить флаг
        state = 2;          // Принудительно перейти на красный
        lastTime = currentTime; // Сбросить таймер
    }
    switch (state)
    {
    case 0: // Зелёный
        digitalWrite(LED_PIN_RED, LOW);
        digitalWrite(LED_PIN_YELLOW, LOW);
        digitalWrite(LED_PIN_GREEN, HIGH);
        if(currentTime - lastTime >= 5000){
            lastTime = currentTime;
            digitalWrite(LED_PIN_GREEN, LOW);
            blink = 6;
            state = 1;
        }
        break;
    case 1: // Жёлтый
        digitalWrite(LED_PIN_RED, LOW);
        digitalWrite(LED_PIN_GREEN, LOW);
        if(currentTime - lastTime >= 500 && blink > 0){
            lastTime = currentTime;
            digitalWrite(LED_PIN_YELLOW, !digitalRead(LED_PIN_YELLOW));
            blink--;
        }
        else if(blink == 0){
            lastTime = currentTime;
            state = 2;
        }
        break;
    case 2: // Red
        digitalWrite(LED_PIN_RED, HIGH);
        digitalWrite(LED_PIN_YELLOW, LOW);
        digitalWrite(LED_PIN_GREEN, LOW);
        if(currentTime - lastTime >= 5000){
            lastTime = currentTime;
            digitalWrite(LED_PIN_RED, LOW);
            state = 0;
        }
        break;
    }
}