import time
import random
import sys 
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
for i in range(6):  
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
for i in range(6):
    qor = reward_func(initial_area[i], initial_delay[i], no_opt_area, no_opt_delay)
    qors.append(qor)
    fitness = fitness_func(initial_area[i], initial_delay[i], no_opt_area, no_opt_delay)
    initial_fitness.append(fitness)

def select_best(population, fitness_value, initial_area, initial_delay):
    """
    select the best individual from pop
    """
    max_fitness = max(fitness_value)
    max_index = fitness_value.index(max_fitness)
    best_selection = population[max_index]
    area = initial_area[max_index]
    delay = initial_delay[max_index]
    return best_selection, area, delay

best_sequence, area, delay = select_best(initial_population,initial_fitness,initial_area,initial_delay)

def find_top_half_population(fitness):
    index_fit = list(enumerate(fitness))
    sorted_fit = sorted(index_fit, key = lambda x:x[1], reverse=True)  
    top_half = sorted_fit[:(len(fitness)//2)]
    top_half_fit = [elem for _, elem in top_half]
    top_half_index = [idx for idx, _ in top_half]
    return top_half_fit, top_half_index

better_fit_of_pop, index_of_better_pop = find_top_half_population(initial_fitness)

def compute_probability(fitness_value):  #compute fitness probability
    fit_p = []
    total = sum(fitness_value)
    print("total:",total)
    for i in range(len(fitness_value)):
        p = fitness_value[i] / total
        fit_p.append(p)
    return fit_p


fit_p = compute_probability(initial_fitness)
next_population = []
next_fitness = []
next_area = []
next_delay = []

for i in range(len(index_of_better_pop)):
        next_population.append(initial_population[index_of_better_pop[i]])
        next_fitness.append(initial_population[index_of_better_pop[i]])
        next_area.append(initial_area[index_of_better_pop[i]])
        next_delay.append(initial_delay[index_of_better_pop[i]])

print(initial_area)
print(initial_delay)
print(initial_fitness)
print(best_sequence)
print(area)
print(delay)
print(better_fit_of_pop)
print(index_of_better_pop)
print("next_population:",next_population)
print("next_area:",next_area)
print("next_delay:",next_delay)

print(fit_p)