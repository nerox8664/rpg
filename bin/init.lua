log.put('Hello from lua script!')

testModule = {
	OnAttach = function ()
	end,
	OnDetach = function ()
	end,
	Tick = function ()
	end,
};

engine.attachModule(testModule, 1);

socket = require("socket")
log.put(socket._VERSION)