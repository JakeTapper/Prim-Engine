jumpTimeBuffer = 0.5

function setup()
	canJump = true
	jumpTime = 0
end

function loop()

	--Increments the jump timer counter
	if (jumpTime ~= 0) then
		jumpTime = jumpTime + deltaTime
	end

	--Spacebar
	if (keys[32] == 1 and canJump == true and (jumpTime >= jumpTimeBuffer or jumpTime == 0)) then
		this:setVelocity(this:getVelocityX(), 3.0, this:getVelocityZ())
		canJump = false
		jumpTime = deltaTime
	end

	--W
	if (keys[87] == 1) then
		this:moveWithCamera(3, true)
	end

	--S
	if (keys[83] == 1) then
		this:moveWithCamera(-3, true)
	end

	--A
	if (keys[65] == 1) then
		this:moveLeftByCamera(3)
	end

	--D
	if (keys[68] == 1) then
		this:moveLeftByCamera(-3)
	end

	--If the cube leaves the bounds of the game or F1 is pressed, reset to the start
	if (this:getPositionY() <= -10 or this:getPositionY() >= 100 or keys[290] == 1) then
		this:setPosition(0, 1.5, 0)
		this:setVelocity(0, 0, 0)

		push = Engine:getObject("PushBlock")
		push:setPosition(0, 8, -27)
		push:setVelocity(0, 0, 0)
	end
end

function onCollide(other)
	canJump = true
	if (jumpTime >= jumpTimeBuffer) then
		jumpTime = 0
	end
end