
import sys
sys.path.insert(0, "c:\\users\\james\\falcie\\build\\etro\\python\\Release")
import falpy

bb = falpy.BlockBuilder(0x01, 0x21FFFFFF)
bc = falpy.Blockchain(bb, 32)
b = bb.build()
nu1 = falpy.NetworkUpdate()
nu2 = falpy.NetworkUpdate()
nu1.add_weight(falpy.Weights([1,2,3,4,5,6,7,8,9]))
nu1.add_weight(falpy.Weights([10,11,12]))
nu2.add_weight(falpy.Weights([2,4,6,8,10,12,14,16,18]))
nu2.add_weight(falpy.Weights([24,22,20]))
nu2.set_examples_seen(2)
nu1.set_examples_seen(4)
b.add_local_update(nu1)
b.add_local_update(nu2)
bc.add(b)
last = bc.get_last()
gu = last.get_global_update()
gu.get_examples_seen()

l = gu.get_weights()

print(l[0].array)
print(l[1].array)
