import subprocess

try:
    subprocess.run(['./main_0.py'])
except NameError:
    print("NameError caught: proc_py is not defined.")
