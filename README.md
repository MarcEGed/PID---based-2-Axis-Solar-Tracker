# 2-Axis Solar Tracker with PID

## Overview
Arduino nano-based 2-axis solar tracker that uses PID control to follow the brightest light using photoresistors. Logs data via serial for analysis and python plotting.

## Features
- Horizontal and vertical tracking
- PID control with adjustable `Kp`, `Ki`, `Kd`
- CSV logging: time, posH, posV, errorH, errorV
- Python plotting for the CSV data