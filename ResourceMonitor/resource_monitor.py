import serial
import serial.tools.list_ports
import psutil 
import time
import pandas as pd
import datetime
# list the ports and choose the best one
ports = serial.tools.list_ports.comports()
port = None
if len(ports) > 1:    
    print("\n".join([f"{x}: {ports[x]}" for x in range(len(ports))]))
    port = ports[int(input("Select a port: "))]
elif len(ports) == 1:
    port = ports[0]
else:
    print("No serial ports found")
    exit()

m5stick = serial.Serial(port.name,115200)
m5stick.read_all() # read all the garbage at the start
cpu = 0
last_read_time = time.time()

event_log = pd.DataFrame({"timestamp":[],"state":[]})
while m5stick.is_open:
    try:
        if m5stick.in_waiting > 0:
            # read the byte
            event_code = m5stick.read()
            event_log.loc[-1] = [datetime.datetime.now().timestamp(),event_code]
            print("Got ", event_code, "from M5 Stick")

        curr_time = time.time()
        if curr_time - last_read_time < 1: continue

        last_read_time = curr_time
        cpu = int(psutil.cpu_percent())
        mem = psutil.virtual_memory()
        mem = int(mem.available / mem.total *100)
        battery = 0
        if hasattr(psutil, "sensors_battery"):
            battery = psutil.sensors_battery()
            if battery:
                battery = int(battery.percent)
        m5stick.write(bytearray([cpu,mem,battery])) 
        
    except Exception as e:
        print(e)
        break



m5stick.close()

event_log.to_csv(f'event_log_{datetime.datetime.now().timestamp()}.csv',index=False)

