# EDA_Challenge

## Introduction

This repository is the source code of the EDA Elite Challenge Question  2: Intelligent Process of Combinational Logic Optimization and Process Mapping. The development platform is iMAP. This project is targeted at the given AIG(And-Inverter Graph) input combinational logic circuit, requiring the intelligent process algorithm to Features, dynamically provide a suitable sequence of logical optimization algorithms, and finally output a functionally equivalent LUT (look-up table) netlist. We used dynamic programming + multi-stage genetic algorithm to realize the design of intelligent process, greatly optimizing the area and delay. We provide the running script(run.sh)  for users to use.

## User guide

#### Method one:

When users enter our repository (EDA Challenge), they will see a running script (run.sh). After compiling and generating an executable file, they can directly run the following instructions:

```shell
run.sh input_path/*.aig output_path/*.seq
```

After running this command, the operator sequence will be output to the *.seq file in the specified path.

**For example**:

```
run.sh /home/EDA_challenge/benchmarks/adder.aig /home/EDA_challenge/output/adder.seq
```

#### Method two:

Users can go to cli/bin and then you will see the imap executable file,and then you can run the following command.

```
read_aiger -f ../benchmark/adder/adder.aig 
ga -O ../output/adder.seq
```

## Develop Enviroment

- Integrated Development Environment: CLion CL-222.4167.35

- OS: Ubuntu 22.04
- Compiler tool chain: gcc 11.4.0 

