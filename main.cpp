#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

// Пины для светодиодов
const int LED_PIN_RED = 23;
const int LED_PIN_YELLOW = 22;
const int LED_PIN_GREEN = 21;

// Пин для кнопки
const int BUTTON = 19;

// Семафор для синхронизации (глобальный)
SemaphoreHandle_t xSemaphore = NULL;

void waitWithSemaphore(int milliseconds) {
    int steps = milliseconds / 100;
    for (int i = 0; i < steps; i++) {
        if (xSemaphoreTake(xSemaphore, 0) == pdTRUE) {
            digitalWrite(LED_PIN_RED, HIGH);
            digitalWrite(LED_PIN_YELLOW, LOW);
            digitalWrite(LED_PIN_GREEN, LOW);
            vTaskDelay(pdMS_TO_TICKS(milliseconds));
            return;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Задача 1: Сам светофор (автономный цикл)
void TaskTrafficLight(void *pvParameters) {
    while (1) {
        // Проверяем, не нажата ли кнопка (семафор "поднят"?)
        if (xSemaphoreTake(xSemaphore, 0) == pdTRUE) {
            // Кнопка нажата! Включаем красный на 5 секунд
            digitalWrite(LED_PIN_RED, HIGH);
            digitalWrite(LED_PIN_YELLOW, LOW);
            digitalWrite(LED_PIN_GREEN, LOW);
            waitWithSemaphore(5000);
        }
        
        // Зелёный на 5 секунд
        digitalWrite(LED_PIN_RED, LOW);
        digitalWrite(LED_PIN_YELLOW, LOW);
        digitalWrite(LED_PIN_GREEN, HIGH);
        waitWithSemaphore(5000);

        // Жёлтый мигает 3 раза
        digitalWrite(LED_PIN_GREEN, LOW);
        for (int i = 0; i < 3; i++) {
            digitalWrite(LED_PIN_YELLOW, HIGH);
            waitWithSemaphore(500);
            digitalWrite(LED_PIN_YELLOW, LOW);
            waitWithSemaphore(500);
        }

        // Красный на 5 секунд
        digitalWrite(LED_PIN_RED, HIGH);
        waitWithSemaphore(5000);
    }
}

// Задача 2: Обработчик кнопки (ждёт семафор) 
void TaskButton(void *pvParameters) {
    while (1) {
        // Ждём, пока кнопка будет нажата
        if (digitalRead(BUTTON) == LOW) {
            vTaskDelay(pdMS_TO_TICKS(50)); // Антидребезг (ждём 50 мс)
            if (digitalRead(BUTTON) == LOW) {
                // Даём семафор
                xSemaphoreGive(xSemaphore);
                // Ждём, пока кнопку отпустят
                while (digitalRead(BUTTON) == LOW) {
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
                vTaskDelay(pdMS_TO_TICKS(50)); // Ещё антидребезг на отпускание
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Даём процессору отдохнуть
    }
}

void setup() {
    // Настройка пинов
    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_YELLOW, OUTPUT);
    pinMode(LED_PIN_GREEN, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);

    // Гасим всё при старте
    digitalWrite(LED_PIN_RED, LOW);
    digitalWrite(LED_PIN_YELLOW, LOW);
    digitalWrite(LED_PIN_GREEN, LOW);

    // Создаём семафор
    xSemaphore = xSemaphoreCreateBinary();

    // Создаём задачу 1 (Светофор)
    xTaskCreate(
        TaskTrafficLight,   // Функция-обработчик
        "TrafficLight",     // Имя задачи (для отладки)
        2048,               // Размер стека в байтах
        NULL,               // Параметры (не передаём)
        1,                  // Приоритет (выше число - выше приоритет)
        NULL                // Дескриптор задачи (не сохраняем)
    );

    // Создаём задачу 2 (Кнопка)
    xTaskCreate(
        TaskButton,
        "Button",
        2048,
        NULL,
        2, // У кнопки приоритет выше, чтобы реакция была мгновенной
        NULL
    );
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}