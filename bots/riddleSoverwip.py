import socket
import random
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

def e():
    sendMsg("#")

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
def checkHappy():
    countHappy = 0
    countAll = 0
    state = get("list")
    state = state.splitlines()
    for s in state:
        countAll += 1
        words = s.split()
        if len(words) == 10:
            pile1 = words[3].split('/')[0]
            pile2 = words[5].split('/')[0]
            if pile1 == pile2:
                countHappy += 1
    if countAll == countHappy:
        return True
    return False

r()
i()

pileCount = [52]
maxTurns = 1000
turnCount = 0
happy = False
maxPiles = 5
attempts = 0
maxAttempts = 20
commands = ["move", "flip"]
while attempts < maxAttempts:
    #print(attempts + 1)
    while turnCount < maxTurns and not happy:
        command = random.choice(commands)
        if command == "flip":
            command += " " + str(random.randint(0,len(pileCount) - 1))
            print command
        elif command == "move":
            fromPile = random.randint(0, len(pileCount) - 1)
            toPile = len(pileCount)
            if random.randint(0,2) == 0 and len(pileCount) is not 1:
                toPile = fromPile
                while toPile == fromPile:
                    toPile = random.randint(0, len(pileCount) - 1)
            num = random.randint(0, pileCount[fromPile])
            command += " " + str(fromPile) + " " + str(toPile) + " " + str(num)
            print command
            pileCount[fromPile] -= num
            if toPile < len(pileCount):
                pileCount[toPile] += num
            else:
                pileCount.append(num)
            if pileCount[fromPile] is 0:
                pileCount.remove(fromPile)

        g(command)
        turnCount += 1
        happy = checkHappy()
    get("list")
    get("reset")
    attempts += 1

e()
s.close()
exit()
