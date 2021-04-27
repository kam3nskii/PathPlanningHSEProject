import sys
import xml.etree.ElementTree as ET
from tkinter import CURRENT, Canvas, Tk

inputFile = sys.argv[1]

tree = ET.parse(inputFile)
XMLroot = tree.getroot()

width = int(XMLroot.find("map").find("width").text)
height = int(XMLroot.find("map").find("height").text)
startx = int(XMLroot.find("map").find("startx").text)
starty = int(XMLroot.find("map").find("starty").text)
finishx = int(XMLroot.find("map").find("finishx").text)
finishy = int(XMLroot.find("map").find("finishy").text)

fileWithPath = sys.argv[1][:-4] + "_log_LPAStarTMP.xml"

grid = []

for i in range(height):
    grid.append(XMLroot.find("map").find("grid")[i].text.split())


tree2 = ET.parse(fileWithPath)
XMLroot2 = tree2.getroot()
grid2 = []
for i in range(height):
    grid2.append(XMLroot2.find("log").find("path")[i].text.split())
grid2[starty][startx] = '@'
grid2[finishy][finishx] = '#'

cell_size = 24

def printGrid():
    for i in range(height):
        for j in range(width):
            if grid2[j][i] == '@':
                c.create_rectangle(i * cell_size, j * cell_size,
                            i * cell_size + cell_size, j * cell_size + cell_size,
                            fill='green')
            elif grid2[j][i] == '#':
                c.create_rectangle(i * cell_size, j * cell_size,
                            i * cell_size + cell_size, j * cell_size + cell_size,
                            fill='red')
            elif grid2[j][i] == '1':
                c.create_rectangle(i * cell_size, j * cell_size,
                            i * cell_size + cell_size, j * cell_size + cell_size,
                            fill='gray')
            elif grid2[j][i] == '*':
                c.create_rectangle(i * cell_size, j * cell_size,
                            i * cell_size + cell_size, j * cell_size + cell_size,
                            fill='#58F')
            elif grid2[j][i] == '0':
                c.create_rectangle(i * cell_size, j * cell_size,
                            i * cell_size + cell_size, j * cell_size + cell_size,
                            fill='white')

def btn1(event):
    c.itemconfig(CURRENT, fill="gray")
    x = event.y // cell_size
    y = event.x // cell_size
    grid[x][y] = '1'
    grid2[x][y] = grid[x][y]
    XMLroot.find("map").find("grid")[x].text = ' '.join(grid[x])
    tree.write(inputFile)
    print(y, x)

def btn3(event):
    global grid2
    tree3 = ET.parse(fileWithPath)
    XMLroot3 = tree3.getroot()
    for i in range(height):
        grid2[i] = XMLroot3.find("log").find("path")[i].text.split()
    grid2[starty][startx] = '@'
    grid2[finishy][finishx] = '#'
    printGrid()

root = Tk()
root.title("Grid")
c = Canvas(root, width = width * cell_size, height = height * cell_size)
c.bind("<Button-1>", btn1)
c.bind("<Button-3>", btn3)
c.bind("<Button-2>", btn3)
c.pack()
printGrid()
root.mainloop()
