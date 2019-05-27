#!/usr/bin/python
import struct
import sys
import signal
import time
import threading
import socket
from bluepy.btle import Scanner, DefaultDelegate, Peripheral

class ScanDelegate(DefaultDelegate):

    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleDiscovery(self, dev, isNewDev, isNewData):
        if isNewDev:
            print "[*] Discovered device", dev.addr
        elif isNewData:
            print "[*] Received new data from", dev.addr


def FindBLEDevice(devices):
    for dev in devices:
        print "Device %s (%s), RSSI=%d dB" % (dev.addr, dev.addrType, dev.rssi)
	for (adtype, desc, value) in dev.getScanData():
            print "  %s = %s" % (desc, value)
            if "VSPM" in value:
                print "[+] Found Medical Pump" 
                return dev

def signal_handler(sig, frame):
        print('[!] Exiting the Program')
        sys.exit(0)


def ConnectDevice(device):
    print "[*] Connecting to Device..."
    dev = Peripheral(device.addr)
    return dev


def DisconnectDevice(device):
    print "[*] Disconnecting to Device..."
    device.disconnect()


def DisplayServices(device):
    print "[*] Discovering Services..."
    for svc in device.services:
        print svc


def GetPumpService(device, name):
    if name == "Command":
        print "[*] Accessing Command Service"
        return device.getServiceByUUID("fcb43012-cbc4-11e8-a8d5-f2801f1b9fd1") 
    if name == "Check":
        print "[*] Accessing Check Service"
        return device.getServiceByUUID("fcb43abc-cbc4-11e8-a8d5-f2801f1b9fd1")
             

def GetPumpCharacteristics(service, name):
    if service.uuid == "fcb43012-cbc4-11e8-a8d5-f2801f1b9fd1":
        for serv in service.getCharacteristics():
            if name == "CREATE":
                if serv.uuid == "fcb432ec-cbc4-11e8-a8d5-f2801f1b9fd1":
                    print "[*] Getting CREATE characteristic"
                    return serv
            if name == "MODIFY":
                if serv.uuid == "fcb4344a-cbc4-11e8-a8d5-f2801f1b9fd1":
                    print "[*] Getting MODIFY characteristic"
                    return serv
            if name == "DELETE":
                if serv.uuid == "fcb4358a-cbc4-11e8-a8d5-f2801f1b9fd1":
                    print "[*] Getting DELETE characteristic"
                    return serv

    if service.uuid == "fcb43abc-cbc4-11e8-a8d5-f2801f1b9fd1":
        for serv in service.getCharacteristics():
            if name == "NAME":
                if serv.uuid == "fcb43bde-cbc4-11e8-a8d5-f2801f1b9fd1":
                    print "[*] Getting NAME characteristic"
                    return serv
            if name == "VOLUME":
                if serv.uuid == "fcb43d0a-cbc4-11e8-a8d5-f2801f1b9fd1":
                    print "[*] Getting VOLUME characteristic"
                    return serv    
            if name == "DURATION":
                if serv.uuid == "fcb4417e-cbc4-11e8-a8d5-f2801f1b9fd1":
                    print "[*] Getting DURATION characteristic"
                    return serv


def WriteCharacteristic(charac, toWrite):
    charac.write(toWrite, True)

def CraftPacket(name, volume, duration):
    packet = name
    packet += '\x00'
    packet += volume
    packet += '\x00'
    packet += duration
    print "[*] Packet Crafted ", packet
    return packet


def BLEReader(nameChar, volChar, durChar):

    for i in xrange(0,1000):

        sys.stdout.write("[x] "+ nameChar.read() + " Volume " + volChar.read() + " Resting Duration " + durChar.read())
        sys.stdout.write("\r")
        sys.stdout.flush()

        sys.stdout.write("[+] "+ nameChar.read() + " Volume " + volChar.read() + " Resting Duration " + durChar.read())
        sys.stdout.write("\r")
        sys.stdout.flush()


def BLEWriter(createChar, modifyChar, deleteChar):
    name = raw_input("Cure Name >")
    volume = raw_input("Cure Volume >")
    duration = raw_input("Cure Duration >")

    packet = CraftPacket(name, volume, duration)
    WriteCharacteristic(createChar, packet)    


def main(argv):
   
    print "[*] Scanning For Device"    
    signal.signal(signal.SIGINT, signal_handler)

    scanner = Scanner().withDelegate(ScanDelegate())
    devices = scanner.scan(10.0)
        
    device = FindBLEDevice(devices)
    deviceConnect = ConnectDevice(device)
    DisplayServices(deviceConnect)

    """ Getting Characteristics from Command service"""
    service = GetPumpService(deviceConnect, "Command")
    createChar = GetPumpCharacteristics(service, "CREATE")
    modifyChar = GetPumpCharacteristics(service, "MODIFIY")
    deleteChar = GetPumpCharacteristics(service, "DELETE")
    
    """Getting Characteristics from Check service"""
    service = GetPumpService(deviceConnect, "Check")
    nameChar = GetPumpCharacteristics(service, "NAME")
    volChar  = GetPumpCharacteristics(service, "VOLUME")
    durChar  = GetPumpCharacteristics(service, "DURATION")

    BLEWriter(createChar, modifyChar, deleteChar)
    BLEReader(nameChar, volChar, durChar)

    DisconnectDevice(deviceConnect)

if __name__ == '__main__':
	sys.exit(main(sys.argv))

