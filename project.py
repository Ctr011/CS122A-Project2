import RPi.GPIO as GPIO
import serial
import time
GPIO.setmode(GPIO.BCM)
GPIO.setup(5,GPIO.OUT) #Red LED
GPIO.setup(6,GPIO.OUT) #Green LED
#GPIO.setup(18,GPIO.OUT) #Transmitter1
GPIO.setup(24,GPIO.IN) #Reciever1 

class Sys:
    initial = 1
    red = 2
    green = 4

ser = serial.Serial(
    port='/dev/ttyAMA0',
    baudrate = 9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)
current = Sys.initial
count = 0 
wait = 5

# GPIO.PWM([pin], [frequency])
# read data GPIO.input([pin])
try:
    while True:
        if(current == Sys.initial):
            current = Sys.red
            print("Initializing...");
          #  GPIO.output(18, True)
           # pwm = GPIO.PWM(18, 38000)
           # pwm.start(50)
        elif(current == Sys.red):
            print("Red State")
            GPIO.output(5,True)
            GPIO.output(6,False)
            ser.write(str.encode('y'))
            time.sleep(1)
            if(GPIO.input(24) == 1):
                count = count + 1
                print("Count: ")
                print(count)
            if(count >= 10):
                wait = 20
                current = Sys.green
        elif(current == Sys.green):
            print("Green State")
            GPIO.output(5,False)
            GPIO.output(6,True)
            ser.write(str.encode('n'))
            wait = wait - 1
            time.sleep(600/1000)
            if(wait <= 0):
                count = 1
                current = Sys.red
        
except KeyboardInterrupt: #CTRL+C
    GPIO.cleanup()
