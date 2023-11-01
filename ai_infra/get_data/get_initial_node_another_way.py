import sys 
import subprocess
import re
sys.path.append("..") 
from imap_engine import EngineIMAP
import time
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> 8d48cb0 (zhixin_push get_data_command)
=======
>>>>>>> main
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

class get_data(object):
    def __init__(self, input_file) -> None:
        self.input_file = input_file
        self.engine = EngineIMAP(input_file,input_file+'.seq')
        self.algo_num = 6
        self.sequences_num = 10

    def set_algo_num(self, new_algo_num):
        self._algo_num = new_algo_num    

    def set_sequences_num(self, new_sequences_num):
        self._sequences_num = new_sequences_num

<<<<<<< HEAD
<<<<<<< HEAD
start_time=time.time()
=======
start_time = time.time()
>>>>>>> 8d48cb0 (zhixin_push get_data_command)
=======
start_time = time.time()
>>>>>>> main
proc = subprocess.check_output(['python3','get_data_another_way.py'])
# print(type(proc))
delay ,area= extract_results(proc)
end_time = time.time()
print(delay)
print(area)
<<<<<<< HEAD
<<<<<<< HEAD
end_time=time.time()
print("run_time",end_time - start_time)    
=======
=======
>>>>>>> main
print("time:",end_time-start_time)


    
>>>>>>> 8d48cb0 (zhixin_push get_data_command)
