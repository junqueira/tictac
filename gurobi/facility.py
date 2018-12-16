#!/usr/bin/python

import numpy as np
from gurobipy import *

f = open("../data/input.in","r")
n = int(f.readline())

cod = np.zeros(n)
pop = np.zeros(n)
idh = [0.0] * n
wage = [0.0] * n

starters = []

nodes = [
        3550308,
        3304557,
        5300108,
        2927408,
        2304400,
        3106200,
        1302603,
        4106902,
        2611606,
        4314902,
        5208707,
        3509502,
        3549904,
        3543402,
        3548500
        ]

for i in range(n):
    item = f.readline().split()
    cod[i], pop[i], idh[i], wage[i] = int(item[0]), int(item[1]), float(item[2]), float(item[3])

    if int(cod[i]) == 3509205:
        cajamar = i

    if int(cod[i]) in nodes:
        starters.append(i)

dist = [[] for _ in range(n)]
for i in range(n):
    dist[i] = list(map(float, f.readline().split()))

hasAirport = [False] * n

fixedCosts = 90000

MINIMUM_POPULATION = 0.6 * sum(pop)

locations = range(n)

m = Model("facility")

open = m.addVars(locations,
                 vtype=GRB.BINARY,
                 obj=0.0,
                 name="open")

storage = m.addVars(locations,
                 vtype=GRB.BINARY,
                 obj=0.0,
                 name="storage")

# Transportation decision variables: transport[w,p] captures the
# optimal quantity to transport to warehouse w from plant p
transport = [m.addVars(locations, obj=0.0, name="trans%d" % p) for p in locations]

# The objective is to minimize the total fixed and variable costs
m.modelSense = GRB.MINIMIZE

idhAvg = sum(idh)/len(idh)
idhMax = max(idh)

obj = LinExpr()

for p in locations:
    obj += storage[p] * fixedCosts
    obj += open[p] * (1 - (0.1 * ((idh[p] - idhAvg) / (idhMax)) * dist[[w for w in locations if storage[w] and transport[w][p]][0]][p]))
    obj -= open[p] * pop[p] * 0.5
    obj -= open[p] * wage[p] * 0.2

for p in locations:
    for w in locations:
        obj += (transport[w][p] * (dist[w][p] * (7.0 if hasAirport[p] else 9.0))) * 0.5

m.setObjective(obj)

for p in locations:
    for w in locations:
      m.addConstr(transport[w][p] <= open[p], "open[%d,%d]" % (p, w))
      m.addConstr(transport[w][p] <= storage[w], "storage[%d]" % w)

for p in locations:
    m.addConstr(open[p] == sum([transport[w][p] for w in locations]))

sumServed = LinExpr()

for p in locations:
    sumServed += open[p] * pop[p]

m.addConstr(sumServed >= MINIMUM_POPULATION)

m.addConstr(storage[cajamar] == 1.0)

for starter in starters:
    m.addConstr(open[starter] == 1.0)
# Save model
m.write('facilityPY.lp')

# First, open all plants
# for p in plants:
#     open[p].start = 1.0

# Now close the plant with the highest fixed cost
#m.Params.method = 2

# Solve
m.optimize()

# Print solution

storages = [p for p in locations if storage[p].x > 0.99]
cities = [p for p in locations if open[p].x > 0.99]

print(len(storages))
print " ".join([str(int(cod[p])) for p in storages])

print(len(cities))
print " ".join([str(int(cod[p])) for p in cities])
