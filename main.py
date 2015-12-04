from xml.dom.minidom import parse
import matrixops

def getEdge(value):
    return int(value.value) - 1

def calcResistance(a, b):
    return 1.0*(a*b)/(a+b)


input = parse("input.xml")
weights = []

ids = input.getElementsByTagName('net')
for i in range(len(ids)):
    weights.append([0]*len(ids))

resists = input.getElementsByTagName('resistor')
for resistor in resists:
    currentValue = weights[getEdge(resistor.attributes['net_from'])][getEdge(resistor.attributes['net_to'])]
    reversed_currentValue = weights[getEdge(resistor.attributes['net_to'])][getEdge(resistor.attributes['net_from'])]
    value = float(resistor.attributes['resistance'].value)
    valueToAssign = value if currentValue == 0 else calcResistance(currentValue, value)
    reversed_valueToAssign = value if reversed_currentValue == 0 else calcResistance(reversed_currentValue, value)
    weights[getEdge(resistor.attributes['net_from'])][getEdge(resistor.attributes['net_to'])] = valueToAssign
    weights[getEdge(resistor.attributes['net_to'])][getEdge(resistor.attributes['net_from'])] = reversed_valueToAssign

capactors = input.getElementsByTagName('capactor')
for capac in capactors:
    currentValue = weights[getEdge(capac.attributes['net_from'])][getEdge(capac.attributes['net_to'])]
    reversed_currentValue = weights[getEdge(capac.attributes['net_to'])][getEdge(capac.attributes['net_from'])]
    value = float(capac.attributes['resistance'].value)
    valueToAssign = value if currentValue == 0 else calcResistance(currentValue, value)
    reversed_valueToAssign = value if reversed_currentValue == 0 else calcResistance(reversed_currentValue, value)
    weights[getEdge(capac.attributes['net_from'])][getEdge(capac.attributes['net_to'])] = valueToAssign
    weights[getEdge(capac.attributes['net_to'])][getEdge(capac.attributes['net_from'])] = reversed_valueToAssign

diods = input.getElementsByTagName('diode')
for diod in diods:
    currentValue = weights[getEdge(diod.attributes['net_from'])][getEdge(diod.attributes['net_to'])]
    reversed_currentValue = weights[getEdge(diod.attributes['net_to'])][getEdge(diod.attributes['net_from'])]
    value = float(diod.attributes['resistance'].value)
    reversed_value = float(diod.attributes['reverse_resistance'].value)
    valueToAssign = value if currentValue == 0 else calcResistance(currentValue, value)
    reversed_valueToAssign = reversed_value if reversed_currentValue == 0 else calcResistance(reversed_currentValue, reversed_value)
    weights[getEdge(diod.attributes['net_from'])][getEdge(diod.attributes['net_to'])] = valueToAssign
    weights[getEdge(diod.attributes['net_to'])][getEdge(diod.attributes['net_from'])] = reversed_valueToAssign

result = matrixops.applyFloydWarshal(weights)
print(result)
