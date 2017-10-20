function setup()

	--Create the regular static ground
	Ground = {
		name = "Ground",
		position = {0, 0, -2},
		color = {0.3, 0.1, 0.1}, 
		size = {3, 1, 6},
		staticBox = 1, 
		gravity = 0 
	} 
	engine.Cube.createNew(Ground)

	Ground["size"] = {3, 1, 4}
	Ground["position"] = {0, 1, -3}
	engine.Cube.createNew(Ground)

	Ground["size"] = {3, 1, 2}
	Ground["position"] = {0, 2, -4}
	engine.Cube.createNew(Ground)

	Ground["size"] = {3, 6, 1}
	Ground["position"] = {0, 7.5, -4}
	engine.Cube.createNew(Ground)

	Ground["size"] = {3, 6, 1}
	Ground["position"] = {0, 3, -8}
	engine.Cube.createNew(Ground)

	Ground["size"] = {1, 1, 15}
	Ground["position"] = {0, 0, -17.5}
	engine.Cube.createNew(Ground)

	Ground["size"] = {3, 2, 1}
	Ground["position"] = {0, 1, -27}
	engine.Cube.createNew(Ground)

	Ground["size"] = {3, 2, 1.5}
	Ground["position"] = {0, 3, -26.75}
	engine.Cube.createNew(Ground)

	Ground["size"] = {3, 2, 2}
	Ground["position"] = {0, 5, -26.5}
	engine.Cube.createNew(Ground)

	Ground["size"] = {3, 2, 2.5}
	Ground["position"] = {0, 7, -26.25}
	engine.Cube.createNew(Ground)


	--Create ground that kills the player and resets the game
	DeadlyGround = {
		name = "DeadlyGround",
		position = {-1.75, 4.25, -26.25},
		size = {.5, 8.5, 2.5},
		color = {0.1, 0.1, 0.1}, 
		staticBox = 1, 
		gravity = 0,
		script = "gamedata/deadlyGround.lua"
	}
	engine.Cube.createNew(DeadlyGround)

	DeadlyGround["position"] = {1.75, 4.25, -26.25}
	engine.Cube.createNew(DeadlyGround)

	DeadlyGround["size"] = {3, 0.5, 1}
	DeadlyGround["position"] = {0, 4.25, -4}
	engine.Cube.createNew(DeadlyGround)

	DeadlyGround["position"] = {0, 6.25, -8}
	engine.Cube.createNew(DeadlyGround)


	--Creates the special function blocks
	PushBlock = {
		name = "PushBlock",
		position = {0, 8, -27},
		color = {0.1, 0.1 , 1.0},
		staticBox = 0,
		gravity = 2,
		friction = 1
	}
	engine.Cube.createNew(PushBlock)

	Finish = {
		name = "FinishLine",
		position = {0, 1, -29},
		color = {1, 1, 1}, 
		staticBox = 1, 
		gravity =  0,
		script = "gamedata/finishLine.lua"
	} 
	engine.Cube.createNew(Finish)

	Player = {
		name = "Player",
		position = {0, 1.5, 0},
		rotation = {0, 0, 0},
		color = {0.2, 0.1, 0.2},
		gravity = 2,
		friction = 1,
		staticBox = 0,
		script = "gamedata/player.lua"
	}
	engine.Cube.createNew(Player)
end

function loop()

end