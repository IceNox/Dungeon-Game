-- Identification
ID = 102
VARIANT = 0
NAME = "Door"

-- Sprites to load
spritecount = 4
spritepaths =
{
	"Content/Game/Door/Door_up.tga",
	"Content/Game/Door/Door_right.tga",
	"Content/Game/Door/Door_down.tga",
	"Content/Game/Door/Door_left.tga"
}
spritexcenters =
{
	48,
	10,
	48,
	10
}
spriteycenters =
{
	60,
	96,
	60,
	96
}
spriteindexes = {}

ground = false

-- Health
maxhealth = 0
health = 0
armor = 0

healthbarheight = 0
healthbarvisible = false

-- Hitbox
hitbox = 
{
	shape = 0,
	size = { w = 0, h = 0, r = 0 },
	offset = { x = 0, y = 0 }
}

-- Light source
lightsourcecount = 0
lightsources =
{
	{ offset = { x = 0, y = 0 }, strength = 0 }
}

-- Other
transparent = 0
obstructive = 1
pressuring = 0
powering = 0

facing = 1

-- Movement
moving = false
moveprogress = 0.0

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
	startmovement = false
	movement.relative = true
	movement.x = 0
	movement.y = 0
	movement.duration = 150
	movement.mtype = -1
end

-- Here you can define your own movement functions
movementtypecount = 1
movementtypes =
{
	function (progress)
		x = math.sqrt(progress)
		y = ((2*(progress-0.5))^2*-1)*30+30
		return x, y
	end
}

-- Conversion from int to direction
dirstr = {}
dirstr[0] = "u"
dirstr[1] = "r"
dirstr[2] = "d"
dirstr[3] = "l"

-- Messages
messagecount = 0
messages = {}

-------------------
------ Logic ------
-------------------

active = false

-- Script variables

function update(pos, ld, curtime_t) end

function damage(di)
	if (di.terrain == true) then
		return true
	end
	
	return false
end

function select_sprite(curtime_t)
	if (VARIANT == 0) then
		return spriteindexes[1]
	elseif (VARIANT == 1) then
		return spriteindexes[2]
	elseif (VARIANT == 2) then
		return spriteindexes[3]
	elseif (VARIANT == 3) then
		return spriteindexes[4]
	end
	
	return 0
end


-- No need to do anything after here
function get_message(index)
	return messages[index+1]
end

function clear_messages()
	messages = {}
	messagecount = 0
end







