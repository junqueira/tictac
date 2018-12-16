#!/usr/bin/python

from gurobipy import *

f = open("input.in","r")
n = int(f.readline())

cod = np.zeros(n)
pop = np.zeros(n)

for i in range(n):
    cod[i], pop[i] = map(int, f.readline().split())

dist = [[] for _ in range(n)]
for i in range(n):
    dist[i] = list(map(float, f.readline().split()))


# Fixed costs for each plant
fixedCosts = 90000

# Transportation costs per thousand units
transCosts = [[4000, 2000, 3000, 2500, 4500],
              [2500, 2600, 3400, 3000, 4000],
              [1200, 1800, 2600, 4100, 3000],
              [2200, 2600, 3100, 3700, 3200]]

MINIMUM_POPULATION = 10000000

# Range of plants and warehouses
locations = range(n)

# Model
m = Model("facility")

# Plant open decision variables: open[p] == 1 if plant p is open.
open = m.addVars(locations,
                 vtype=GRB.BINARY,
                 obj=fixedCosts,
                 name="open")

# Transportation decision variables: transport[w,p] captures the
# optimal quantity to transport to warehouse w from plant p
transport = m.addVars(locations, locations, obj=dist, name="trans")

# You could use Python looping constructs and m.addVar() to create
# these decision variables instead.  The following would be equivalent
# to the preceding two statements...
#
#open = []
#for p in plants:
#  open.append(m.addVar(vtype=GRB.BINARY,
#                       obj=fixedCosts[p],
#                       name="open[%d]" % p))
#
#transport = []
#for w in warehouses:
#  transport.append([])
#  for p in plants:
#    transport[w].append(m.addVar(obj=transCosts[w][p],
#                                 name="trans[%d,%d]" % (w, p)))

# The objective is to minimize the total fixed and variable costs
m.modelSense = GRB.MINIMIZE

# Production constraints
# Note that the right-hand limit sets the production to zero if the plant
# is closed
for p in locations:
    for w in locations:
      m.addConstr(transport[w][p] <= open[p], "open[%d]" % p)

m.addConstr(sum(sum(transport[w][p] for p in locations)*population[w] for p in
    locations) >= MINIMUM_POPULATION)

for w in locations:
    m.addConstr(sum(transport[w][p] for p in locations) <= 1,
            "transport[%d]" % w)
# Save model
m.write('facilityPY.lp')

# Guess at the starting point: close the plant with the highest fixed costs;
# open all others

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
