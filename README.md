# 🔐 Security Door System

This project implements a **Door Locker Security System** using two **ATmega32** microcontrollers. The system ensures **secure access control** by using a password-based authentication mechanism, a **buzzer alarm**, and a **motorized lock mechanism**.

## 🚀 Project Overview

The **Security Door System** follows a **layered architecture** with two microcontrollers:
- **HMI_ECU (Human Machine Interface)**: Handles **user input via keypad** and **displays messages on an LCD**.
- **CONTROL_ECU**: Manages **password verification, EEPROM storage, motor control, and security alarms**.

## 📌 Features

### 1️⃣ **Password-Based Authentication**
- Users set a **5-digit password**.
- Passwords are stored securely in **EEPROM**.
- Password verification is required to unlock the door.

### 2️⃣ **Motorized Door Locking System**
- The **DC motor** rotates **clockwise** to unlock the door.
- The motor **holds for 3 seconds** before reversing to lock the door.

### 3️⃣ **Security Mechanism**
- **Three failed attempts** trigger the **buzzer alarm** for **1 minute**.
- System locks out user input for the duration of the alarm.

### 4️⃣ **Password Reset Functionality**
- Users can update their password after verification.
- New passwords must be entered **twice for confirmation**.

## 🛠️ System Components

### **Hardware**
- **ATmega32 Microcontrollers** (HMI_ECU & CONTROL_ECU)
- **16x2 LCD** for displaying messages
- **4x4 Keypad** for user input
- **DC Motor** for door locking
- **EEPROM** for password storage
- **Buzzer** for security alerts

### **Software Modules**
- **GPIO Driver** for hardware interfacing
- **LCD Driver** for message display
- **Keypad Driver** for password input
- **UART Driver** for MCU communication
- **I2C Driver** for EEPROM storage
- **Timer Driver** for motor & display timing
- **Buzzer Driver** for security alerts

## 🔧 System Workflow

1️⃣ **User sets a new password** → Password saved in EEPROM.  
2️⃣ **User enters password** → System verifies stored password.  
3️⃣ **If correct:** Unlocks door for **15 seconds**, then relocks.  
4️⃣ **If incorrect:** Three failed attempts trigger the alarm.  
5️⃣ **User can reset password** after successful authentication.  


