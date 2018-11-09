#include "StatusEffects.h"

#include "GlobalData.h"
#include "Constants.h"

using namespace std::chrono;

StatusEffects::StatusEffects()
{
    slowed  = false;
    snared    = false;
    stunned = false;
    burning = false;
    cursed    = false;

     slowDuration = 0;
    snareDuration = 0;
     stunDuration = 0;
     burnDuration = 0;
    curseDuration = 0;

     burnStrength = 0;
     slowStrength = 0.0f;
    curseStrength = 0;

     slowStart = maintime::now();
    snareStart = maintime::now();
     stunStart = maintime::now();
    curseStart = maintime::now();

    burnLastTick = maintime::now();
    burnTicksRem = 0;
}

void StatusEffects::update_effects()
{
    // Slow
    if (slowed) {
        Duration time_elapsed = maintime::now() - slowStart;

        if (time_elapsed.get_duration(MILLISECONDS) > slowDuration) {
            stop_slow();
        }
    }

    // Snare
    if (snared) {
        Duration time_elapsed = maintime::now() - snareStart;

        if (time_elapsed.get_duration(MILLISECONDS) > snareDuration) {
            stop_snare();
        }
    }

    // Stun
    if (stunned) {
        Duration time_elapsed = maintime::now() - stunStart;

        if (time_elapsed.get_duration(MILLISECONDS) > stunDuration) {
            stop_stun();

            stunned = false;
        }
    }

    // Burn
    if (burning) {
        if (burnTicksRem <= 0) {
            stop_burn();
        }
        else {
            Duration time_elapsed = maintime::now() - burnLastTick;

            if (time_elapsed.get_duration(MILLISECONDS) >= 1.0f) {
                fireDamage += burnStrength;

                burnLastTick = maintime::now();
                burnTicksRem--;
            }
        }
    }

    // Curse
    if (cursed) {
        Duration time_elapsed = maintime::now() - curseStart;

        if (time_elapsed.get_duration(MILLISECONDS) > stunDuration) {
            stop_curse();
        }
    }
}

void StatusEffects::slow(int duration, float strength)
{
    // Calculate time
    Duration time_elapsed   = maintime::now() - slowStart;
    long int time_remaining = slowDuration - time_elapsed.get_duration(MILLISECONDS);

    // Prevent the text 'SLOWED' from being spammed
    if (time_elapsed.get_duration(MILLISECONDS) < 1000) return;

    // Evaluate powers
    float newPower = strength      * duration;
    float curPower = slowStrength * time_remaining / 1000;

    // Return if new slow is weaker
    if (newPower < curPower) return;
    
    // Apply slow
    slowed = true;

    if (strength >= 0.9f) strength = 0.9f;
    if (strength <= 0.1f) strength = 0.1f;

    slowStrength = 1.0f - strength;
    slowDuration = duration;

    slowStart = maintime::now();
}

void StatusEffects::snare(int duration)
{
    // Calculate time
    Duration time_elapsed    = maintime::now() - snareStart;
    long int time_remaining = snareDuration - time_elapsed.get_duration(MILLISECONDS);

    // Prevent the text 'SNARED' from being spammed
    if (time_elapsed.get_duration(MILLISECONDS) < 1000) return;

    // Return if new stun is weaker
    if (duration < time_remaining) return;

    // Apply snare
    snared = true;

    snareDuration = duration;

    snareStart = maintime::now();
}

void StatusEffects::stun(int duration)
{
    // Calculate time
    Duration time_elapsed    = maintime::now() - stunStart;
    long int time_remaining = stunDuration - time_elapsed.get_duration(MILLISECONDS);

    // Prevent the text 'STUNNED' from being spammed
    if (time_elapsed.get_duration(MILLISECONDS) < 1000) return;

    // Return if new stun is weaker
    if (duration < time_remaining) return;
    
    // Apply stun
    stunned    = true;

    stunDuration = duration;

    stunStart = maintime::now();
}

void StatusEffects::burn(int duration, int strength)
{
    // Evaluate powers
    float newPower = strength      * duration;
    float curPower = burnStrength * burnTicksRem;

    // Return if new burn is weaker
    if (newPower < curPower) return;
    
    // Apply burn
    burning = true;

    if (strength < 1) strength = 1;
    if (duration < 1) duration = 1;

    burnStrength = strength;
    burnDuration = duration;

    burnLastTick = maintime::now();
    burnTicksRem = duration;
}

void StatusEffects::curse(int duration, int strength)
{
    // Calculate time
    Duration time_elapsed    = maintime::now() - curseStart;
    long int time_remaining = curseDuration - time_elapsed.get_duration(MILLISECONDS);

    // Prevent the text 'CURSED' from being spammed
    if (time_elapsed.get_duration(MILLISECONDS) < 1000) return;

    // Return if new strength is weaker
    if (strength < curseStrength) return;

    // Apply curse
    cursed = true;

    if (strength < 1) strength = 1;
    if (duration < 0) duration = 0;

    curseStrength = strength;
    curseDuration = duration;

    curseStart = maintime::now();
}

void StatusEffects::stop_slow()
{
    slowed = false;
    slowStrength = -1.0f;
}

void StatusEffects::stop_snare()
{
    snared = false;
}

void StatusEffects::stop_stun()
{
    stunned = false;
}

void StatusEffects::stop_burn()
{
    burning = false;
    burnStrength = -1.0f;
}

void StatusEffects::stop_curse()
{
    cursed = false;
    curseStrength = -1;
}

void StatusEffects::stop_effects()
{
    stop_slow();
    stop_snare();
    stop_stun();
    stop_burn();
    stop_curse();
}