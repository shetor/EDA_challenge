import numpy as np
import time
import random
import sys 
import importlib as imp
sys.path.append("..") 
from imap_engine import EngineIMAP
import get_node_by_command as get_node

time_start = time.time()

input_file = '../benchmark/b05_comb/b05_comb.aig'
strings = ["balance", "rewrite", "rewrite -z", "rewrite -v", "refactor", "refactor -z", "refactor -v"]
algo_num = 10  #the numbers of the operator sequence
no_opt_delay,no_opt_area = get_node.get_initial_output(input_file)
print(no_opt_area)
print(no_opt_delay)

initial_population = [] 
initial_area = []
initial_delay = []
for i in range(50):  
    initial_node = get_node.get_random_sequence(strings,algo_num)
    print(initial_node)
    initial_population.append(initial_node)
    delay,area=get_node.run_optimization(input_file,initial_node)
    initial_area.append(area)
    initial_delay.append(delay)
    #imp.reload(get_node)  #reload read_node_2.py to get new node

def reward_func(current_area , current_delay , no_opt_area , no_opt_delay):
    qor = 0.4*(current_area / no_opt_area) + 0.6*(current_delay / no_opt_delay)
    return qor

def fitness_func(current_area , current_delay , no_opt_area , no_opt_delay):
    fitness_value = 1/(reward_func(current_area , current_delay , no_opt_area , no_opt_delay))
    return fitness_value
qors = []
initial_fitness = []
for i in range(50):
    qor = reward_func(initial_area[i], initial_delay[i], no_opt_area, no_opt_delay)
    qors.append(qor)
    fitness = fitness_func(initial_area[i], initial_delay[i], no_opt_area, no_opt_delay)
    initial_fitness.append(fitness)

#print(qors)
#print(initial_fitness)

def compute_probability(fitness_value):  #计算适应度概率
    total = 0
    fit_p = []
    for i in range(len(fitness_value)):
        total += fitness_value[i]
    for i in range(len(fitness_value)):
        p = fitness_value[i] / total
        fit_p.append(p)
    return fit_p

def ga_selection(population, fitness_value):
    #轮盘赌选择
    fit_p = compute_probability(fitness_value)
    print(fit_p)
    select_population = []
    for i in range(10):
        random_num = random.uniform(0.0,1.0)
        print(random_num)
        cum_p = 0
        for j in range(len(fit_p)):
            cum_p = cum_p + fit_p[j]
            if random_num <= cum_p:
                select_population.append(population[j])
                break
    return select_population

new_population1 = ga_selection(initial_population,initial_fitness)
new_population2 = ga_selection(initial_population,initial_fitness)

def crossvover(select_population):
    return





#print(initial_population)
#print(initial_population[1])
#print(initial_area)
#print(initial_delay)
print(new_population1)
print(new_population2)
#print(len(new_population))


time_end = time.time()
print(time_end-time_start)