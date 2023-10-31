import subprocess
import re
import time
import random

def get_random_sequence(strings,algo_num):
    counts = [0] * len(strings)
    for i in range(algo_num ):       
        index = random.randint(0, len(strings) - 1)
        counts[index] += 1

    sequences = []

    for i in range(len(strings)):
        string = strings[i]
        count = counts[i]        
        sequences.extend([string] * count)
        random.shuffle(sequences)
    sequences.append("map_fpga")
    return sequences

def extract_results(stats):
    """
    extracts area and delay from the printed stats on stdout
    """
    line = stats.decode("utf-8").split('\n')[-2].split(':')[-1].strip()
    ob = re.search(r'depth *= *[1-9]+.?[0-9]*', line)
    delay = float(ob.group().split('=')[1].strip())
    ob = re.search(r'area *= *[1-9][0-9]*(?:\.[0-9]+)?', line)
    area = float(ob.group().split('=')[1].strip())  
    return delay,area
def get_initial_output(design_file):
    imap_command = './imap -c ' 
    imap_command+='"read_aiger -f '+design_file+'; '  
    imap_command+='map_fpga; '
    imap_command+='print_stats -t 1; "'
    proc = subprocess.check_output(imap_command,shell=True,cwd='../../bin/')
    print(proc)
    delay,area = extract_results(proc)
    return delay,area

def run_optimization(design_file,order):
    imap_command = './imap -c ' 
    imap_command+='"read_aiger -f '+design_file+'; '
    imap_command+='print_stats -t 0; '

    for op in order:
        if op == 'rewrite':
            imap_command+='rewrite; '
        elif op == 'balance':
            imap_command+='balance; '
        elif op == 'refactor':
            imap_command+='refactor; '
        elif op == 'rewrite -z':
            imap_command+='rewrite -z; '
        elif op == 'rewrite -v':
            imap_command+='rewrite -v;'
        elif op == 'refactor -z':
            imap_command+='refactor -z;'
        elif op == 'refactor -v':
            imap_command+='refactor -v;'  
        elif op == 'lut_opt':
            imap_command+='lut_opt;'    
        elif op == 'map_fpga':
            imap_command+='map_fpga; '                     
    imap_command+='print_stats -t 1;"'

    proc = subprocess.check_output(imap_command,shell=True,cwd='../../bin/')
    print(proc)
    delay,area = extract_results(proc)
    return delay,area

start_time = time.time()
input_file = '../benchmark/b05_comb/b05_comb.aig'
strings = ["balance", "rewrite", "rewrite -z", "rewrite -v", "refactor", "refactor -z", "refactor -v"]
algo_num = 10
# algo_sequence = get_random_sequence(strings,algo_num)
# delay,area=run_optimization(input_file,algo_sequence)
delay,area=get_initial_output(input_file)
print("delay:",delay)
print("area",area)
end_time = time.time()

print("run_time",end_time - start_time)