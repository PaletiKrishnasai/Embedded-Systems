# -*- coding: utf-8 -*-
"""
Created on Fri Mar 19 01:39:05 2021

@author: Paleti Krishnasai
"""


from tkinter import *
import tkinter
from matplotlib.figure import Figure 
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk) 
from PIL import ImageTk, Image
import serial
from time import sleep
ser = serial.Serial ("COM3", 9600)    #Open port with baud rate


def red():
    ser.write(bytes(b'r\r'))
def green():
    ser.write(bytes(b'g\r'))
def blue():
    ser.write(bytes(b'b\r'))
def white():
    ser.write(bytes(b'w\r'))
def begWrite():
    ser.write(bytes(b'v\r'))

tkWindow = Tk()
frame = Frame(tkWindow)
frame.pack()
tkWindow.geometry('400x150')  
bottomframe = Frame(tkWindow)
bottomframe.pack( side = BOTTOM )


redbutton = Button(tkWindow,text = 'RED',command = red)  
redbutton.pack(side = BOTTOM )  

bluebutton = Button(tkWindow,text = 'BLUE',command = blue)  
bluebutton.pack(side = BOTTOM )  

whitebutton = Button(tkWindow,text = 'WHITE',command = white)  
whitebutton.pack(side = BOTTOM )  

greenbutton = Button(tkWindow,text = 'GREEN',command = green)  
greenbutton.pack(side = BOTTOM )  

graphbutton = Button(tkWindow,text = 'GRAPH' )  
graphbutton.pack(side = BOTTOM )  
 
tkWindow.mainloop()

'''
print("Red")
red()
sleep(2)

print("Blue")
blue()
sleep(2)

print("Green")
green()
sleep(2)

print("White")
white()
sleep(2)

print("Begin Serial read:")
begWrite()

while True:
    line = ser.readline().decode("utf-8")
    print(line)
'''
