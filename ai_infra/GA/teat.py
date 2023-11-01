import subprocess
import time

def run_command(command):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return stdout.decode('utf-8'), stderr.decode('utf-8')
time_start = time.time()    

command = f"python3 initial_node_2.py"
run_command(command)
output = []
result = subprocess.run(["python3", "initial_node_2.py"], capture_output=True, text=True)

if result.returncode == 0:
    output = result.stdout
    output_lines = output.split('\n')

operator_sequences = []
stats = []
area = []
depth = []
# QoR = []

output_lines = output.split('\n')  # 使用换行符拆分行

for item in output_lines:
    if "map_fpga" in item:
        operator_sequences.append(item)
    if "area=" in item:
        area_index = item.find("area=")
        area_end_index = item.find(",", area_index)
        if area_index != -1 and area_end_index != -1:
            area.append(int(item[area_index + len("area="):area_end_index]))

    if "depth=" in item:
        depth_index = item.find("depth=")
        if depth_index != -1:
            depth.append(int(item[depth_index + len("depth="):]))
 

print("depth:\n",depth)
print("area:\n",area)
print("stats:\n",stats)
print("squence: ",operator_sequences)

end_time = time.time()
print("time: ",end_time-time_start)
