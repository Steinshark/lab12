access = [120,10,15,150,60,190,195,20,35,50]

location = 100


while not len(access) == 0:
    distances = [abs(location - i) for i in access]

    closest = min(distances)
    index = distances.index(closest)

    print(access[index])
    location = access[index]
    access.pop(index)
