import serial,time,datetime

if __name__ == '__main__':
    print('Running SerialToFile program for waterio');
    with serial.Serial("/dev/ttyACM0", 9600, timeout=1) as arduino:
        time.sleep(0.1)

        if arduino.isOpen():
            print("{} connected!".format(arduino.port))

            try:
                while True:
                    while arduino.inWaiting()==0: pass
                    if arduino.inWaiting()>0:
                        message=str(arduino.readline())

                        if "[FILE]" in message:
                            print("adding {} to log file".format(message))
                            with open("waterio.log", 'a') as logfile:
                                logfile.write("{}:{}\n".format(datetime.datetime.now(), message.split(':')[1]))

                        arduino.flushInput();
            except KeyboardInterrupt:
                print("Uh oh, someone interupted with keyboard")
