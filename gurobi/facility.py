#!/usr/bin/python

from gurobipy import *

f = open("../data/input.in","r")
n = int(f.readline())

cod = np.zeros(n)
pop = np.zeros(n)

for i in range(n):
    cod[i], pop[i] = map(int, f.readline().split())

dist = [[] for _ in range(n)]
for i in range(n):
    dist[i] = list(map(float, f.readline().split()))

hasAirport = [False] * n


# Fixed costs for each plant
fixedCosts = 90000

MINIMUM_POPULATION = 0.6 * sum(pop)

# Range of plants and warehouses
locations = range(n)

# Model
m = Model("facility")

# Plant open decision variables: open[p] == 1 if plant p is open.
open = m.addVars(locations,
                 vtype=GRB.BINARY,
                 obj=fixedCosts,
                 name="open")

storage = m.addVars(locations,
                 vtype=GRB.BINARY,
                 obj=fixedCosts,
                 name="storage")

# Transportation decision variables: transport[w,p] captures the
# optimal quantity to transport to warehouse w from plant p
transport = m.addVars(locations, locations, obj=dist, name="trans")

# The objective is to minimize the total fixed and variable costs
m.modelSense = GRB.MINIMIZE

obj = LinExpr()

for p in locations:
    obj += open[p] * (0.0)

for p in locations:
    for w in locations:
        obj += transport[w][p] * (dist[w][u] * (7.0 if hasAirport[u] else 9.0))

m.setObjective(obj)

for p in locations:
    for w in locations:
      m.addConstr(transport[w][p] <= open[p], "open[%d]" % p)
      m.addConstr(transport[w][p] <= storage[w], "storage[%d]" % w)

m.addConstr(sum(sum(transport[w][p] for p in locations)*population[w] for p in
    locations) >= MINIMUM_POPULATION)

for w in locations:
    m.addConstr(sum(transport[w][p] for p in locations) >= open[p],
            "transport[%d]" % w)
# Save model
m.write('facilityPY.lp')

# First, open all plants
for p in plants:
    open[p].start = 1.0

# Now close the plant with the highest fixed cost
print('Initial guess:')
maxFixed = max(fixedCosts)
for p in plants:
    if fixedCosts[p] == maxFixed:
        open[p].start = 0.0
        print('Closing plant %s' % p)
        break
print('')

# Use barrier to solve root relaxation
m.Params.method = 2

# Solve
m.optimize()

# Print solution
print('\nTOTAL COSTS: %g' % m.objVal)
print('SOLUTION:')
for p in plants:
    if open[p].x > 0.99:
        print('Plant %s open' % p)
        for w in warehouses:
            if transport[w,p].x > 0:
                print('  Transport %g units to warehouse %s' % \
                      (transport[w,p].x, w))
    else:
        print('Plant %s closed!' % p)
