from imap_engine import EngineIMAP
class output(object):
    def __init__(self, input_file,algo_sequences) -> None:
        self.input_file = input_file
        self.engine = EngineIMAP(input_file, input_file+'.seq')
        self.algo_sequences = algo_sequences
    def add_algos_to_file(self):
        for op in self.algo_sequences:
            if op == 'rewrite':
                self.engine.add_sequence('rewrite')
            elif op == 'rewrite -l':
                self.engine.add_sequence('rewrite -l')
            elif op == 'refactor -l':
                self.engine.add_sequence('refactor -l')
            elif op == 'balance':
                self.engine.add_sequence('balance')
            elif op == 'refactor':
                self.engine.add_sequence('refactor')
            elif op == 'rewrite -z':
                self.engine.add_sequence('rewrite -z')
            elif op == 'rewrite -v':
                self.engine.add_sequence('rewrite -v')
            elif op == 'refactor -z':
                self.engine.add_sequence('refactor -z')
            elif op == 'refactor -v':
                self.engine.add_sequence('refactor -v')
            elif op == 'lut_opt':
                self.engine.add_sequence('lut_opt')
            elif op == 'map_fpga':
                self.engine.add_sequence('map_fpga')


