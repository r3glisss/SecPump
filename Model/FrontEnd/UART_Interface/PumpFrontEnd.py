#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
from Tkinter import *
from threading import *
from tkMessageBox import *
from PumpBackEnd import *
import Queue

queue=Queue.Queue(maxsize=5)

class Interface(Tk):

    def __init__(self, parent):
        Tk.__init__(self)

        self.title('Medical Pump Demonstrator')
        self.resizable(width=False, height=False) 
        self.grid()
        self['bg']='white'

        self.drugItem = DrugItem()

        self.cure1Name=''
        self.cure1Debit=0
        self.cure1RestingTime=0

        self.cure2Name=''
        self.cure2Debit=0
        self.cure2RestingTime=0
       
###ROW 0 initialization

        self.Frame00 = Frame(self, borderwidth=5, width=400, height=400, relief=GROOVE)

        self.photo00 = PhotoImage(file="bag.png")

        self.canvas00 = Canvas(self.Frame00,width=400, height=400)
        self.canvas00.create_image(200,200, anchor=CENTER, image=self.photo00)
        self.canvas00['bg']='white'
        self.canvas00.pack()

        self.Frame00.grid(row=0, column=0)

        self.Frame01 = Frame(self, borderwidth=5, width=1000, height=400, relief=GROOVE)
        self.Frame01.grid(row=0, column=1)

        self.canvas01=Canvas(self.Frame01, width=1000, height=400)
        self.canvas01.create_text(500, 200, text='Medical Pump Interface', anchor=CENTER, font="Arial 28 bold")
        self.canvas01.pack()

### ROW 1 initialization

        self.Frame10 = Frame(self, borderwidth=5, width=400, height=400, relief=GROOVE)

        self.photo10 = PhotoImage(file="drop.png")

        self.canvas10 = Canvas(self.Frame10,width=400, height=400)
        self.canvas10.create_image(200, 200, anchor=CENTER, image=self.photo10)
        self.canvas10['bg']='white'
        self.canvas10.pack()

        self.Frame10.grid(row=1, column=0)

        self.Frame11 = Frame(self, borderwidth=5, width=1000, height=400, relief=GROOVE)
        self.Frame11.grid(row=1, column=1)

        #Child grids
        self.Frame11_00 = Frame(self.Frame11, bg='red', width=500, height=200)
        self.Frame11_00.grid(row=0,column=0)

        self.Frame11_01 = Frame(self.Frame11, bg='green', width=500, height=200)
        self.Frame11_01.grid(row=0,column=1)

        self.Frame11_10 = Frame(self.Frame11, bg='blue', width=500, height=200)
        self.Frame11_10.grid(row=1,column=0)

        self.Frame11_11 = Frame(self.Frame11, bg='black', width=500, height=200)
        self.Frame11_11.grid(row=1,column=1)

        self.Frame11_10_00=Frame(self.Frame11_10, bg='yellow', width=250, height=200)
        self.Frame11_10_00.grid(row=0, column=0)

        self.Frame11_10_01=Frame(self.Frame11_10, bg='orange', width=250, height=200)
        self.Frame11_10_01.grid(row=0, column=1)

        self.canvas11_00=Canvas(self.Frame11_00, width=500, height=200)
        self.canvas11_00.create_text(250, 100, text='Cure 1', anchor=CENTER, font="Arial 26 bold")
        self.canvas11_00.pack()

        self.canvas11_01=Canvas(self.Frame11_01, width=500, height=200)
        self.canvas11_01.create_text(250, 100, text='RUNNING', anchor=CENTER, font="Arial 26 bold")
        self.canvas11_01.pack()

        self.canvas11_10_00=Canvas(self.Frame11_10_00, width=250, height=200)
        self.canvas11_10_00.create_text(125, 100, text='Cure name\nDebit\nTime\n', anchor=CENTER, font="Arial 14")
        self.canvas11_10_00.pack()

        self.canvas11_10_01=Canvas(self.Frame11_10_01, width=250, height=200)
        self.id1 = self.canvas11_10_01.create_text(125, 80, text=self.cure1Name + '\n' + str(self.cure1Debit) + '\n'  + str(self.cure1RestingTime)
, anchor=CENTER, font="Arial 14")
        self.canvas11_10_01.pack()


        self.bouton11_11=Button(self.Frame11_11, text="Program", width=16, height=3, command=self.buttonCallBack)
        self.bouton11_11.grid(row=1, column=1)


##Row 2 initialisation

        self.Frame20 = Frame(self, borderwidth=5, width=400, height=400, relief=GROOVE)
        self.Frame20.grid(row=2, column=0)

        self.photo20 = PhotoImage(file="drop.png")

        self.canvas20 = Canvas(self.Frame20,width=400, height=400)
        self.canvas20.create_image(200, 200, anchor=CENTER, image=self.photo20)
        self.canvas20['bg']='white'
        self.canvas20.pack()

        self.Frame21 = Frame(self, borderwidth=5, width=1000, height=400, relief=GROOVE)
        self.Frame21.grid(row=2, column=1)

        self.Frame21_00 = Frame(self.Frame21, bg='red', width=500, height=200)
        self.Frame21_00.grid(row=0,column=0)

        self.Frame21_01 = Frame(self.Frame21, bg='green', width=500, height=200)
        self.Frame21_01.grid(row=0,column=1)

        self.Frame21_10 = Frame(self.Frame21, bg='blue', width=500, height=200)
        self.Frame21_10.grid(row=1,column=0)

        self.Frame21_11 = Frame(self.Frame21, bg='black', width=500, height=200)
        self.Frame21_11.grid(row=1,column=1)

        self.Frame21_10_00=Frame(self.Frame21_10, bg='yellow', width=250, height=200)
        self.Frame21_10_00.grid(row=0, column=0)

        self.Frame21_10_01=Frame(self.Frame21_10, bg='orange', width=250, height=200)
        self.Frame21_10_01.grid(row=0, column=1)

        self.canvas21_00=Canvas(self.Frame21_00, width=500, height=200)
        self.canvas21_00.create_text(250, 100, text='Cure 2', anchor=CENTER, font="Arial 26 bold")
        self.canvas21_00.pack()

        self.canvas21_01=Canvas(self.Frame21_01, width=500, height=200)
        self.canvas21_01.create_text(250, 100, text='STOPPED', anchor=CENTER, font="Arial 26 bold")
        self.canvas21_01.pack()

        self.canvas21_10_00=Canvas(self.Frame21_10_00, width=250, height=200)
        self.canvas21_10_00.create_text(125, 100, text='Cure name\nDebit\nTime\n', anchor=CENTER, font="Arial 14")
        self.canvas21_10_00.pack()

        self.canvas21_10_01=Canvas(self.Frame21_10_01, width=250, height=200)
        self.id2 = self.canvas21_10_01.create_text(125, 80, text=self.cure2Name + '\n' + str(self.cure2Debit) + '\n'  + str(self.cure2RestingTime)
, anchor=CENTER, font="Arial 14")
        self.canvas21_10_01.pack()

        self.bouton21_11=Button(self.Frame21_11, text="Program", width=16, height=3, command=self.buttonCallBack)
        self.bouton21_11.grid(row=1, column=1)

            
    def updateCure(self, drugItem): 
        self.drugItem = drugItem
        #print "In update"
        print self.drugItem.name
        if self.drugItem.id == 1:
            self.canvas11_10_01.itemconfigure(self.id1, text=self.drugItem.name + '\n' + str(self.drugItem.debit) + '\n' + str(self.drugItem.restingtime)) 
        
        if self.drugItem.id == 2:
            self.canvas21_10_01.itemconfigure(self.id2, text=self.drugItem.name + '\n' + str(self.drugItem.debit) + '\n' + str(self.drugItem.restingtime)) 
    

    def buttonCallBack(self):
        childwin=Toplevel(self)
       
        nameVar = StringVar()
        hourVar = StringVar()
        minutesVar = StringVar()
        volumeVar = StringVar()

 
        label00 = Label(childwin, text="Name", width=10, height=2)
        labelentry01 = Entry(childwin, textvariable=nameVar, width=10)

         
        label10 = Label(childwin, text="Hours", width=10, height=2)
        labelentry11 = Entry(childwin, textvariable=hourVar, width=10)

         
        label20 = Label(childwin, text="Minutes", width=10, height=2)
        labelentry21 = Entry(childwin, textvariable=minutesVar, width=10)

    
        label30 = Label(childwin, text="Volume", width=10, height=2)
        labelentry31 = Entry(childwin, textvariable=volumeVar, width=10)


        label00.grid(column=0, row=0)
        labelentry01.grid(column=1, row=0)

        label10.grid(column=0, row=1)
        labelentry11.grid(column=1, row=1)

        label20.grid(column=0, row=2)
        labelentry21.grid(column=1, row=2)

        label30.grid(column=0, row=3)
        labelentry31.grid(column=1, row=3)

        buttonNew = Button(childwin, text='New', command= lambda: self.createNewCure(str(nameVar.get()), int(hourVar.get()), int(minutesVar.get()), int(volumeVar.get())))
        buttonNew.grid(row=0, column=2)
        
        buttonModify = Button(childwin, text='Modify', command= lambda: self.modifyCure(str(nameVar.get()), int(hourVar.get()), int(minutesVar.get()), int(volumeVar.get())))
        buttonModify.grid(row=1, column=2)        

        buttonDelete = Button(childwin, text='Delete', command= lambda: self.deleteCure(str(nameVar.get())))
        buttonDelete.grid(row=2, column=2)

        buttonExit = Button(childwin, text='Exit', command=childwin.destroy)
        buttonExit.grid(row=3, column=2)

    def createNewCure(self, name, hours, minutes, volume):
        global queue
        drugToInject = DrugItem()
        drugToInject.name  = name
        drugToInject.hours = hours
        drugToInject.minutes = minutes
        drugToInject.volume = volume
        drugToInject.action = 1
        
        try :
            queue.put_nowait(drugToInject)
        except Queue.Full:
            print "[*] Queue Full\n"
            
    
    def modifyCure(self, name, hours, minutes, volume):
        global queue
        drugToInject = DrugItem()
        drugToInject.name = name
        drugToInject.hours = hours
        drugToInject.minutes = minutes
        drugToInject.volume = volume
        drugToInject.action = 2        

        try :
            queue.put_nowait(drugToInject)
        except Queue.Full:
            print "[*] Queue Full\n"
 
    def deleteCure(self, name):
        global queue
        drugToInject = DrugItem()
        drugToInject.name = name
        drugToInject.action = 0        

        try :
            queue.put_nowait(drugToInject)
        except Queue.Full:
            print "[*] Queue Full\n"
 

if __name__ == "__main__":

    interface = Interface(None)

    t1 = threading.Thread(target=updaterCure, args=[interface])
    t2 = threading.Thread(target=uartWriter, args=[queue]) 

    t1.start()
    t2.start()

    interface.mainloop()

    t1.join()
    t2.join()
