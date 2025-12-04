🦾⚙️🦾⚙️🦾⚙️🦾⚙️🦾⚙️🦾⚙️🦾⚙️🦾⚙️🦾⚙️🦾⚙️🦾⚙️🦾

# Tripteron Robot Controller (ESP32)

This repository contains the control firmware for a Tripteron-type parallel kinematics robot. The system was developed in **C++** for the ESP32 microcontroller, utilizing the **FreeRTOS** real-time operating system.

The main focus of this project is the generation of smooth and synchronized movements through the efficient use of hardware peripherals (RMT) and concurrent programming (Multithreading).

## 🚀🧩 Features

* 📌**Parallel Kinematics:** Coordinated control of orthogonal linear axes.
* 📌**Hardware Pulse Generation (RMT):** Uses the ESP32's *Remote Control Transceiver* peripheral to generate step pulses (STEP) with microsecond precision, without occupying the main CPU (zero jitter).
* 📌**Multithreading & Synchronization:** Each axis operates in its own dedicated thread. Synchronized movement (interpolation) is guaranteed through semaphores (`std::counting_semaphore`), allowing for complex trajectories such as circles.
* 📌**Auto-Calibration (Homing):** Automatic routine for physical limit detection and stroke mapping via limit switches (endstops).
* 📌**Relative Positioning:** Movement abstraction based on a percentage of the total stroke (0% to 100%), independent of the physical number of steps.

## 🛠️ Required Hardware

* 📌**MCU:** ESP32 Development Board (ESP32 DevKit V1).
* 📌**Drivers:** 3x Stepper Motor Drivers (DRV8825).
* 📌**Actuators:** 3x Stepper Motors (NEMA 17).
* 📌**Sensors:** 3x Limit Switches (Mechanical Endstops).

## 🔌 Pinout

The default pin configuration is defined in `Tripteron.hpp`.

| Axis | STEP (RMT) | DIR (Digital) | Endstop (Input Pullup) |
| :--- | :--- | :--- | :--- |
| **X Axis** | GPIO 23 | GPIO 25 | GPIO 14 |
| **Y Axis** | GPIO 22 | GPIO 26 | GPIO 12 |
| **Z Axis** | GPIO 32 | GPIO 27 | GPIO 13 |

## 💻 Software Architecture

The project follows a modular object-oriented architecture:

* `main.cpp`: Entry point. Generates the mathematical trajectory (e.g., circle) and sends commands to the robot.
* `Tripteron.hpp`: Main class that orchestrates the axes. Manages threads and "Fork-Join" synchronization.
* `Axis.hpp`: Represents a logical axis. Converts percentage to steps and manages calibration.
* `Motor.hpp`: Low-level driver. Configures the RMT peripheral for sending pulse bursts.
* `RMT.hpp`: C++ wrapper for the ESP-IDF RMT C API.

### Execution Diagram (Multithreading)
Movement (x, y) is executed by splitting the task into two simultaneous threads. The processor waits for both to finish before processing the next trajectory point, ensuring perfect synchronization.

<img width="1208" height="1733" alt="Untitled diagram-2025-12-04-172610" src="https://github.com/user-attachments/assets/94fd68ba-5c7b-4dfd-bcd9-c8017e855c29" />


