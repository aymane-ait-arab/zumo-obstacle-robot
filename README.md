# 🤖 Zumo Obstacle-Avoidance Robot — Reactive Navigation + Bluetooth Control

A Zumo-chassis robot with two operating modes: reactive autonomous obstacle avoidance, and manual Bluetooth drive control.

> 📎 Based on the project report *"Robot Mobile Zumo — Évitement d'Obstacles, Contrôle Bluetooth et Application Mobile Intégrée"* — Master ISOC, module Robotique, Faculté des Sciences de Meknès.

## Overview

Two independent Arduino sketches implement the two phases of the project:

1. **`code1_eviteur_obstacles`** — fully autonomous reactive obstacle avoidance
2. **`code2_bluetooth_control`** — Bluetooth-driven manual control, with an obstacle-avoidance safety interlock on the forward command

## Hardware

| Component | Role | Pins |
|---|---|---|
| Arduino Uno | Main controller | — |
| Zumo Shield v1.2 (Pololu) | Dual H-bridge DRV8835 motor driver, 4×AA power regulation | — |
| HC-SR04 | Ultrasonic obstacle detection | TRIG: A1 · ECHO: A0 |
| HC-05 | Bluetooth serial (SoftwareSerial) | RX: D4 (← HC-05 TXD) · TX: D5 (→ HC-05 RXD) |

The Zumo Shield's onboard DRV8835 H-bridge was chosen specifically for its mechanical compatibility with the Zumo chassis (the Mega form factor isn't compatible), and its headroom easily covers this project's needs (1 ultrasonic sensor, 1 Bluetooth module, 2 motors) within the Uno's 32 KB flash / 2 KB SRAM.

## Mode 1 — Reactive obstacle avoidance (`code1_eviteur_obstacles`)

Simple reactive perception-decision-action loop, chosen over a deliberative/planning approach due to the Arduino Uno's memory and compute constraints:

- **Perception:** continuous HC-SR04 distance polling
- **Decision:** threshold at **20 cm**
- **Action:** if clear → advance at `SPEED=200`; if obstacle → stop (200ms) → reverse-turn (`-TURN_SPEED/+TURN_SPEED` for 400ms) → brief pause (100ms) → resume

## Mode 2 — Bluetooth manual control (`code2_bluetooth_control`)

| Command received | Action |
|---|---|
| `F` | Forward (both motors at `SPEED`) — **blocked if an obstacle is within 15 cm** |
| `B` | Reverse |
| `L` | Turn left (`-TURN_SPEED` / `+TURN_SPEED`) |
| `R` | Turn right (`+TURN_SPEED` / `-TURN_SPEED`) |
| `S` | Stop |

HC-05 runs over `SoftwareSerial` (not the hardware UART) specifically so the hardware serial port stays free for USB debugging via Serial Monitor. The obstacle threshold here is tighter (15 cm vs. 20 cm in Mode 1) since forward motion is user-commanded rather than continuous.

## Mobile app

Control was done via an existing generic "Arduino Bluetooth Controller" app from the Google Play Store (not custom-built), adapted by aligning the Arduino-side command characters to match what the app sends — rather than the reverse. This was a deliberate trade-off: faster to working control, at the cost of no live distance readout on the phone (distance is only visible via Serial Monitor) and no UI customization (APK decompilation to adapt the interface wasn't successful).

## Repository structure

```
zumo-obstacle-robot/
├── code1_eviteur_obstacles/
│   └── _Code1_Eviteur.ino          # Autonomous reactive obstacle avoidance
├── code2_bluetooth_control/
│   └── _Code2_Bleuthooth.ino        # Bluetooth manual control + safety interlock
├── images/                           # Add your report screenshots here
└── README.md
```

> 🎥 Demo videos (`Contrôle Bluetooth.mp4`, `éviteur d'obstacle.mp4`) exist from testing but aren't included here — GitHub isn't ideal for video hosting. Consider uploading them to YouTube (unlisted) and linking them in this README instead.

## Measured results

| Paramètre | Mesuré | Spécifié | Statut |
|---|---|---|---|
| Latence Bluetooth | 80–120 ms | < 200 ms | ✅ |
| Précision évitement | 95% (19/20 tests) | > 95% | ✅ |
| Portée Bluetooth | 8 m (intérieur) | > 5 m | ✅ |
| Autonomie batterie | ~45 min | > 30 min | ✅ |

*Conditions de test : salle de classe 8m×6m, obstacles variés (murs, chaises, cartons, jambes), 4×AA alcalines neuves, Android 12 / Bluetooth 5.0.*

## What I'd improve next

- Replace the generic Play Store app with a custom one (own source) to add live distance readout
- Replace threshold-based avoidance with a weighted/fuzzy-logic approach for smoother navigation
- Use the Zumo Shield's onboard IMU (LSM6DS33 + LIS3MDL) for basic odometry

## Author

Aymane Ait Arab & Kawthar Derouich — M2 Intelligence et Sécurité des Objets Connectés, Faculté des Sciences de Meknès
