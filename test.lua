if PI==nil then
 write=io.write
 strlen=string.len
 PI=math.pi
 gsub=string.gsub
 strbyte=string.byte
 format=string.format
end

function hex(s)
 s=gsub(s,"(.)",function (x) return format("%02X",strbyte(x)) end)
 return s
end

s=bpack("Sidif",_VERSION,1962,4514,-1,4514)
print(hex(s))
write(s,"\n")

S,a,i=bunpack(s,"Si")
print(S,a,i,strlen(s))
x,i=bunpack(s,"d",i)
print(x,i,strlen(s))
_,x,i=bunpack(s,"if",i)
print(x,i,strlen(s))
