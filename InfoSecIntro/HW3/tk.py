from utils import *

if pyversion == 2:
    import Tkinter as tkinter
    import tkMessageBox as messagebox
elif pyversion == 3:
    import tkinter
    import tkinter.messagebox as messagebox
else:
    raise Exception('Unsupported python version')
