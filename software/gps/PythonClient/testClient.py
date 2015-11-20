"""
2.12 GPS Client Test Code
MIT 2.12 Intro To Robotics 2015
Daniel J. Gonzalez - dgonz@mit.edu
Steven Keyes - srkeyes@mit.edu

This is an example of how to access the GPS server used in 2.12. The format of
the data is [x, y, phi] where x and y are coordinates in meters and phi is
orientation in radians. The origin is in the center of the field, and x is
along the longer dimension of the field.
"""

import sys,socket,time,struct,signal

def signal_handler(signal, frame):
    global s1
    print('Closing Connection...')
    s1.close()
    sys.exit(0)

def getVals(sObj):
    sObj.send('r')
    raw = sObj.recv(16)
    x = struct.unpack('>f',raw[0:4])[0]
    y = struct.unpack('>f',raw[4:8])[0]
    phi = struct.unpack('>f',raw[8:12])[0]
    timestamp = struct.unpack('>f',raw[12:16])[0]
    return (x, y, phi, timestamp)
    
########  Set up TCP/IP Connection ####
#serverIP = '192.168.1.212'  #Use with the 2.12 Servers
serverIP = 'localhost'      #Use for loopback testing on your own computer
s1=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
try:
    s1.connect((serverIP,2121))
    print 'Connecting...'
except socket.error:
    print('connection error')
    sys.exit(0)

########  Run your main loop code here. Here I'm just looping 100 times.  ####
tic = time.time() #For timing purposes

N = 500
for n in range(0,N):
    (x, y, phi, timestamp) = getVals(s1)

toc = time.time()-tic #For timing purposes
print 'average time per read: ',str(toc/N) #Avg time per read: 

########  Close the connection when you end the program.  ####
print('Closing Connection...')
s1.close()
print (x, y, phi, timestamp)
#Print latest result to prove that we have the proper
# For example: (1000, 2000, 3000, 4000, 5000, 6000, 88.26200103759766)
