import sys 
import time
sys.path.append("..") 
from imap_engine import EngineIMAP
import subprocess
import random
from get_initial_nodes import random

class GetInitialNodes(object):
    
    def __init__(self, input_file) -> None:
        self.input_file = input_file
        self.engine = EngineIMAP(input_file,input_file+'.seq')
    
    def optimize_with_order(self, order):
        for op in order:
            if op == 'rewrite':
                self.engine.rewrite()
            elif op == 'balance':
                self.engine.balance()
            elif op == 'refactor':
                self.engine.refactor(zero_gain=True)
            elif op == 'lut_opt':
                self.engine.lut_opt()
        self.engine.print_stats(type=0)
    
    def run(self,order):
        self.engine.read()
        self.optimize_with_order(order)

time_start = time.time()    
input_file = '../../benchmark/adder/adder.aig'
inst = GetInitialNodes(input_file)

strings = ["balance", "rewrite", "rewrite -z", "rewrite -v", "refector", "refector -z", "refector -v"]

from get_initial_nodes import random

Random =random()
data_seq = []

total_count = Random.total_count
total = Random.total

for j in range(total_count):

    counts = [0] * len(strings)

    for i in range(total - 1):
        index = random.randint(0, len(strings) - 1)
        counts[index] += 1

    # for i in range(len(strings)):
    #     print(f"{strings[i]} 出现次数: {counts[i]}")

    sequences = []
    sequences.append(strings[j % len(strings)])

    for i in range(len(strings)):
        string = strings[i]
        count = counts[i]
        if i != j % len(strings):
            sequences.extend([string] * count)

    random.shuffle(sequences[1:])
    data_seq.append(sequences)
    #print(Alg)

optimization_orders = data_seq

for orders in optimization_orders:
    inst.run(orders)
    #print(orders)
    time_end = time.time()
    time_sum = time_end - time_start    
    #print(time_sum)
