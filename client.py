import socket
import random
TCP_IP = '127.0.0.1'
TCP_PORT = 5555
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
inputReady = "#Ready for input."

def specialInput(m):
    if m[0] == "#":
        return True
    return False

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

def e():
    sendMsg("#")

def r():
    reply = ""
    fullReply = ""
    receiving = True
    while receiving:
        reply = recvMsg()
        if not specialInput(reply):
            fullReply += reply
        else:
            if reply == inputReady:
                receiving = False
    return fullReply

def get(m): # Sends a message and returns the reply
    sendMsg(m)
    reply = r()
    return reply

def g(m): # Sends a message, prints the reply, and then returns it
    reply = get(m)
    if reply != "":
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
print r()
g("Caleb")
i()


e()
s.close()
exit()
