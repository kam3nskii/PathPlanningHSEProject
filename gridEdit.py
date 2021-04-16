import xml.etree.ElementTree as ET
from tkinter import Tk
from tkinter import Canvas
from tkinter import CURRENT

file = 'Examples/example.xml'

tree = ET.parse(file)
XMLroot = tree.getroot()

width = int(XMLroot.find("map").find("width").text)
height = int(XMLroot.find("map").find("height").text)
startx = int(XMLroot.find("map").find("startx").text)
starty = int(XMLroot.find("map").find("starty").text)
finishx = int(XMLroot.find("map").find("finishx").text)
finishy = int(XMLroot.find("map").find("finishy").text)

file2 = 'Examples/example_log_LPAStar.xml'
tree2 = ET.parse(file2)
XMLroot2 = tree2.getroot()

grid = []

for i in range(height):
    grid.append(XMLroot.find("map").find("grid")[i].text.split())

grid2 = []

for i in range(height):
    grid2.append(XMLroot2.find("log").find("path")[i].text.split())

grid2[starty][startx] = '@'
grid2[finishy][finishx] = '#'

# for i in range(height):
#     for j in range(width):
#         print(grid[i][j], end=' ')
#     print()

cell_size = 24

def btn1(event):
    c.itemconfig(CURRENT, fill="gray")
    x = event.y // cell_size
    y = event.x // cell_size
    grid[x][y] = '1'
    grid2[x][y] = grid[x][y]
    XMLroot.find("map").find("grid")[x].text = ' '.join(grid[x])
    tree.write(file)
    print(y, x)


root = Tk()
root.title("Grid")
c = Canvas(root, width = width * cell_size, height = height * cell_size)
c.bind("<Button-1>", btn1)
c.pack()
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
root.mainloop()
