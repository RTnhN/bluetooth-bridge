import serial
import keyboard
from pynput import mouse
import pygetwindow as gw
import pyperclip
import sys
import time

# Global variables
send_keystrokes = False
spinner_position = 0
spinner_states = ['|', '/', '-', '\\']  # Spinner animation frames

IGNORE_WORDS = ["shift", "caps lock"]

# Setup serial connection (adjust 'COM8' to your Arduino's serial port)
ser = serial.Serial('COM8', 115200, timeout=1)

def send_command(command):
    # Wrapping the command in curly braces
    wrapped_command = f"{{{command}}}"
    ser.write(wrapped_command.encode())

def on_press(event):
    global send_keystrokes, spinner_position, ctrl_pressed
    if send_keystrokes:
        clear_line()
        if event.name == "ctrl":
            ctrl_pressed = True
        elif event.name == "v" and ctrl_pressed:
            clipboard_content = pyperclip.paste()
            for char in clipboard_content:
                send_command(char)
            ctrl_pressed = False
        elif event.name == "space":
            send_command("SPACE")
        elif event.name == "backspace":
            send_command("BACKSPACE")
        elif event.name == "enter":
            send_command("ENTER")
        elif event.name in IGNORE_WORDS:
            pass
        elif event.name == "right":
            send_command("RIGHT")
        elif event.name == "left":
            send_command("LEFT")
        elif event.name == "up":
            send_command("UP")
        elif event.name == "down":
            send_command("DOWN")
        else:
            # Sending regular key press as normal text
            send_command(event.name)
    else:
        # Update spinner and print warning message
        clear_line()
        sys.stdout.write(f"\rKeystrokes not sent. Not in 'Reflector 4' window. {spinner_states[spinner_position]} ")
        sys.stdout.flush()
        spinner_position = (spinner_position + 1) % 4  # Update spinner position

def on_click(x, y, button, pressed):
    global send_keystrokes
    if pressed:
        window_title = "Reflector 4"
        try:
            window = gw.getWindowsWithTitle(window_title)[0]
            if window:
                win_left, win_top, win_width, win_height = window.left, window.top, window.width, window.height
                if win_left <= x <= win_left + win_width and win_top <= y <= win_top + win_height:
                    send_keystrokes = True
                else:
                    send_keystrokes = False
        except IndexError:
            print(f"\rNo window with title '{window_title}' found.")
            send_keystrokes = False

def clear_line():
    sys.stdout.write('\x1b[2K\r')

def main():
    print("Python script started. Press keys to send them via serial. Press ESC to quit.")
    keyboard.on_press(on_press)  # Setup the keyboard hook to capture key presses
    listener = mouse.Listener(on_click=on_click)
    listener.start()

    keyboard.wait('esc')  # Wait until ESC is pressed to quit the script
    listener.stop()  # Stop the mouse listener

    # Clean up
    ser.close()
    print("\nSerial connection closed. Exiting script.")

if __name__ == "__main__":
    main()
