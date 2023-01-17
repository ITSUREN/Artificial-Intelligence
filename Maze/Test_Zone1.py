frontier = [5,2,6,3]
var=0

def check(state):
    return any(var == state for node in frontier)

print(check(5))
print(var)