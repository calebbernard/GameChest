import socket
TCP_IP = '127.0.0.1'
TCP_PORT = 5555
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

def sendMsg(m):
    l = str(len(m))
    while len(l) < 5:
        l = "0" + l;
    s.send(l + m)

def recvMsg():
    msg = ""
    headerSize = 5
    size = int(s.recv(headerSize))
    if size > 0:
        msg = s.recv(size)
    if msg == "#":
        s.close()
        print "Server disconnected."
        exit()
    else:
        return msg


def r():
    print recvMsg()

def get(m): # Sends a message and returns the reply
    sendMsg(m)
    return recvMsg()

def g(m): # Sends a message, prints the reply, and then returns it
    reply = get(m)
    print reply
    return reply

def d():
    x = raw_input("> ")
    if x == "#auto":
        return True
    if x == "#":
        sendMsg(x)
        s.close()
        exit()
    g(x)
    return False

def i():
    finished = False
    while not finished:
        finished = d()

# client code goes here
r()
i()

s.close()
exit()