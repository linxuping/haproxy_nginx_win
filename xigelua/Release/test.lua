require "xigelua"  --ָ��������

--�ڵ���ʱ��������package.function
--print(xigelua.sub(20.1,19))
--print(xigelua.add(1.0,2.0))
local timeout = 8
a = nil
b = nil

for i=1,3 do
	if a == nil then
		a, b = xigelua.xige("***","***",1009227147,timeout)
	else
		break;
	end
end



print("return");
print(a);
print(b);
--a,b = xigelua.xige2();
--print(a,b)