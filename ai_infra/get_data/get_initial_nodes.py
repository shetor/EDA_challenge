import sys 
import subprocess
import re

class random:
    def _init_ (self):
        self.total_count = 5
        self.total = 5


def run_command(command):
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = process.communicate()
    return stdout.decode('utf-8'), stderr.decode('utf-8')

command = f"python3 get_data.py"
run_command(command)


result = subprocess.run(["python3", "get_data.py"], capture_output=True, text=True)
if result.returncode == 0:
    output = result.stdout
    output_lines = output.split('\n')

   
    print("output: \n",output)
    print("stop")


Alg = []

pattern = r"Stats of AIG: pis=(\d+), pos=(\d+), area=(\d+), depth=(\d+),not_num=(\d+)\n\[(.*)\]"

max_area = 0
min_area = float('inf')
max_depth = 0
min_depth = float('inf')


for item in output:
    area = int(item.split("area=")[1].split(",")[0])
    depth = int(item.split("depth=")[1].split(",")[0])
    max_area = max(max_area, area)
    min_area = min(min_area,area)
    max_depth = max(max_depth, depth)
    min_depth = min(min_depth,depth)

print("Max Area:", max_area)
print("Min Area:", min_area)
print("Max Depth:", max_depth)
print("Min Depth:", min_depth)

for item in output:
    matches = re.findall(pattern, item)
    if matches:
        match = matches[0]
        pis = int(match[0])
        pos = int(match[1])
        area = [int(val) for val in match[2].split(',')]
        depth = [int(val) for val in match[3].split(',')]
        sequence = match[5].replace("'", "").split(", ")

    
weight_area = 0.6
weight_depth = 0.4

for sequence, area, depth in Alg:
    QOR = []
    for a, d in zip(area, depth):
        normalized_area = (a - min_area) / (max_area - min_area) if max_area != min_area else 0.0
        normalized_depth = (d - min_depth) / (max_depth - min_depth) if max_depth != min_depth else 0.0
        qor = weight_area * normalized_area + weight_depth * normalized_depth
        QOR.append(qor)


    Alg.append((sequence, QOR))
print(QOR)  
print(Alg)

