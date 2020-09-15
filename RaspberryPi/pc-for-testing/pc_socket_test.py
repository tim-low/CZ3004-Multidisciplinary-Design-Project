import socket
import threading

HEADER = 64 #First message to server is always length 64. Communicates number of bytes.
PORT = 5050
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"
SERVER = '192.168.32.32'
ADDR = (SERVER, PORT)

#Create a socket
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#Bind socket to server socket
client.connect(ADDR)

def send(msg):
    message = msg.encode(FORMAT)    #encode string into bytes
#    msg_length = len(message)       #get message length
#    send_length = str(msg_length).encode(FORMAT)
#    send_length += b' ' * (HEADER - len(send_length))  #pad length of string to Header size.
#    client.send(send_length)        #send length of message
    client.send(message)

    #print(client.recv(2048).decode(FORMAT))       #just a large number TODO: Implement the same header + length

send("PC connection established.")
while(1):
#    send("Hello")
    send(input("Enter a message: "))


