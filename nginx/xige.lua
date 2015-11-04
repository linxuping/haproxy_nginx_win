--if ngx.var.http_host ~= "localhost" then
--  return ngx.exec("/robots_disallow.txt");
--end
ngx.header.content_type = "text/html";
ngx.status = ngx.HTTP_OK

args = ngx.req.get_uri_args()
uname = args["username"]
passwd = args["passwd"]
itemid = args["itemid"]

local xigelua = require "xigelua"
local timeout = 60
plist = nil
ret = nil

for i=1,3 do
	if plist == nil then
		plist,ret = xigelua.xige(uname,passwd,tonumber(itemid),timeout)
	else
		break;
	end
end

ngx.header.xigecode = ret
ngx.say( plist );


