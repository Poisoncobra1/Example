import serial               #import serial pacakge
from time import sleep
import webbrowser           #import package for opening link in browser
import sys                  #import system package

def GPS_Info():
    global NMEA_buff
    global lat_in_degrees
    global long_in_degrees
    nmea_time = []
    nmea_latitude = []
    nmea_longitude = []
    nmea_time = NMEA_buff[0]                    #extract time from GPGGA string
    nmea_latitude = NMEA_buff[1]                #extract latitude from GPGGA string
    nmea_longitude = NMEA_buff[3]#extract longitude from GPGGA string

    nmea_bearing = NMEA_buff[8]
    
    print("NMEA Time: ", nmea_time,'\n')
    print ("NMEA Latitude:", nmea_latitude,"NMEA Longitude:", nmea_longitude,'\n', "bearing", nmea_bearing)
    
    lat = float(nmea_latitude)                  #convert string into float for calculation
    longi = float(nmea_longitude)               #convertr string into float for calculation
    
    lat_in_degrees = convert_to_degrees(lat)    #get latitude in degree decimal format
    long_in_degrees = convert_to_degrees(longi)#get longitude in degree decimal format
    

    data = open("file1", "w+")
    date.write(str(nmea_latitide) + '\n' + str(nema_longitude) + '\n' + str(nema_bearing))
    
    data.close()
    time.sleep(10)

#convert raw NMEA string into degree decimal format   
def convert_to_degrees(raw_value):
    decimal_value = raw_value/100.00
    degrees = int(decimal_value)
    mm_mmmm = (decimal_value - int(decimal_value))/0.6
    position = degrees + mm_mmmm
    position = "%.4f" %(position)
    return position
    


gpgga_info = "$GPGGA,"
ser = serial.Serial ("/dev/ttyS0")              #Open port with baud rate
GPGGA_buffer = 0
NMEA_buff = 0
lat_in_degrees = 0
long_in_degrees = 0

while True:
    try:
        received_data = (str)(ser.readline())                   #read NMEA string received
        GPGGA_data_available = received_data.find(gpgga_info)#check for NMEA GPGGA string
        GPGGA_buffer = received_data.split("$GPGGA,",1)[1]  #store data coming after "$GPGGA," string 
        NMEA_buff = (GPGGA_buffer.split(','))               #store comma separated data in buffer
        GPS_Info()                                          #get time, latitude, longitude
        print("lat in degrees:", lat_in_degrees," long in degree: ", long_in_degrees, '\n')
        map_link = 'http://maps.google.com/?q=' + lat_in_degrees + ',' + long_in_degrees    #create link to plot location on Google map
        print("<<<<<<<<press ctrl+c to plot location on google maps>>>>>>\n")               #press ctrl+c to plot on map and exit 
        print("------------------------------------------------------------\n")
        time.sleep(10)
    except:
        print("tt")
        pass
          
#except KeyboardInterrupt:
 #   webbrowser.open(map_link)        #open current position information in google map
  #  sys.exit(0)
