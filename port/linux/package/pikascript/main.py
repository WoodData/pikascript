import PikaStdLib
import PikaStdDevice
import PikaStdData
import PikaStdTask
import PikaMath
from PikaObj import *
def todo1():
    print('task 1 running...')

def todo2():
    print('task 2 running...')

def todo3():
    print('task 3 running...')

def when3():
    return True

task = PikaStdTask.Task()
task.call_always(todo1)
task.call_always(todo2)
# task.call_when(todo3, when3)

task.run_once()
