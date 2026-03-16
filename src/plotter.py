import pandas as pd
import matplotlib.pyplot as plt
import os

df = pd.read_csv('low_p_0.05.csv')

output_folder = "low_p_0.05_results"
os.makedirs(output_folder, exist_ok=True)

plt.figure(figsize=(10,5))
plt.plot(df['time'], df['posH'], label='posH', color='blue')
plt.plot(df['time'], df['posV'], label='posV', color='orange')
plt.xlabel('Time (ms)')
plt.ylabel('Servo Position')
plt.title('Servo Positions vs Time')
plt.legend()
plt.grid(True)

plt.savefig(os.path.join(output_folder, "servo_positions.png"), dpi=300)
plt.close()

plt.figure(figsize=(10,5))
plt.plot(df['time'], df['errorH'], label='errorH', color='green')
plt.plot(df['time'], df['errorV'], label='errorV', color='red')

sigH = df['errorH'].abs() > 50
sigV = df['errorV'].abs() > 50
plt.scatter(df['time'][sigH], df['errorH'][sigH], color='black', marker='o', label='errorH > 50')
plt.scatter(df['time'][sigV], df['errorV'][sigV], color='purple', marker='x', label='errorV > 50')

plt.xlabel('Time (ms)')
plt.ylabel('Error')
plt.title('Servo Errors vs Time')
plt.legend()
plt.grid(True)

plt.savefig(os.path.join(output_folder, "servo_errors.png"), dpi=300)
plt.close()