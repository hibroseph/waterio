#! /bin/bash
# kill process that is using the serial
pids=`ps axo pid,command | grep SerialReadToFile.py | awk '{print $1}'`

for line in $pids; do
	echo killing process $line
	kill -9 $line
done

# compile and upload new code to arduino
arduino-cli compile --fqbn arduino:avr:uno
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno

# restart serial monitor python script in background disconnected from tty (ssh)
nohup python SerialReadToFile.py </dev/null >/dev/null 2>&1 &
