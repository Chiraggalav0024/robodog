import serial
import tkinter as tk

# CHANGE THIS to your port
SERIAL_PORT = '/dev/ttyACM0'

BAUD = 9600

ser = serial.Serial(SERIAL_PORT, BAUD, timeout=1)

def send_angle(servo, value):
    cmd = f"s{servo}={int(value)}\n"
    ser.write(cmd.encode())

root = tk.Tk()
root.title("GoodBoy Servo Tuner")

for i in range(1, 9):
    slider = tk.Scale(
        root,
        from_=0,
        to=180,
        orient=tk.HORIZONTAL,
        length=400,
        label=f"Servo {i}",
        command=lambda v, s=i: send_angle(s, v)
    )
    slider.set(90)
    slider.pack()

root.mainloop()
