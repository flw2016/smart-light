# 🚦 Smart Traffic Light

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![ESP32](https://img.shields.io/badge/ESP32-PlatformIO-green.svg)](https://platformio.org/)
[![Wokwi](https://img.shields.io/badge/Wokwi-Simulation-orange.svg)](https://wokwi.com/)

Проект «Умный светофор» на базе ESP32 с конечным автоматом, неблокирующим таймером и обработкой прерываний. Реализован на C++ с использованием PlatformIO и готов к симуляции в Wokwi.

## 🎯 Возможности

- **Три фазы светофора:** зелёный (5 сек), жёлтый (3 мигания), красный (5 сек).
- **Многозадачность на FreeRTOS:** две независимые задачи (светофор и обработка кнопки).
- **Синхронизация через семафор:** кнопка мгновенно включает красный, не блокируя цикл.
- **Неблокирующее ожидание:** собственная функция `waitWithSemaphore()` вместо `delay()`.
- **Явное управление пинами** в каждой фазе — никаких «висящих» состояний.

## 🛠 Технологии

| Компонент | Технология |
| :--- | :--- |
| **Язык** | C++17 |
| **Плата** | ESP32 (ESP32 Dev Module) |
| **Сборка** | PlatformIO |
| **Симуляция** | Wokwi (diagram.json + wokwi.toml) |
| **RTOS** | FreeRTOS (задачи, семафоры) |
| **Таймер** | `vTaskDelay` + неблокирующее ожидание |
| **Прерывания** | `attachInterrupt` + `volatile bool` |
| **Архитектура** | Конечный автомат + паттерн ожидания с проверкой |

## 📦 Структура проекта
```
smart-light/
├── main.cpp # Прошивка светофора
├── diagram.json # Схема для Wokwi
├── wokwi.toml # Конфигурация симулятора
├── platformio.ini # Конфигурация PlatformIO
└── README.md # Этот файл
```

## 🚀 Быстрый старт

### Симуляция в Wokwi

1. Открой [Wokwi.com](https://wokwi.com).
2. Создай новый проект ESP32.
3. Скопируй содержимое `main.cpp` в редактор кода.
4. Скопируй содержимое `diagram.json` во вкладку `diagram.json`.
5. Нажми **Start Simulation**.

### Сборка через PlatformIO

```bash
git clone https://github.com/flw2016/smart-light.git
cd smart-light
pio run
```
Заливка на реальную плату
```bash
pio run --target upload
```
## 🧠 Как это работает

1. **`TaskTrafficLight`** — основная задача светофора: перебирает фазы и проверяет семафор.
2. **`TaskButton`** — задача обработки кнопки: поднимает семафор при нажатии.
3. **`waitWithSemaphore()`** — неблокирующая задержка: спит по 100 мс, проверяя семафор. При нажатии немедленно включает красный.
