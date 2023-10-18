import sys 
import subprocess
def run_command(command):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return stdout.decode('utf-8'), stderr.decode('utf-8')

command = f"python3 get_data.py"
run_command(command)
    
result = subprocess.run(["python3", "get_data.py"], capture_output=True, text=True)
if result.returncode == 0:
    output = result.stdout
    print("output: ",output)
    