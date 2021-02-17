arq = open('output.txt', 'r')

linha = arq.readline().split()
linha_int = list(map(int, linha))
anterior = None
for elem in linha_int:
    if anterior == None:
        anterior = elem
    else:
        if elem < anterior:
            print("Array ordenada incorretamente: {} antes de {}".format(anterior, elem))
            break
        anterior = elem

print("O vetor está corretamente ordenado!")
        
arq.close()
