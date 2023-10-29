import sys 
import time
sys.path.append("..") 
from imap_engine import EngineIMAP
import random

class GetInitialNodes(object):
    def __init__(self, input_file) -> None:
        self.input_file = input_file
        self.engine = EngineIMAP(input_file,input_file+'.seq')
        self.algo_num = 6
        self.sequences_num = 100
    
    def optimize_with_order(self, order):
        for op in order:
            if op == 'rewrite':
                self.engine.rewrite()
            elif op == 'balance':
                self.engine.balance()
            elif op == 'refactor':
                self.engine.refactor()
            elif op == 'rewrite -z':
                self.engine.rewrite(zero_gain=True)
            elif op == 'rewrite -v':
                self.engine.rewrite(verbose=True)
            elif op == 'refactor -z':
                self.engine.refactor(zero_gain=True)
            elif op == 'refactor -v':
                self.engine.refactor(verbose=True)   
            elif op == 'lut_opt':
                self.engine.lut_opt()      
            elif op == 'map_fpga':
                self.engine.map_fpga()                          
        self.engine.print_stats(type=1)
            
    def run(self,order):
        self.engine.read()
        self.optimize_with_order(order)

time_start = time.time()    
input_file = '../../benchmark/b05_comb/b05_comb.aig'
inst = GetInitialNodes(input_file)
# "lut_opt",
strings = ["balance", "rewrite", "rewrite -z", "rewrite -v", "refactor", "refactor -z", "refactor -v"]
for j in range(inst.sequences_num):
    counts = [0] * len(strings)
    for i in range(inst.algo_num ):       
        index = random.randint(0, len(strings) - 1)
        counts[index] += 1

    sequences = []

    for i in range(len(strings)):
        string = strings[i]
        count = counts[i]        
        sequences.extend([string] * count)
        random.shuffle(sequences)
    sequences.append("map_fpga")

    print(sequences)
    inst.run(sequences)    
