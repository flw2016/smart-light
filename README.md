# 🚦 Smart Traffic Light

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![ESP32](https://img.shields.io/badge/ESP32-PlatformIO-green.svg)](https://platformio.org/)
[![Wokwi](https://img.shields.io/badge/Wokwi-Simulation-orange.svg)](https://wokwi.com/)

Проект «Умный светофор» на базе ESP32 с конечным автоматом, неблокирующим таймером и обработкой прерываний. Реализован на C++ с использованием PlatformIO и готов к симуляции в Wokwi.

## 🎯 Возможности

- **Три фазы светофора:** зелёный (5 сек), жёлтый (3 мигания), красный (5 сек).
- **Конечный автомат** на `switch/case` с чистой сменой состояний.
- **Неблокирующий таймер** на `millis()` — ни одного `delay()`.
- **Аппаратное прерывание** по нажатию кнопки — мгновенный переход на красный.
- **Явное управление пинами** в каждой фазе — никаких «висящих» состояний.

## 🛠 Технологии

| Компонент | Технология |
| :--- | :--- |
| **Язык** | C++17 |
| **Плата** | ESP32 (ESP32 Dev Module) |
| **Сборка** | PlatformIO |
| **Симуляция** | Wokwi (diagram.json + wokwi.toml) |
| **Таймер** | `millis()` (неблокирующий) |
| **Прерывания** | `attachInterrupt` + `volatile bool` |
| **Архитектура** | Конечный автомат (State Machine) |

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
# 🧠 Как это работает
- `setup()` — настраивает пины светодиодов (23, 22, 21) и кнопки (19), вешает прерывание.

- `loop()` — вызывает `switch(state)`, где:

- `case 0`: зелёный горит 5 секунд.

- `case 1`: жёлтый мигает 3 раза с интервалом 500 мс.

- `case 2`: красный горит 5 секунд.

- Прерывание — по нажатию кнопки взводится флаг `volatile bool emergency`, который принудительно переключает state = 2 (красный) и сбрасывает таймер.
