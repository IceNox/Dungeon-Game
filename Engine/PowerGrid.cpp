#include "PowerGrid.h"

PowerGrid::PowerGrid()
{
    wireGrid.clear();
    gates.clear();
    tilesPowered.clear();
}

void PowerGrid::set_dimensions(int width, int height)
{
    wireGrid.clear();
    gates.clear();
    tilesPowered.clear();

    this->width = width;
    this->height = height;

    for (int i = 0; i < width * height; i++) {
        wireGrid.push_back(WireTile());
        tilesPowered.push_back(false);
    }
}

void PowerGrid::set_wire(Pos2D pos, int type, bool powered)
{
    // Check if there are no same type wires in pos already
    for (unsigned i = 0; i < wireGrid[pos.index(width)].types.size(); i++) {
        if (wireGrid[pos.index(width)].types[i] == type) {
            return;
        }
    }

    wireGrid[pos.index(width)].types.push_back(type);
    wireGrid[pos.index(width)].powered.push_back(powered);
}

void PowerGrid::set_gate(Pos2D pos, GateType type, Direction facing, bool poweredL, bool poweredR, bool poweredB)
{
    // Check if there are no wires in pos already
    if (wireGrid[pos.index(width)].types.size() > 0) return;

    // Check if there are no gates in pos already
    for (unsigned i = 0; i < gates.size(); i++) {
        if (gates[i].pos == pos) {
            return;
        }
    }

    gates.push_back(GateTile(pos, type, facing, poweredL, poweredR, poweredB));
}

bool PowerGrid::is_powered(Pos2D pos) const
{
    int index = pos.index(width);

    for (unsigned i = 0; i < wireGrid[index].powered.size(); i++) {
        if (wireGrid[index].powered[i]) {
            return true;
        }
    }

    return false;
}

void PowerGrid::update(const std::vector<TileData>& td)
{
    for (unsigned i = 0; i < td.size(); i++) {
        if (td[i].powered) {
            tilesPowered[i] = true;
        }
    }

    int msElapsed = (maintime::now() - lastUpdate).get_duration(MILLISECONDS);
    if (msElapsed > MS_PER_TICK) {
        // Reset wire power
        for (unsigned i = 0; i < wireGrid.size(); i++) {
            for (unsigned j = 0; j < wireGrid[i].powered.size(); j++) {
                wireGrid[i].powered[j] = false;
            }
        }

        // Get which gates should send power this tick
        std::vector<int> indices;
        for (unsigned i = 0; i < gates.size(); i++) {
            if (gates[i].type == DIODE) {
                // Back powered
                if (gates[i].poweredB) {
                    indices.push_back(i);
                }
            }
            else if (gates[i].type == NOT) {
                // Back not powered
                if (!gates[i].poweredB) {
                    indices.push_back(i);
                }
            }
            else if (gates[i].type == OR) {
                // Right or Left powered
                if (gates[i].poweredL || gates[i].poweredR) {
                    indices.push_back(i);
                }
            }
            else if (gates[i].type == AND) {
                // Right and Left powered
                if (gates[i].poweredL && gates[i].poweredR) {
                    indices.push_back(i);
                }
            }
            else if (gates[i].type == XOR) {
                // Right or Left (bot not both) powered
                if (gates[i].poweredL || gates[i].poweredR &&
                    !(gates[i].poweredL && gates[i].poweredR)) {
                    indices.push_back(i);
                }
            }
        }

        // Send power from those gates
        for (unsigned i = 0; i < indices.size(); i++) {
            GateTile &gate = gates[indices[i]];

            gate.poweredL = false;
            gate.poweredR = false;
            gate.poweredB = false;

            power(gate.pos.index(width, gate.facing, 1), gate.facing);
        }

        // Send power from powered tiles
        for (unsigned i = 0; i < td.size(); i++) {
            if (td[i].powered) {
                power(i, NO_DIRECTION);
            }
        }

        // Set variables
        msElapsed -= MS_PER_TICK;
        lastUpdate += Duration(MS_PER_TICK);

        for (unsigned i = 0; i < tilesPowered.size(); i++) {
            tilesPowered[i] = false;
        }
    }
}

void PowerGrid::power(int index, Direction dir, int type)
{
    Pos2D indexPos = Pos2D(index % width, index / width);
    int fromDir = (dir == NO_DIRECTION) ? -1 : ((dir + 2) % 4); // Flipped dir

    // Check for gate
    for (unsigned i = 0; i < gates.size(); i++) {
        if (gates[i].pos.index(width) != index) continue;

        if (dir == NO_DIRECTION) return;

        // Evaluate power direction
        if      ((gates[i].facing + 1) % 4 == fromDir) gates[i].poweredR = true;
        else if ((gates[i].facing + 2) % 4 == fromDir) gates[i].poweredB = true;
        else if ((gates[i].facing + 3) % 4 == fromDir) gates[i].poweredL = true;

        return;
    }

    // If type == -1 power all wire types
    if (type == -1) {
        for (unsigned i = 0; i < wireGrid[index].types.size(); i++) {
            if (wireGrid[index].powered[i]) continue;

            wireGrid[index].powered[i] = true;

            // Power all 4 directions (except input)
            for (int count = 0; count < 4; count++) {
                Direction powerDir = static_cast<Direction>(count);
                if (powerDir == fromDir) continue;

                // Recursively power adjacent tiles
                power(indexPos.index(width, powerDir, 1), powerDir, wireGrid[index].types[i]);
            }
        }

        return;
    }
    else {
        for (unsigned i = 0; i < wireGrid[index].types.size(); i++) {
            if (wireGrid[index].powered[i] || wireGrid[index].types[i] != type) continue;

            wireGrid[index].powered[i] = true;

            // Power all 4 directions (except input)
            for (int count = 0; count < 4; count++) {
                Direction powerDir = static_cast<Direction>(count);
                if (powerDir == fromDir) continue;

                // Recursively power adjacent tiles
                power(indexPos.index(width, powerDir, 1), powerDir, wireGrid[index].types[i]);
            }

            return;
        }
    }
}
