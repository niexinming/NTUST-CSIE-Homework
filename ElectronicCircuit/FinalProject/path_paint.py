from tkinter import *
from threading import Thread, Lock
import math
import time
import bluetooth



class DataParser(object):
    def __init__(self, serial):
        self.serial = serial

    def setup(self):
        self.serial.cmd(b'Init')
        print('Wait...')
        while True:
            r = self.serial.readline().decode().strip()
            if r == '#':
                break

    def get(self):
        x = self.serial.readline().decode().strip()
        y = self.serial.readline().decode().strip()
        z = self.serial.readline().decode().strip()
        self.serial.readline()
        return float(x), float(y), float(z)

class ThreadReader(Thread):
    def __init__(self):
        super().__init__()
        self.lock = Lock()
        self.pool = []
        self._stop_flag = False

    def run(self):
        try:
            print('Thread Start')
            serial = bluetooth.BlueToothSerial('/dev/tty.Inndy-DevB')
            parser = DataParser(serial)
            parser.setup()
            while not self._stop_flag:
                data = parser.get()
                self.lock.acquire()
                self.pool.append(data)
                self.lock.release()
                time.sleep(0.0555)
        except:
            pass
        finally:
            print('Thread Exit')

    def stop(self, wait=False):
        self._stop_flag = True
        if wait:
            self.join()

    def get(self):
        if self.lock.acquire(False):
            data = self.pool
            self.pool = []
            self.lock.release()
            return data
        else:
            return []


reader = None
def reconnect():
    global reader
    reader = ThreadReader()
    reader.start()


lasty, lastx = 0, 0

def left_click_event(event):
    global lastx, lasty
    lastx = event.x
    lasty = event.y

def left_move_event(event):
    global lastx, lasty
    canvas.create_line((lastx, lasty, event.x, event.y))
    lasty = event.y
    lastx = event.x



reconnect()

root = Tk()
root.title("Painter")


canvas = Canvas(root, width = 800, height = 600)
canvas.grid(row = 1, column = 1, columnspan = 2)

btnConnect = Button(root, text = "Connect", command = reconnect)
btnConnect.grid(row = 0, column = 0)
btnClear = Button(root, text = "Clear", command = lambda: canvas.delete("all"))
btnClear.grid(row = 0, column = 1)

canvas.bind('<Button-1>', left_click_event)
canvas.bind('<B1-Motion>', left_move_event)


def draw_loop():
    global lastx, lasty
    ax, ay = 0, 0
    for x, y, _ in reader.get():
        ax, ay = ax+x, ay+y
    ax, ay = ax * 3, ay * 3
    ay = -ay
    if -2.8 < ax < 2.8 and -2.8 < ay < 2.8:
        pass
    else:
        vx, vy = ax, ay
        nx, ny = lastx + vx, lasty + vy
        canvas.create_line((lastx, lasty, nx, ny))
        print('%6.3f, %6.3f, %7.4f, %7.4f, %7.4f, %7.4f' % (ax, ay, lastx, lasty, nx, ny))
        lastx, lasty = nx, ny
    root.after(90, draw_loop)

draw_loop()
root.mainloop()

print('Wait for thread')
reader.stop()
