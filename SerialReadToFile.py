import serial,time,datetime,os

log_filename = "waterio.log"
log_directory = "/home/steve/Repos/waterio"

def attempt_to_connect_to_serial():
    print("Attempting to connect to serial") 
    
    with serial.Serial("/dev/ttyACM0", 9600, timeout=1) as arduino:
        time.sleep(0.1)

        if arduino.isOpen():
            print("{} connected!".format(arduino.port))

            try:
                while True:
                    print("waitingggggg")
                    while arduino.inWaiting()==0: pass
                    
                    if arduino.inWaiting()>0:
                        print("there is something in waiting")
                        message=str(arduino.readline())

                        print(message)
                        if "[FILE]" in message:
                            print("adding {} to log file".format(message))
                            with open( "{}/{}".format(log_directory, log_filename), 'a') as logfile:
                                logfile.write("{}:{}\n".format(datetime.datetime.now(), message.split('[FILE]:')[1]))

                        print("flushing input");
                        arduino.flushInput();
                    print("finishing waiting loop")
            except KeyboardInterrupt:
                print("Uh oh, someone interupted with keyboard")

        print("i dont think we should ever hit this")

if __name__ == '__main__':
    print('Running SerialToFile program for waterio')
    while(True):
        try:
            attempt_to_connect_to_serial()
        except KeyboardInterrupt:
            print("Someone killed from the keyboard");
            break
        except Exception as e:
            print("There was an issue but we will keep waiting to connect to serial")
            print(e)
            time.sleep(15);

    print('Ending SerialToFile program for waterio')


