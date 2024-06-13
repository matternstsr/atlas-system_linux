import subprocess

try:
    result = subprocess.run(['./main_0.py'], stderr=subprocess.PIPE, text=True)
    if "NameError" in result.stderr:
        print("NameError caught: proc_py is not defined.")
except Exception as e:
    print("An error occurred:", e)
