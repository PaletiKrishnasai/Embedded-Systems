from tkinter import *
import tkinter
import time
import matplotlib.pyplot as plt
from PIL import ImageTk, Image
import serial
from time import sleep
from stream_engine.stream import Stream, StreamAnimation


ser = serial.Serial ("/dev/ttyACM1", 9600)    #Open port with baud rate

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

y = []

def plot():
    window.destroy()
    begWrite()

    #global x,y
    plt.ion()
    fig,ax = plt.subplots()
    ax.set_ylim(0,4)
  

    while True:
        line = ser.readline().decode("utf-8")
        num = 0.0
        try:
            num = float(line)
        except ValueError:
            num = 0.0

        ax.set_xlim(int(len(y)/20)*20,int(len(y)/20)*20+20)
        y.append(num)
        ax.plot(y,c='b')
        fig.canvas.draw()
        fig.canvas.flush_events()

        print(num)

window = Tk() 

# setting the title 
window.title('CED18D039-EMBEDDED SYSTEMS_LAB7')   


redbutton = Button(window,text = 'RED',command = red)  
redbutton.place(x=200,y=100) 

bluebutton = Button(window,text = 'BLUE',command = blue)  
bluebutton.place(x=300,y=100) 

whitebutton = Button(window,text = 'WHITE',command = white)  
whitebutton.place(x=400,y=100)  

greenbutton = Button(window,text = 'GREEN',command = green)  
greenbutton.place(x=500,y=100) 
  
# dimensions of the main window 
window.geometry("700x800") 

# # button that displays the plot 
plot_button = Button(master = window, command = plot, height = 2, width = 10, text = "Plot") 
plot_button.place(x=300,y=200) 

# run the gui 
window.mainloop() 
