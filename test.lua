bpack=string.pack
bunpack=string.unpack

function hex(s)
 s=string.gsub(s,"(.)",function (x) return string.format("%02X",string.byte(x)) end)
 return s
end

a=bpack("Ab10n","\027Lua",5*16+0,1,4,4,4,6,8,9,9,8,3.14159265358979323846E7)
print(hex(a))

b=string.dump(hex)
b=string.sub(b,1,string.len(a))
print(a==b,string.len(b))
print(bunpack(b,"bA3b10n"))

i=314159265 f="<I>I=I"
a=bpack(f,i,i,i)
print(hex(a))
print(bunpack(a,f))

i=3.14159265 f="<d>d=d"
a=bpack(f,i,i,i)
print(hex(a))
print(bunpack(a,f))
