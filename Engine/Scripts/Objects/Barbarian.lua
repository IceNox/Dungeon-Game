-- Identification
ID = 201
VARIANT = 0
NAME = "Barbarian"

-- Sprites to load
spritecount = 14
spritepaths =
{
	"Content/Game/Mobs/Barbarian/Barbarian_attack_left_1.tga", 	-- 1
	"Content/Game/Mobs/Barbarian/Barbarian_attack_right_1.tga", -- 2
	"Content/Game/Mobs/Barbarian/Barbarian_idle_left_1.tga", 	-- 3
	"Content/Game/Mobs/Barbarian/Barbarian_idle_left_2.tga", 	-- 4
	"Content/Game/Mobs/Barbarian/Barbarian_idle_right_1.tga", 	-- 5
	"Content/Game/Mobs/Barbarian/Barbarian_idle_right_2.tga", 	-- 6
	"Content/Game/Mobs/Barbarian/Barbarian_jump_left_1.tga", 	-- 7
	"Content/Game/Mobs/Barbarian/Barbarian_jump_left_2.tga", 	-- 8
	"Content/Game/Mobs/Barbarian/Barbarian_jump_right_1.tga", 	-- 9
	"Content/Game/Mobs/Barbarian/Barbarian_jump_right_2.tga", 	-- 10
	"Content/Game/Animations/Attack/Attack_chop_up_animation-f07.tga",	  -- 11
	"Content/Game/Animations/Attack/Attack_chop_right_animation-f07.tga", -- 12
	"Content/Game/Animations/Attack/Attack_chop_down_animation-f07.tga",  -- 13
	"Content/Game/Animations/Attack/Attack_chop_left_animation-f07.tga"   -- 14
}
spritexcenters =
{
	36, -- 1
	36, -- 2
	36, -- 3
	36, -- 4
	36, -- 5
	36, -- 6
	36, -- 7
	36, -- 8
	36, -- 9
	36, -- 10
	4,  -- 11
	12, -- 12
	4,  -- 13
	12  -- 14
}
spriteycenters =
{
	88, -- 1
	88, -- 2
	88, -- 3
	88, -- 4
	88, -- 5
	88, -- 6
	88, -- 7
	88, -- 8
	88, -- 9
	88, -- 10
	36, -- 11
	36, -- 12
	36, -- 13
	36  -- 14
}
spriteindexes = {}

-- Health
maxhealth = 4
health = 4
armor = 0

healthbarheight = 92
healthbarvisible = false

-- Hitbox
hitbox = 
{
	shape = 0,
	size = { w = 0, h = 0, r = 8 },
	offset = { x = 0, y = 0 }
}

-- Other
obstructive = 1
pressuring = 1
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

-- Cooldowns
actioncooldown_d = 1000
attackcooldown_d = 1000

lastaction_t = 0
lastattack_t = 0

-- Attack variables
attackdur_d = 250
attackstart_t = 0
attacking = false
attackdir = -1
attackpos = position.new()

function update(pos, ld, curtime_t)
	gpos = position.new()
	gpos.x = math.floor(pos.x / 96)
	gpos.y = math.floor(pos.y / 96)

	-- Check for activation by light
	if (not(active)) then
		if (ld.tiles[gpos.y * ld.width + gpos.x + 1].directlyLit == 1) then
			active = true
		end
	end
	
	-- Return if inactive
	if (not(active)) then return end

	-- Evaluate action
	if (not(moving) and not(attacking)) then
		if (curtime_t - lastaction_t > actioncooldown_d) then 
			-- Find closest player
			local ppos = position.new()
			local dist = 1000
			
			for i = 1, ld.playercount, 1 do
				d = math.abs(gpos.x - ld.playerx[i]) + math.abs(gpos.y - ld.playery[i])
				if (d < dist) then
					ppos.x = ld.playerx[i]
					ppos.y = ld.playery[i]
					dist = d
				end
			end
			
			-- Get unit coordinate difference
			local dx = 0
			local dy = 0
			
			if (ppos.x ~= gpos.x) then
				dx = math.floor((ppos.x - gpos.x) / math.abs(ppos.x - gpos.x))
			else
				dx = 0
			end
			
			if (ppos.y ~= gpos.y) then
				dy = math.floor((ppos.y - gpos.y) / math.abs(ppos.y - gpos.y))
			else
				dy = 0
			end
			
			-- Start Attack
			if (dist == 1) then
				if (curtime_t - lastattack_t > attackcooldown_d) then
					attackpos.x = ppos.x
					attackpos.y = ppos.y
				
					-- Evaluate attack direction
					if 	   (dy == -1) then attackdir = 0
					elseif (dx ==  1) then attackdir = 1
					elseif (dy ==  1) then attackdir = 2
					elseif (dx == -1) then attackdir = 3
					else attackdir = -1 end
					
					-- Start attack
					if (attackdir ~= -1) then
						attacking = true
						lastattack_t = curtime_t
						lastaction_t = curtime_t
						attackstart_t = curtime_t
					end
				end
				
			-- Start movement
			elseif (dist > 1) then
				-- Check for walls
				local dx_ = dx
				local dy_ = dy
				
				local xindex = gpos.y * ld.width + (gpos.x + dx_) + 1
				local yindex = (gpos.y + dy_) * ld.width + gpos.x + 1
				
				
				if (ld.tiles[xindex].occupied == 1) then dx_ = 0 end
				if (ld.tiles[yindex].occupied == 1) then dy_ = 0 end
				
				-- Prioritize horizontal movement
				if (dx_ ~= 0) then
					dy_ = 0
				end
				
				if (not(dx_ == 0 and dy_ == 0)) then
					-- Set up variables
					startmovement = true
					movement.relative = true
					movement.duration = 150
					movement.x = dx_
					movement.y = dy_
					
					lastaction_t = curtime_t
				end
			end
		end
	end
	
	-- Update attack
	if (attacking) then
		local mselapsed_d = curtime_t - attackstart_t
		
		-- Finish attack if necessary
		if (mselapsed_d > attackdur_d) then
			-- Deal damage
			messages[#messages+1] = levelMessage.new()
			messages[#messages].source = "barbarian"
			messages[#messages].message = "damage " .. attackpos.x .. " " .. attackpos.y .. " 1 p " .. dirstr[attackdir]
			
			-- Show swing animation
			messages[#messages+1] = levelMessage.new()
			messages[#messages].source = "barbarian"
			if (attackdir == 0) then
				messages[#messages].message = "animation " .. pos.x .. " " .. pos.y - 48 - 36 .. " " .. spriteindexes[11]
			elseif (attackdir == 1) then
				messages[#messages].message = "animation " .. pos.x + 48 .. " " .. pos.y - 36 .. " " .. spriteindexes[12]
			elseif (attackdir == 2) then
				messages[#messages].message = "animation " .. pos.x .. " " .. pos.y + 48 - 36 .. " " .. spriteindexes[13]
			elseif (attackdir == 3) then
				messages[#messages].message = "animation " .. pos.x - 48 .. " " .. pos.y - 36 .. " " .. spriteindexes[14]
			end
			messages[#messages].argKeys = {"fholdtime", "ground"}
			messages[#messages].argValsInt = {15, 0}
			messages[#messages].argValsStr = {"", "false"}
			
			attacking = false
		end
	end
	
	messagecount = #messages
end

function damage(di)
	if (di.enemies == true) then
		health = health - di.amount
		healthbarvisible = true
		
		if (health <= 0) then
			health = 0
			healthbarvisible = false
			
			--messages[#messages+1] = levelMessage.new()
			
			return true
		end
		
		return false
	end
end

idleanimationtime_d = 500 -- Cooldown between idle animation changes in ms
function select_sprite(curtime_t)
	if (not(moving) and not(attacking)) then
		mselapsed_d = curtime_t - lastaction_t
		
		if (math.floor(mselapsed_d / idleanimationtime_d) % 2 == 0) then
			if (facing == 1) then
				return spriteindexes[5]
			elseif (facing == 3) then
				return spriteindexes[3]
			end
		else
			if (facing == 1) then
				return spriteindexes[6]
			elseif (facing == 3) then
				return spriteindexes[4]
			end
		end
	elseif (moving) then
		if (moveprogress < 0.5) then
			if (facing == 1) then
				return spriteindexes[9]
			elseif (facing == 3) then
				return spriteindexes[7]
			end
		else
			if (facing == 1) then
				return spriteindexes[10]
			elseif (facing == 3) then
				return spriteindexes[8]
			end
		end
	elseif (attacking) then
		if (facing == 1) then
			return spriteindexes[2]
		elseif (facing == 3) then
			return spriteindexes[1]
		end
	end
end

function get_message(index)
	return messages[index+1]
end

function clear_messages()
	messages = {}
	messagecount = 0
end







