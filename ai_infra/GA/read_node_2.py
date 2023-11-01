import sys 
import subprocess
import re
sys.path.append("..") 
from imap_engine import EngineIMAP
#import initial_node_2 as in2

def extract_results(stats):
    """
    extracts area and delay from the printed stats on stdout
    """
 
    line = stats.decode("utf-8").split('\n')[-2].split(':')[-1].strip()
    ob = re.search(r'depth *= *[1-9]+.?[0-9]*', line)
    delay = float(ob.group().split('=')[1].strip())
    ob = re.search(r'area *= *[1-9][0-9]*(?:\.[0-9]+)?', line)
    area = float(ob.group().split('=')[1].strip()) 
    line = stats.decode("utf-8").split('\n')[0]
    operator_seq = line

    return delay,area,operator_seq

proc = subprocess.check_output(['python3','initial_node_2.py'])
# print(type(proc))
delay ,area ,operator_seq = extract_results(proc)
#print(delay)
#print(area)
#print(operator_seq)