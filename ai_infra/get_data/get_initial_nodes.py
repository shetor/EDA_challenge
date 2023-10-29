import subprocess
import time

def run_command(command):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return stdout.decode('utf-8'), stderr.decode('utf-8')
time_start = time.time()    

command = f"python3 get_data.py"
run_command(command)
output = []
result = subprocess.run(["python3", "get_data.py"], capture_output=True, text=True)

if result.returncode == 0:
    output = result.stdout
    output_lines = output.split('\n')

max_area = 0
min_area = 10000000
max_depth = 0
min_depth = 10000000
operator_sequences = []
stats = []
QoR = []

output_lines = output.split('\n')  # 使用换行符拆分行

for item in output_lines:
    if "Stats of FPGA" in item:
        stats.append(item)
    if "map_fpga" in item:
        operator_sequences.append(item)
    if "area=" in item:
        area_index = item.find("area=")
        area_end_index = item.find(",", area_index)
        if area_index != -1 and area_end_index != -1:
            area = int(item[area_index + len("area="):area_end_index])
            max_area = max(max_area, area)
            min_area = min(min_area, area)
    if "depth=" in item:
        depth_index = item.find("depth=")
        if depth_index != -1:
            depth = int(item[depth_index + len("depth="):])
            max_depth = max(max_depth, depth)
            min_depth = min(min_depth, depth)

print("max_area:",max_area)
print("min_area:",min_area)
print("max_delay:",max_depth)
print("min_delay:",min_depth)
print("stats:",stats)
print("squence: ",operator_sequences)

for item in stats:    
    depth_index = item.find("depth=")
    area_index = item.find("area=")
    area_end_index = item.find(",", area_index)
    depth = int(item[depth_index + len("depth="):])
    area = int(item[area_index + len("area="):area_end_index])
    qor = 0.6*(depth - min_depth)/(max_depth - min_depth)+0.4*(area - min_area)/(max_area-min_area)
    print("qor:",qor)
    QoR.append(qor)

print("QoR:",QoR)
end_time = time.time()
print("time: ",end_time-time_start)


