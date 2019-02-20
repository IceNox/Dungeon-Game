-- Identification
ID = 0
VARIANT = 0
NAME = ""

-- Sprites to load
spritecount = 0
spritepaths =
{
}
spritexcenters =
{
}
spriteycenters =
{
}
spriteindexes = {}

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

-- Other
obstructive = 0
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

function update(pos, ld, curtime_t)
	-- Logic here
	
	messagecount = #messages
end

function damage(di)
	-- Damage handling here
	
	messagecount = #messages
	return false -- Return true if object is dead/destroyed
end

function select_sprite(curtime_t)
	return 0 -- Return sprite index
end


-- No need to do anything after here
function get_message(index)
	return messages[index+1]
end

function clear_messages()
	messages = {}
	messagecount = 0
end







