# STM32F446RE Bare-Metal Drivers

Навчальний проєкт із розробки низькорівневих драйверів периферії мікроконтролера **STM32F446RE** мовою C. Основна мета — вивчення архітектури STM32, принципів роботи периферійних модулів і безпосереднього керування апаратними Registers без використання HAL/LL.

Проєкт розвивається від базових експериментів із GPIO до реалізації власних драйверів і обміну даними через SPI. Він також використовується для практичного закріплення знань, отриманих під час курсу *Mastering Microcontroller and Embedded Driver Development (MCU_1)*.

## Особливості проєкту

- **Bare-Metal C:** безпосередня робота з Registers мікроконтролера.
- **Власний GPIO driver:** окремі Header і Source файли для конфігурації та керування GPIO.
- **Власний SPI driver:** реалізація драйвера SPI та навчальні приклади його використання.
- **GPIO Input/Output:** експерименти з керуванням LED і зчитуванням стану Push Button.
- **Interrupt:** окремий приклад керування LED за допомогою переривань.
- **SPI communication:** експерименти з передаванням даних і протоколом Command–Response між STM32 та Arduino.
- **Апаратні експерименти:** перевірка підключень і робота з логічним аналізатором та іншими вимірювальними інструментами.

## Апаратне забезпечення

- **MCU:** STM32F446RE, ARM Cortex-M4.
- **Development environment:** STM32CubeIDE.
- **Programming language:** Embedded C.
- **Logic analyzer:** Kingst LA2016.
- **Additional hardware:** Arduino Uno, TXS0108E logic level converter, LEDs, Push Buttons, breadboard і jumper wires.

## Структура репозиторію

```text
stm32f446xx_drivers/
├── Startup/
│   └── startup_stm32f446retx.s
├── Src/
│   ├── 000_TXS0108E_testing.c
│   ├── 001led_toggle.c
│   ├── 002Led_btn.c
│   ├── 003led_btn_ext.c
│   ├── 004led_btn_interrupt.c
│   ├── 005spi_tx_testing.c
│   ├── 006spi_tx_only_arduino.c
│   ├── 007spi_cmd_response_arduino.c
│   ├── syscalls.c
│   └── sysmem.c
├── drivers/
│   ├── Inc/
│   │   ├── stm32f446xx.h
│   │   ├── stm32f446xx_gpio_driver.h
│   │   └── stm32f446xx_spi_driver.h
│   └── Src/
│       ├── stm32f446xx_gpio_driver.c
│       └── stm32f446xx_spi_driver.c
├── STM32F446RETX_FLASH.ld
├── STM32F446RETX_RAM.ld
├── .cproject
├── .project
└── stm32f446xx_gpio_driver_MCU_1.launch
```

## Навчальні приклади

| Файл | Призначення |
|---|---|
| `000_TXS0108E_testing.c` | Експерименти з TXS0108E logic level converter |
| `001led_toggle.c` | Базове керування LED через GPIO |
| `002Led_btn.c` | Зчитування стану Push Button і керування LED |
| `003led_btn_ext.c` | Розширений приклад роботи з LED і Push Button |
| `004led_btn_interrupt.c` | Керування LED із використанням Interrupt |
| `005spi_tx_testing.c` | Початкові експерименти з передаванням даних через SPI |
| `006spi_tx_only_arduino.c` | SPI Master і передавання даних до Arduino |
| `007spi_cmd_response_arduino.c` | Обмін Command–Response між STM32 та Arduino через SPI Full Duplex |

Назви файлів відображають навчальну послідовність. Конкретні можливості кожного прикладу визначаються його реалізацією.

## Архітектура драйверів

Драйвери розділено на Header-файли в `drivers/Inc/` і реалізації в `drivers/Src/`.

- `stm32f446xx.h` — визначення, пов'язані з апаратними Registers STM32F446xx.
- `stm32f446xx_gpio_driver.h` — інтерфейс GPIO driver.
- `stm32f446xx_gpio_driver.c` — реалізація GPIO driver.
- `stm32f446xx_spi_driver.h` — інтерфейс SPI driver.
- `stm32f446xx_spi_driver.c` — реалізація SPI driver.

Такий поділ відокремлює інтерфейс драйвера від його реалізації та дозволяє використовувати драйвери в різних навчальних прикладах.

## SPI: STM32 Master — Arduino Slave

Окремий напрям проєкту — дослідження SPI Full Duplex та реалізація обміну командами й відповідями між STM32F446RE та Arduino Uno.

Основні теми експериментів:

- налаштування SPI Master і SPI Slave;
- конфігурація SPI Registers;
- передавання та приймання даних через MOSI і MISO;
- робота SCLK і NSS;
- взаємодія TX buffer, RX buffer і Shift Register;
- реалізація протоколу Command–Response;
- перевірка часових параметрів сигналів за допомогою логічного аналізатора.

Для підключення між платами використовується TXS0108E logic level converter. Конкретні параметри SPI та результати вимірювання сигналів документуються відповідно до фактичних експериментів.

## Середовище розробки

Проєкт організовано для роботи у STM32CubeIDE. Репозиторій містить файли проєкту IDE, startup-код і linker scripts для STM32F446RETX.

Для відтворення експериментів потрібно відкрити проєкт у STM32CubeIDE, перевірити конфігурацію build environment і апаратні підключення, після чого зібрати та завантажити відповідний приклад у MCU.

## Мета проєкту

Мета — послідовно перейти від розуміння апаратної архітектури STM32 до самостійної реалізації та перевірки драйверів периферії.

Проєкт охоплює шлях від налаштування окремих GPIO Registers до організації обміну даними між мікроконтролерами через SPI. Надалі документація, приклади, результати тестування та структура драйверів розвиватимуться разом із проєктом.

