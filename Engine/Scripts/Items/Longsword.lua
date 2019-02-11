-- Identification
ID = 1
VARIANT = 0
NAME = "Longsword"

-- Sprites to load
spritecount = 5
spritepaths =
{
	"Content/Game/Items/Longsword.tga",
	"Content/Game/Animations/Attack/Attack_chop_up_animation-f07.tga",
	"Content/Game/Animations/Attack/Attack_chop_right_animation-f07.tga",
	"Content/Game/Animations/Attack/Attack_chop_down_animation-f07.tga",
	"Content/Game/Animations/Attack/Attack_chop_left_animation-f07.tga"
}
spriteindexes = {}

-- Stacking
stackable = false
stacklimit = 1

-- Usage
usable = true
usetime = 0
cooldown = 1000

-- Hitpattern
hitp = {}

-- UP
hitp[0] =
{
	centerx = 1,
	centery = 3,
	width = 1,
	height = 3,
	pattern =
	{
		{1},
		{1},
		{0}
	}
}
-- RIGHT
hitp[1] =
{
	centerx = 1,
	centery = 1,
	width = 3,
	height = 1,
	pattern =
	{
		{0, 1, 1}
	}
}
-- DOWN
hitp[2] =
{
	centerx = 1,
	centery = 1,
	width = 1,
	height = 3,
	pattern =
	{
		{0},
		{1},
		{1}
	}
}
-- LEFT
hitp[3] =
{
	centerx = 3,
	centery = 1,
	width = 3,
	height = 1,
	pattern =
	{
		{1, 1, 0}
	}
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

-- Logic
function use(pos, direction, ld, curtime_t)
	gpos = position.new()
	gpos.x = math.floor(pos.x / 96)
	gpos.y = math.floor(pos.y / 96)

	if (direction == -1) then
		return
	end
	
	for y = 1, hitp[direction].height do
		for x = 1, hitp[direction].width do
			dx = x - hitp[direction].centerx
			dy = y - hitp[direction].centery
			
			if (hitp[direction].pattern[y][x] == 1) then
				messages[#messages+1] = levelMessage.new()
				messages[#messages].source = "player"
				messages[#messages].message = "damage " .. gpos.x + dx .. " " .. gpos.y + dy .. " 1 p " .. dirstr[direction]
			end
		end
	end
	
	-- Show swing animation
	messages[#messages+1] = levelMessage.new()
	messages[#messages].source = "player"
	if (direction == 0) then
		messages[#messages].message = "animation " .. pos.x .. " " .. pos.y - 48 - 36 .. " " .. spriteindexes[2]
	elseif (direction == 1) then
		messages[#messages].message = "animation " .. pos.x + 48 .. " " .. pos.y - 36 .. " " .. spriteindexes[3]
	elseif (direction == 2) then
		messages[#messages].message = "animation " .. pos.x .. " " .. pos.y + 48 - 36 .. " " .. spriteindexes[4]
	elseif (direction == 3) then
		messages[#messages].message = "animation " .. pos.x - 48 .. " " .. pos.y - 36 .. " " .. spriteindexes[5]
	end
	messages[#messages].argKeys = {"fholdtime", "ground"}
	messages[#messages].argValsInt = {15, 0}
	messages[#messages].argValsStr = {"", "false"}
	
	messagecount = #messages
	do return end

	if (direction == 0) then
		messages[#messages+1] = levelMessage.new()
		messages[#messages].source = "player"
		messages[#messages].message = "particle " .. pos.x .. " " .. pos.y
		messages[#messages].argKeys = {"velocity", "elength", "flength", "text"}
		messages[#messages].argValsInt = {0, 1000, 1000, 0}
		messages[#messages].argValsStr = {"0;-100", "", "", "swooord"}
	elseif (direction == 1) then
		messages[#messages+1] = levelMessage.new()
		messages[#messages].source = "player"
		messages[#messages].message = "particle " .. pos.x .. " " .. pos.y
		messages[#messages].argKeys = {"velocity", "elength", "flength", "text"}
		messages[#messages].argValsInt = {0, 1000, 1000, 0}
		messages[#messages].argValsStr = {"100;0", "", "", "swooord"}
	elseif (direction == 2) then
		messages[#messages+1] = levelMessage.new()
		messages[#messages].source = "player"
		messages[#messages].message = "particle " .. pos.x .. " " .. pos.y
		messages[#messages].argKeys = {"velocity", "elength", "flength", "text"}
		messages[#messages].argValsInt = {0, 1000, 1000, 0}
		messages[#messages].argValsStr = {"0;100", "", "", "swooord"}
	elseif (direction == 3) then
		messages[#messages+1] = levelMessage.new()
		messages[#messages].source = "player"
		messages[#messages].message = "particle " .. pos.x .. " " .. pos.y
		messages[#messages].argKeys = {"velocity", "elength", "flength", "text"}
		messages[#messages].argValsInt = {0, 1000, 1000, 0}
		messages[#messages].argValsStr = {"-100;0", "", "", "swooord"}
	end
	
	messagecount = #messages
	direction = -1
end

function update(pos, direction, ld, curTime)

end

function get_message(index)
	return messages[index+1]
end

function clear_messages()
	messages = {}
end


