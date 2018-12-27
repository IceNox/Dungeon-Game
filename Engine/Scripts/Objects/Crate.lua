-- Identification
ID = 101
VARIANT = 0
NAME = "Crate"

-- Sprites to load
spritecount = 1
spritepaths =
{
	"Content/Game/Wiring/Crate.tga"
}
spritexcenters =
{
	36
}
spriteycenters =
{
	68
}
spriteindexes = {}

-- Hitbox
hitbox = 
{
	shape = 1,
	size = { w = 72, h = 72, r = 0 },
	offset = { x = 0, y = 0 }
}

-- Light source
lightsourcecount = 0
lightsources =
{
	{ offset = { x = 0, y = 0 }, strength = 0 }
}

-- Other
obstructive = 1
pressuring = 1
powering = 0

-- 125 of 6666 functions ( 1.9%) were compiled

-- Movement
startmovement = false
movement =
{
	relative = true,
	x = 0,
	y = 0,
	duration = 150,
	mtype = -1
}

function reset_movement()
	movement.relative = true
	movement.x = 0
	movement.y = 0
	movement.duration = 150
	movement.mtype = -1
end

movementtypecount = 1
movementtypes =
{
	function (progress)
		x = math.sqrt(progress)
		y = ((2*(progress-0.5))^2*-1)*30+30
		return x, y
	end
}

-- Logic
function update(ld, curTime) end

function select_sprite(facing, moving, progress)
	return spriteindexes[1]
end

function damage(di, destroyed)
	if (di.terrain == true) then
		movement.mtype = 1
		movement.relative = true
		movement.duration = 150
		
		if (di.direction == -1) then
			movement.x = 0
			movement.y = 0
		end
		if (di.direction == 0) then
			movement.x = 0
			movement.y = -1
		end
		if (di.direction == 1) then
			movement.x = 1
			movement.y = 0
		end
		if (di.direction == 2) then
			movement.x = 0
			movement.y = 1
		end
		if (di.direction == 3) then
			movement.x = -1
			movement.y = 0
		end
		
		startmovement = true
	end
	
	return false;
end