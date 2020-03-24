import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)

GPIO.setup(17,GPIO.OUT)

try:
    while True:
        GPIO.output(17,1)
        time.sleep(0.0015)
        GPIO.output(17,0)

        time.sleep(0.1)

except KeyboardInterrupt:
    GPIO.cleanup()
