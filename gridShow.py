import xml.etree.ElementTree as ET
from tkinter import Tk
from tkinter import Canvas
from tkinter import CURRENT
import sys
tree = ET.parse(sys.argv[1])
XMLroot = tree.getroot()

width = int(XMLroot.find("map").find("width").text)
height = int(XMLroot.find("map").find("height").text)
startx = int(XMLroot.find("map").find("startx").text)
starty = int(XMLroot.find("map").find("starty").text)
finishx = int(XMLroot.find("map").find("finishx").text)
finishy = int(XMLroot.find("map").find("finishy").text)

grid = []
for i in range(height):
    grid.append(XMLroot.find("log").find("path")[i].text.split())

grid[starty][startx] = '@'
grid[finishy][finishx] = '#'

cell_size = 24

root = Tk()
root.title(sys.argv[1])
c = Canvas(root, width = width * cell_size, height = height * cell_size)
c.pack()
for i in range(height):
    for j in range(width):
        if grid[j][i] == '@':
            c.create_rectangle(i * cell_size, j * cell_size,
                           i * cell_size + cell_size, j * cell_size + cell_size,
                           fill='green')
        elif grid[j][i] == '#':
            c.create_rectangle(i * cell_size, j * cell_size,
                           i * cell_size + cell_size, j * cell_size + cell_size,
                           fill='red')
        elif grid[j][i] == '*':
            c.create_rectangle(i * cell_size, j * cell_size,
                           i * cell_size + cell_size, j * cell_size + cell_size,
                           fill='#58F')
        elif grid[j][i] == '1':
            c.create_rectangle(i * cell_size, j * cell_size,
                           i * cell_size + cell_size, j * cell_size + cell_size,
                           fill='gray')
        elif grid[j][i] == '0':
            c.create_rectangle(i * cell_size, j * cell_size,
                           i * cell_size + cell_size, j * cell_size + cell_size,
                           fill='white')
        elif grid[j][i] == '2':
            c.create_rectangle(i * cell_size, j * cell_size,
                           i * cell_size + cell_size, j * cell_size + cell_size,
                           fill='yellow')
        elif grid[j][i] == '3':
            c.create_rectangle(i * cell_size, j * cell_size,
                           i * cell_size + cell_size, j * cell_size + cell_size,
                           fill='yellow')
        elif grid[j][i] == '4':
            c.create_rectangle(i * cell_size, j * cell_size,
                           i * cell_size + cell_size, j * cell_size + cell_size,
                           fill='pink')
root.mainloop()
