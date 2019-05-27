#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
#import PumpFrontEnd
import Queue
import threading
import time 
import serial

port="/dev/ttyACM0"
baud=115200
ser = serial.Serial(port, baud, timeout=0)

class DrugItem():

    def __init__(self):
        self.name=''
        self.hours=0
        self.minutes=0
        self.volume=0
        self.debit=0
        self.restingtime=0
        self.state='STOPPED'
        self.id=0  
        self.action=0


def concatSend(action, name, hours, minutes, volume):
    bufSend = ''
    tempBuf = ''
    global ser

    if action == 1:
        bufSend += "0001"
        tempBuf = name
        if len(tempBuf) < 20:
            bufSend += "0" * (20 - len(tempBuf))
        bufSend += name        

        tempBuf = str(hours)
        if len(tempBuf) < 4:
            bufSend += "0" * (4-len(tempBuf))
        bufSend += str(hours)
        
        tempBuf = str(minutes)
        if len(tempBuf) < 4:
            bufSend += "0" * (4-len(tempBuf))
        bufSend += str(minutes)

        tempBuf = str(volume)
        if len(tempBuf) < 4:
            bufSend += "0" * (4-len(tempBuf))
        bufSend += str(volume)

    if action == 2:
        bufSend += "0002"
        tempBuf = name
        if len(tempBuf) < 20:
            bufSend += "0" * (20 - len(tempBuf))
        bufSend += name        

        tempBuf = str(hours)
        if len(tempBuf) < 4:
            bufSend += "0" * (4-len(tempBuf))
        bufSend += str(hours)
        
        tempBuf = str(minutes)
        if len(tempBuf) < 4:
            bufSend += "0" * (4-len(tempBuf))
        bufSend += str(minutes)

        tempBuf = str(volume)
        if len(tempBuf) < 4:
            bufSend += "0" * (4-len(tempBuf))
        bufSend += str(volume)

    if action == 0:
        bufSend += "0000"
        tempBuf = name
        if len(tempBuf) < 20:
            bufSend += "0" * (20 - len(tempBuf))
        bufSend += name        

        tempBuf = str(hours)
        if len(tempBuf) < 4:
            bufSend += "0" * (4-len(tempBuf))
        bufSend += str(hours)
        
        tempBuf = str(minutes)
        if len(tempBuf) < 4:
            bufSend += "0" * (4-len(tempBuf))
        bufSend += str(minutes)

        tempBuf = str(volume)
        if len(tempBuf) < 4:
            bufSend += "0" * (4-len(tempBuf))
        bufSend += str(volume)
 
    print '[*] Concat Send Function'
    print bufSend
    print len(bufSend)
    print '[*] Send to Pump'
    ser.write(bufSend)

def updaterCure(interface):
    timer=100

    drugItem = DrugItem()
    drugItem.name="A"
    drugItem.debit=1000
    drugItem.id=1

    while 1:
        try:
            timer-=1
            interface.updateCure(drugItem)
            trame = ser.read(32)
            print trame
            drugItem.debit +=1
            try:
                drugItem.id = int(str(trame[0:4]))
            except ValueError:
                pass

            drugItem.name = str(trame[4:24]).replace("0","")

            try:
                drugItem.restingtime =  int(str(trame[24:32]))
            except ValueError:
                pass

        finally:    
            time.sleep(1)
            if timer == 0:
                break

def uartWriter(queue):
    drugItem = DrugItem()
    for i in range(0 ,100):
        try:
            drugItem = queue.get()
            print drugItem.name
            print str(drugItem.hours)
            print str(drugItem.minutes)        
            print str(drugItem.volume)
            concatSend(drugItem.action, drugItem.name, drugItem.hours, drugItem.minutes, drugItem.volume)        
        except Queue.Empty:
            print '[*] Waiting for the queue\n'
            time.sleep(1)
