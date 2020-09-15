import socket
import threading

HEADER = 64     #First message to server is the length. Communicates number of bytes.
PORT = 5050     #Any Port
SERVER = '192.168.32.32'
ADDR = (SERVER, PORT)
FORMAT = 'utf-8'
DISCONNECT_MESSAGE = "!DISCONNECT"      #Might not need this

#Create a socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#Bind socket to address. Anything that connects to this address will hit this socket.
server.bind(ADDR)

#Handle communication between client and server
def handle_client(conn, addr):
    print("[NEW CONNECTION] "+str(addr)+"  connected.")

    connected = True
    while connected:

        #When receiving info, print it to screen.
        msg_length = conn.recv(HEADER).decode(FORMAT)     #first message contains the length
        if msg_length:  #Message must exist
            msg_length = int(msg_length)
            msg = conn.recv(msg_length).decode(FORMAT)


            #Handle disconnect. Server needs to know.
            if msg == DISCONNECT_MESSAGE:
                connected = False
                conn.send("Connection Terminated by Disconnect Message.")

            print(str(addr) + msg)

            #Message to send to the PC when (a message is received)
            conn.send("Message from RPi: Hi.".encode(FORMAT))

    conn.close()

#Handle new connection
def start():
    server.listen(1)
    print("[LISTENING] Server is listening on "+SERVER)

    while True:
        #Wait for new connection. Store object to send info to, ip address + port
        conn, addr = server.accept()
        #Create thread to handle client
        thread = threading.Thread(target=handle_client, args=(conn, addr))
        thread.start()
        #Total threads - 1 = number of connected threads
        print("[ACTIVE CONNECTIONS] "+str(threading.activeCount()-1))


print("[STARTING] Server is starting...")
start()
