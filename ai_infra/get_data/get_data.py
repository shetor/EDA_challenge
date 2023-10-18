import sys 
import time
sys.path.append("..") 
from imap_engine import EngineIMAP
import subprocess


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
optimization_orders = [
        ['balance', 'rewrite','refactor','lut_opt'],
        ['rewrite','refactor' ]
            # 添加其他顺序组合
        ]
for orders in optimization_orders:
    inst.run(orders)
    print(orders)
    time_end = time.time()
    time_sum = time_end - time_start    
    # print(time_sum)
