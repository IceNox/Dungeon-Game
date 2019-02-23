-- Identification
ID = 104
VARIANT = 0
NAME = "Lamp"

-- Sprites to load
spritecount = 2
spritepaths =
{
	"Content/Game/Wiring/Lamp_led_off.tga",
	"Content/Game/Wiring/Lamp_led_on.tga"
}
spritexcenters =
{
	12,
	12
}
spriteycenters =
{
	20,
	20
}
spriteindexes = {}

ground = true

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
transparent = 1
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
powered = false

function update(pos, ld, curtime_t)
	gpos = position.new()
	gpos.x = math.floor(pos.x / 96)
	gpos.y = math.floor(pos.y / 96)
	
	index = gpos.y * ld.width + gpos.x
	if (ld.tiles[index+1].powered == 1) then
		powered = true
	else
		powered = false
	end
end

function damage(di) end

function select_sprite(curtime_t)
	if (VARIANT == 0) then
		if (powered == false) then
			return spriteindexes[1]
		else
			return spriteindexes[2]
		end
	end
	
	return 0
end

-- Serialization
function serialize()
	return ""
end

function deserialize(data)

end

-- No need to do anything after here
function get_message(index)
	return messages[index+1]
end

function clear_messages()
	messages = {}
	messagecount = 0
end







