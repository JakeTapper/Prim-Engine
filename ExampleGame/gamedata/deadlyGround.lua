function setup()
	timer = 0
end

function loop()

	if (timer > 1) then
		local push = Engine:getObject("PushBlock")
		local player = Engine:getObject("Player")

		player:setPosition(0, 1.5, 0)

		push:setPosition(0, 8, -27)

		timer = 0
	elseif (timer ~= 0) then
		local push = Engine:getObject("PushBlock")
		local player = Engine:getObject("Player")

		timer = timer + deltaTime

		player:setVelocity(0, 0, 0)
		push:setVelocity(0, 0, 0)
	end
end

function onCollide(other)
	if (other.name == "Player") then
		timer = deltaTime
	end
end