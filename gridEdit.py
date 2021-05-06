import sys
import xml.etree.ElementTree as ET
from tkinter import CURRENT, Canvas, Tk

inputFile = sys.argv[1]

inputTree = ET.parse(inputFile)
XMLroot = inputTree.getroot()

width = int(XMLroot.find("map").find("width").text)
height = int(XMLroot.find("map").find("height").text)
startx = int(XMLroot.find("map").find("startx").text)
starty = int(XMLroot.find("map").find("starty").text)
finishx = int(XMLroot.find("map").find("finishx").text)
finishy = int(XMLroot.find("map").find("finishy").text)

inputGrid = []
for i in range(height):
    inputGrid.append(XMLroot.find("map").find("grid")[i].text.split())

fileWithPath = sys.argv[1][:-4] + "_log_LPAStarTMP.xml"
XMLrootPath = ET.parse(fileWithPath).getroot()
outputGrid = []
for i in range(height):
    outputGrid.append(XMLrootPath.find("log").find("path")[i].text.split())

outputGrid[starty][startx] = '@'
outputGrid[finishy][finishx] = '#'

cell_size = 24

def printGrid():
    for i in range(height):
        for j in range(width):
            if outputGrid[j][i] == '@':
                c.create_rectangle(i * cell_size, j * cell_size,
                            i * cell_size + cell_size, j * cell_size + cell_size,
                            fill='green')
            elif outputGrid[j][i] == '#':
                c.create_rectangle(i * cell_size, j * cell_size,
                            i * cell_size + cell_size, j * cell_size + cell_size,
                            fill='red')
            elif outputGrid[j][i] == '1':
                c.create_rectangle(i * cell_size, j * cell_size,
                            i * cell_size + cell_size, j * cell_size + cell_size,
                            fill='gray')
            elif outputGrid[j][i] == '*':
                c.create_rectangle(i * cell_size, j * cell_size,
                            i * cell_size + cell_size, j * cell_size + cell_size,
                            fill='#58F')
            elif outputGrid[j][i] == '0':
                c.create_rectangle(i * cell_size, j * cell_size,
                            i * cell_size + cell_size, j * cell_size + cell_size,
                            fill='white')

def btn1(event):
    c.itemconfig(CURRENT, fill="gray")
    x = event.y // cell_size
    y = event.x // cell_size
    inputGrid[x][y] = '1'
    outputGrid[x][y] = inputGrid[x][y]
    XMLroot.find("map").find("grid")[x].text = ' '.join(inputGrid[x])
    inputTree.write(inputFile)
    print(y, x)

def btn3(event):
    global outputGrid
    XMLrootTMP = ET.parse(fileWithPath).getroot()
    for i in range(height):
        outputGrid[i] = XMLrootTMP.find("log").find("path")[i].text.split()
    outputGrid[starty][startx] = '@'
    outputGrid[finishy][finishx] = '#'
    printGrid()

def btn2(event):
    btn3(event)

root = Tk()
root.title("Grid")
c = Canvas(root, width = width * cell_size, height = height * cell_size)
c.bind("<Button-1>", btn1)
c.bind("<Button-2>", btn2)
c.bind("<Button-3>", btn3)
c.pack()
printGrid()
root.mainloop()
