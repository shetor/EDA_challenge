import sys 
import time
sys.path.append("..") 
from imap_engine import EngineIMAP


def get_stats(engine):
    output = engine.print_stats(type=0)
    print(output)
    return output

def main():
    input_file = '../../benchmark/adder/adder.aig'
    engine = EngineIMAP(input_file, input_file + '.seq')
    engine.read()
    get_stats(engine)

if __name__ == "__main__":
    main()