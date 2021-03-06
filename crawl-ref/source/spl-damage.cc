/**
 * @file
 * @brief Damage-dealing spells not already handled elsewhere.
 *           Other targeted spells are covered in spl-zap.cc.
**/

#include "AppHdr.h"

#include "spl-damage.h"
#include "externs.h"

#include "areas.h"
#include "beam.h"
#include "cloud.h"
#include "coord.h"
#include "coordit.h"
#include "directn.h"
#include "env.h"
#include "food.h"
#include "fprop.h"
#include "godconduct.h"
#include "itemprop.h"
#include "items.h"
#include "libutil.h"
#include "message.h"
#include "misc.h"
#include "mon-behv.h"
#include "mon-iter.h"
#include "mon-stuff.h"
#include "mon-util.h"
#include "ouch.h"
#include "player.h"
#include "player-equip.h"
#include "shout.h"
#include "spl-util.h"
#include "stuff.h"
#include "terrain.h"
#include "transform.h"
#include "traps.h"
#include "view.h"
#include "viewchar.h"


bool fireball(int pow, bolt &beam)
{
    return (zapping(ZAP_FIREBALL, pow, beam, true));
}

void setup_fire_storm(const actor *source, int pow, bolt &beam)
{
    beam.name         = "great blast of fire";
    beam.ex_size      = 3;
    beam.flavour      = BEAM_LAVA;
    beam.real_flavour = beam.flavour;
    beam.colour       = RED;
    beam.glyph        = dchar_glyph(DCHAR_FIRED_ZAP);
    beam.beam_source  = source->mindex();
    // XXX: Should this be KILL_MON_MISSILE?
    beam.thrower      =
        source->atype() == ACT_PLAYER ? KILL_YOU_MISSILE : KILL_MON;
    beam.aux_source.clear();
    beam.obvious_effect = false;
    beam.is_beam      = false;
    beam.is_tracer    = true;
    beam.is_explosion = true;
    beam.ench_power   = pow;      // used for radius
    beam.hit          = 20 + pow / 10;
    beam.damage       = calc_dice(8, 5 + pow);
}

bool cast_fire_storm(int pow, bolt &beam)
{
    if (grid_distance(beam.target, beam.source) > beam.range)
        return (false);

    setup_fire_storm(&you, pow, beam);
    
    beam.explode(false);
    
    if(beam.beam_cancelled)
    {
        //Player aborted the casting.
        canned_msg(MSG_OK);
        return(false);
    }
    
    //Reset the parameters.  beam.ex_size was 3 before to make
    //*absolutely* sure the player doesn't hit him/herself;
    //Fire Storm *hurts*.
    beam.ex_size            = 2 + (random2(1000) < pow);
    beam.is_tracer          = false;
    //Failure to reset in_explosion_phase means failing an assert.
    beam.in_explosion_phase = false;

    mpr("A raging storm of fire appears!");

    beam.explode(false);

    viewwindow();
    return (true);
}

// No setup/cast split here as monster hellfire is completely different.
// Sad, but needed to maintain balance - monster hellfirers get asymmetric
// torment too.
bool cast_hellfire_burst(int pow, bolt &beam)
{
    beam.name              = "burst of hellfire";
    beam.aux_source        = "burst of hellfire";
    beam.ex_size           = 1;
    beam.flavour           = BEAM_HELLFIRE;
    beam.real_flavour      = beam.flavour;
    beam.glyph             = dchar_glyph(DCHAR_FIRED_BURST);
    beam.colour            = RED;
    beam.beam_source       = MHITYOU;
    beam.thrower           = KILL_YOU;
    beam.obvious_effect    = false;
    beam.is_beam           = false;
    beam.is_explosion      = true;
    beam.ench_power        = pow;      // used for radius
    beam.hit               = 20 + pow / 10;
    beam.damage            = calc_dice(6, 30 + pow);
    beam.can_see_invis     = you.can_see_invisible();
    beam.smart_monster     = true;
    beam.attitude          = ATT_FRIENDLY;
    beam.friend_info.count = 0;
    beam.is_tracer         = true;

    beam.explode(false);

    if (beam.beam_cancelled)
    {
        canned_msg(MSG_OK);
        return (false);
    }

    mpr("You call forth a pillar of hellfire!");

    beam.is_tracer = false;
    beam.in_explosion_phase = false;
    beam.explode(true);

    return (true);
}

static bool _lightning_los(const coord_def& source, const coord_def& target)
{
    // XXX: currently bounded by square LOS radius;
    // XXX: adapt opacity -- allow passing clouds.
    return (exists_ray(source, target, opc_solid,
                       circle_def(LOS_MAX_RADIUS, C_SQUARE)));
}

void cast_chain_lightning(int pow, const actor *caster)
{
    bolt beam;

    // initialise beam structure
    beam.name           = "lightning arc";
    beam.aux_source     = "chain lightning";
    beam.beam_source    = caster->mindex();
    beam.thrower        = (caster == &you) ? KILL_YOU_MISSILE : KILL_MON_MISSILE;
    beam.range          = 8;
    beam.hit            = AUTOMATIC_HIT;
    beam.glyph          = dchar_glyph(DCHAR_FIRED_ZAP);
    beam.flavour        = BEAM_ELECTRICITY;
    beam.obvious_effect = true;
    beam.is_beam        = false;       // since we want to stop at our target
    beam.is_explosion   = false;
    beam.is_tracer      = false;

    if (const monster* mons = caster->as_monster())
        beam.source_name = mons->name(DESC_PLAIN, true);

    bool first = true;
    coord_def source, target;

    for (source = caster->pos(); pow > 0;
         pow -= 8 + random2(13), source = target)
    {
        // infinity as far as this spell is concerned
        // (Range - 1) is used because the distance is randomised and
        // may be shifted by one.
        int min_dist = MONSTER_LOS_RANGE - 1;

        int dist;
        int count = 0;

        target.x = -1;
        target.y = -1;

        for (monster_iterator mi; mi; ++mi)
        {
            if (invalid_monster(*mi))
                continue;

            dist = grid_distance(source, mi->pos());

            // check for the source of this arc
            if (!dist)
                continue;

            // randomise distance (arcs don't care about a couple of feet)
            dist += (random2(3) - 1);

            // always ignore targets further than current one
            if (dist > min_dist)
                continue;

            if (!_lightning_los(source, mi->pos()))
                continue;

            count++;

            if (dist < min_dist)
            {
                // switch to looking for closer targets (but not always)
                if (!one_chance_in(10))
                {
                    min_dist = dist;
                    target = mi->pos();
                    count = 0;
                }
            }
            else if (target.x == -1 || one_chance_in(count))
            {
                // either first target, or new selected target at min_dist
                target = mi->pos();

                // need to set min_dist for first target case
                dist = std::max(dist, min_dist);
            }
        }

        // now check if the player is a target
        dist = grid_distance(source, you.pos());

        if (dist)       // i.e., player was not the source
        {
            // distance randomised (as above)
            dist += (random2(3) - 1);

            // select player if only, closest, or randomly selected
            if ((target.x == -1
                    || dist < min_dist
                    || (dist == min_dist && one_chance_in(count + 1)))
                && _lightning_los(source, you.pos()))
            {
                target = you.pos();
            }
        }

        const bool see_source = you.see_cell(source);
        const bool see_targ   = you.see_cell(target);

        if (target.x == -1)
        {
            if (see_source)
                mpr("The lightning grounds out.");

            break;
        }

        // Trying to limit message spamming here so we'll only mention
        // the thunder at the start or when it's out of LoS.
        const char* msg = "You hear a mighty clap of thunder!";
        noisy(25, source, (first || !see_source) ? msg : NULL);
        first = false;

        if (see_source && !see_targ)
            mpr("The lightning arcs out of your line of sight!");
        else if (!see_source && see_targ)
            mpr("The lightning arc suddenly appears!");

        if (!you.see_cell_no_trans(target))
        {
            // It's no longer in the caster's LOS and influence.
            pow = pow / 2 + 1;
        }

        beam.source = source;
        beam.target = target;
        beam.colour = LIGHTBLUE;
        beam.damage = calc_dice(5, 12 + pow * 2 / 3);

        // Be kinder to the caster.
        if (target == caster->pos())
        {
            if (!(beam.damage.num /= 2))
                beam.damage.num = 1;
            if ((beam.damage.size /= 2) < 3)
                beam.damage.size = 3;
        }
        beam.fire();
    }

    more();
}

// Poisonous light passes right through invisible players
// and monsters, and so, they are unaffected by this spell --
// assumes only you can cast this spell (or would want to).
void cast_toxic_radiance(bool non_player)
{
    if (non_player)
        mpr("The air is filled with a sickly green light!");
    else
        mpr("You radiate a sickly green light!");

    flash_view(GREEN);
    more();
    mesclr();

    // Determine whether the player is hit by the radiance. {dlb}
    if (you.duration[DUR_INVIS])
    {
        mpr("The light passes straight through your body.");
    }
    else
    {
        int poison_amount = poison_player(2, "", "toxic radiance");
        if (poison_amount)
            mpr("You feel rather sick.");
    }

    counted_monster_list affected_monsters;
    // determine which monsters are hit by the radiance: {dlb}
    for (monster_iterator mi(you.get_los()); mi; ++mi)
    {
        if (!mi->submerged())
        {
            // Monsters affected by corona are still invisible in that
            // radiation passes through them without affecting them. Therefore,
            // this check should not be !mons->invisible().
            if (!mi->has_ench(ENCH_INVIS))
            {
                const actor* agent = non_player ? 0 : &you;
                bool affected = poison_monster(*mi, agent, 1, false, false);

                if (coinflip() && poison_monster(*mi, agent, 1, false, false))
                    affected = true;

                if (affected)
                    affected_monsters.add(*mi);
            }
            else if (you.can_see_invisible())
            {
                // message player re:"miss" where appropriate {dlb}
                mprf("The light passes through %s.",
                     mi->name(DESC_NOCAP_THE).c_str());
            }
        }
    }

    if (!affected_monsters.empty())
    {
        const std::string message =
            make_stringf("%s %s poisoned.",
                         affected_monsters.describe().c_str(),
                         affected_monsters.count() == 1? "is" : "are");
        if (strwidth(message) < get_number_of_cols() - 2)
            mpr(message.c_str());
        else
        {
            // Exclamation mark to suggest that a lot of creatures were
            // affected.
            if (non_player)
                mpr("Nearby monsters are poisoned!");
            else
                mpr("The monsters around you are poisoned!");
        }
    }
}

bool cast_refrigeration(int pow, bool non_player, bool freeze_potions)
{
    {
        targetter_los hitfunc(&you);
        if (stop_attack_prompt(hitfunc, "harm"))
            return false;
    }

    if (non_player)
        mpr("Something drains the heat from around you.");
    else
        mpr("The heat is drained from your surroundings.");

    flash_view(LIGHTCYAN);
    more();
    mesclr();

    // Handle the player.
    const dice_def dam_dice(3, 5 + pow / 10);
    const int hurted = check_your_resists(dam_dice.roll(), BEAM_COLD,
            "refrigeration");

    if (hurted > 0)
    {
        mpr("You feel very cold.");
        ouch(hurted, NON_MONSTER, KILLED_BY_FREEZING);

        // Note: this used to be 12!... and it was also applied even if
        // the player didn't take damage from the cold, so we're being
        // a lot nicer now.  -- bwr
        if (freeze_potions)
            expose_player_to_element(BEAM_COLD, 5);
    }

    // Now do the monsters.

    // First build the message.
    counted_monster_list affected_monsters;

    for (monster_iterator mi(&you); mi; ++mi)
        if (cell_see_cell(you.pos(), mi->pos())) // not just you.can_see (Scry)
            affected_monsters.add(*mi);

    if (!affected_monsters.empty())
    {
        const std::string message =
            make_stringf("%s %s frozen.",
                         affected_monsters.describe().c_str(),
                         affected_monsters.count() == 1? "is" : "are");
        if (strwidth(message) < get_number_of_cols() - 2)
            mpr(message.c_str());
        else
        {
            // Exclamation mark to suggest that a lot of creatures were
            // affected.
            mpr("The monsters around you are frozen!");
        }
    }

    // Now damage the creatures.

    // Set up the cold attack.
    bolt beam;
    beam.flavour = BEAM_COLD;
    beam.thrower = KILL_YOU;

    for (monster_iterator mi(you.get_los()); mi; ++mi)
    {
        // Note that we *do* hurt monsters which you can't see
        // (submerged, invisible) even though you get no information
        // about it.

        // ... but not ones you see only via Scrying.
        if (!cell_see_cell(you.pos(), mi->pos()))
            continue;
        // Calculate damage and apply.
        int hurt = mons_adjust_flavoured(*mi, beam, dam_dice.roll());
        dprf("damage done: %d", hurt);
        if (non_player)
            mi->hurt(NULL, hurt, BEAM_COLD);
        else
            mi->hurt(&you, hurt, BEAM_COLD);

        if (!non_player && (is_sanctuary(you.pos()) || is_sanctuary(mi->pos())))
            remove_sanctuary(true);

        // Cold-blooded creatures can be slowed.
        if (mi->alive()
            && mons_class_flag(mi->type, M_COLD_BLOOD)
            && coinflip())
        {
            mi->add_ench(ENCH_SLOW);
        }
    }
    
    return true;
}

bool vampiric_drain(int pow, monster* mons)
{
    if (mons == NULL || mons->submerged())
    {
        canned_msg(MSG_NOTHING_CLOSE_ENOUGH);
        // Cost to disallow freely locating invisible/submerged
        // monsters.
        return (true);
    }

    if (mons->observable() && mons->undead_or_demonic())
    {
        mpr("Draining that being is not a good idea.");
        return (false);
    }

    god_conduct_trigger conducts[3];
    disable_attack_conducts(conducts);

    const bool success = !stop_attack_prompt(mons, false, you.pos());

    if (success)
    {
        set_attack_conducts(conducts, mons);

        behaviour_event(mons, ME_WHACK, MHITYOU, you.pos());
    }

    enable_attack_conducts(conducts);

    if (!success)
        return (false);

    if (!mons->alive())
    {
        canned_msg(MSG_NOTHING_HAPPENS);
        return (true);
    }

    // Monster might be invisible or player misled.
    if (mons->undead_or_demonic())
    {
        mpr("Aaaarggghhhhh!");
        dec_hp(random2avg(39, 2) + 10, false, "vampiric drain backlash");
        return (true);
    }

    if (mons->holiness() != MH_NATURAL
        || mons->res_negative_energy())
    {
        canned_msg(MSG_NOTHING_HAPPENS);
        return (true);
    }

    // The practical maximum of this is about 25 (pow @ 100). - bwr
    int hp_gain = 3 + random2avg(9, 2) + random2(pow) / 7;

    hp_gain = std::min(mons->hit_points, hp_gain);
    hp_gain = std::min(you.hp_max - you.hp, hp_gain);

    if (!hp_gain)
    {
        canned_msg(MSG_NOTHING_HAPPENS);
        return (true);
    }

    const bool mons_was_summoned = mons->is_summoned();

    mons->hurt(&you, hp_gain);

    if (mons->alive())
        print_wounds(mons);

    hp_gain /= 2;

    if (hp_gain && !mons_was_summoned)
    {
        if(you.challenge != CHALLENGE_VEHUMET)
        {
            mpr("You feel life coursing into your body.");
            inc_hp(hp_gain, false);
        }
        else
            mpr("For some reason, you fail to gain life from its injuries.");
    }

    return (true);
}

bool cast_freeze(int pow, monster* mons)
{
    pow = std::min(25, pow);

    if (mons == NULL || mons->submerged())
    {
        canned_msg(MSG_NOTHING_CLOSE_ENOUGH);
        // If there's no monster there, you still pay the costs in
        // order to prevent locating invisible/submerged monsters.
        return (true);
    }

    god_conduct_trigger conducts[3];
    disable_attack_conducts(conducts);

    const bool success = !stop_attack_prompt(mons, false, you.pos());

    if (success)
    {
        set_attack_conducts(conducts, mons);

        mprf("You freeze %s.", mons->name(DESC_NOCAP_THE).c_str());

        behaviour_event(mons, ME_ANNOY, MHITYOU);
    }

    enable_attack_conducts(conducts);

    if (!success)
        return (false);

    bolt beam;
    beam.flavour = BEAM_COLD;
    beam.thrower = KILL_YOU;

    const int orig_hurted = roll_dice(1, 3 + pow / 3);
    int hurted = mons_adjust_flavoured(mons, beam, orig_hurted);
    mons->hurt(&you, hurted);

    if (mons->alive())
    {
        mons->expose_to_element(BEAM_COLD, orig_hurted);
        print_wounds(mons);

        const int cold_res = mons->res_cold();
        if (cold_res <= 0)
        {
            const int stun = (1 - cold_res) * random2(2 + pow/5);
            mons->speed_increment -= stun;
        }
    }

    return (true);
}

bool cast_airstrike(int pow, const dist &beam)
{
    monster* mons = monster_at(beam.target);
    if (mons && (mons->submerged() ||
                 (cell_is_solid(beam.target) && mons_wall_shielded(mons))))
        mons = NULL;

    if (mons == NULL)
    {
        canned_msg(MSG_SPELL_FIZZLES);
        return true; // still losing a turn
    }

    if (mons->res_wind() > 0)
    {
        if (mons->observable())
        {
            mprf("But air would do no harm to %s!",
                 mons->name(DESC_NOCAP_THE).c_str());
            return false;
        }
        mprf("The air twists arounds and harmlessly tosses %s around.",
             mons->name(DESC_NOCAP_THE).c_str());
        // Bailing out early, no need to upset the gods or the target.
        return true; // you still did discover the invisible monster
    }

    god_conduct_trigger conducts[3];
    disable_attack_conducts(conducts);

    if (stop_attack_prompt(mons, false, you.pos()))
        return false;

    set_attack_conducts(conducts, mons);

    mprf("The air twists around and strikes %s!",
         mons->name(DESC_NOCAP_THE).c_str());
    noisy(4, beam.target);

    behaviour_event(mons, ME_ANNOY, MHITYOU);
    if (mons_is_mimic(mons->type))
        mimic_alert(mons);

    enable_attack_conducts(conducts);

    int hurted = 8 + random2(random2(4) + (random2(pow) / 6)
                   + (random2(pow) / 7));

    bolt pbeam;
    pbeam.flavour = BEAM_AIR;
    hurted = mons->beam_resists(pbeam, hurted, false);
    // perhaps we should let the beam subtract AC and do damage too?

    hurted -= random2(1 + mons->armour_class());

    hurted = std::max(0, hurted);

    mons->hurt(&you, hurted);
    if (mons->alive())
        print_wounds(mons);

    return true;
}

// Just to avoid typing this over and over.
// Returns true if monster died. -- bwr
static bool _player_hurt_monster(monster& m, int damage,
                                 beam_type flavour = BEAM_MISSILE)
{
    if (damage > 0)
    {
        m.hurt(&you, damage, flavour, false);

        if (m.alive())
        {
            print_wounds(&m);
            behaviour_event(&m, ME_WHACK, MHITYOU);
        }
        else
        {
            monster_die(&m, KILL_YOU, NON_MONSTER);
            return (true);
        }
    }

    return (false);
}

// Here begin the actual spells:
static int _shatter_mon_dice(const monster *mon)
{
    if (!mon)
        return 0;

    // Removed a lot of silly monsters down here... people, just because
    // it says ice, rock, or iron in the name doesn't mean it's actually
    // made out of the substance. - bwr
    switch (mon->type)
    {
    // Double damage to stone, metal and crystal.
    case MONS_EARTH_ELEMENTAL:
    case MONS_CLAY_GOLEM:
    case MONS_STONE_GOLEM:
    case MONS_STATUE:
    case MONS_GARGOYLE:
    case MONS_IRON_ELEMENTAL:
    case MONS_IRON_GOLEM:
    case MONS_METAL_GARGOYLE:
    case MONS_CRYSTAL_GOLEM:
    case MONS_SILVER_STATUE:
    case MONS_ORANGE_STATUE:
    case MONS_ROXANNE:
        return 6;

    // 1/3 damage to liquids.
    case MONS_JELLYFISH:
    case MONS_WATER_ELEMENTAL:
        return 1;

    default:
        const bool petrifying = mon->petrifying();
        const bool petrified = mon->petrified() && !petrifying;

        // Extra damage to petrifying/petrified things.
        if (petrifying || petrified)
            return petrifying ? 4 : 6;
        // No damage to insubstantials.
        else if (mon->is_insubstantial())
            return 0;
        // 1/3 damage to fliers and slimes.
        else if (mons_flies(mon) || mons_is_slime(mon))
            return 1;
        // 3/2 damage to ice.
        else if (mon->is_icy())
            return 4;
        // Double damage to bone.
        else if (mon->is_skeletal())
            return 6;
        // Normal damage to everything else.
        else
            return 3;
    }
}

static int _shatter_monsters(coord_def where, int pow, int, actor *)
{
    dice_def dam_dice(0, 5 + pow / 3); // Number of dice set below.
    monster* mon = monster_at(where);

    if (mon == NULL)
        return (0);

    dam_dice.num = _shatter_mon_dice(mon);

    int damage = std::max(0, dam_dice.roll() - random2(mon->armour_class()));

    if (damage > 0)
    {
        _player_hurt_monster(*mon, damage);

        if (is_sanctuary(you.pos()) || is_sanctuary(mon->pos()))
            remove_sanctuary(true);
    }

    return (damage);
}

static int _shatter_items(coord_def where, int pow, int, actor *)
{
    UNUSED(pow);

    int broke_stuff = 0;

    for (stack_iterator si(where); si; ++si)
    {
        if (si->base_type == OBJ_POTIONS && !one_chance_in(10))
        {
            broke_stuff++;
            destroy_item(si->index());
        }
    }

    if (broke_stuff)
    {
        if (player_can_hear(where))
            mpr("You hear glass break.", MSGCH_SOUND);

        return 1;
    }

    return 0;
}

static int _shatter_walls(coord_def where, int pow, int, actor *)
{
    int chance = 0;

    // if not in-bounds then we can't really shatter it -- bwr
    if (!in_bounds(where))
        return 0;

    if (env.markers.property_at(where, MAT_ANY, "veto_shatter") == "veto")
        return 0;

    const dungeon_feature_type grid = grd(where);

    switch (grid)
    {
    case DNGN_SECRET_DOOR:
        if (you.see_cell(where))
            mpr("A secret door shatters!");
        chance = 100;
        break;

    case DNGN_CLOSED_DOOR:
    case DNGN_DETECTED_SECRET_DOOR:
    case DNGN_OPEN_DOOR:
        if (you.see_cell(where))
            mpr("A door shatters!");
        chance = 100;
        break;

    case DNGN_GRATE:
        if (you.see_cell(where))
            mpr("An iron grate is ripped into pieces!");
        chance = 100;
        break;

    case DNGN_METAL_WALL:
        chance = pow / 10;
        break;

    case DNGN_ORCISH_IDOL:
    case DNGN_GRANITE_STATUE:
        chance = 50;
        break;

    case DNGN_CLEAR_STONE_WALL:
    case DNGN_STONE_WALL:
        chance = pow / 6;
        break;

    case DNGN_CLEAR_ROCK_WALL:
    case DNGN_ROCK_WALL:
    case DNGN_SLIMY_WALL:
        chance = pow / 4;
        break;

    case DNGN_GREEN_CRYSTAL_WALL:
        chance = 50;
        break;

    default:
        break;
    }

    if (x_chance_in_y(chance, 100))
    {
        noisy(30, where);

        grd(where) = DNGN_FLOOR;
        set_terrain_changed(where);

        if (grid == DNGN_ORCISH_IDOL)
            did_god_conduct(DID_DESTROY_ORCISH_IDOL, 8);

        return (1);
    }

    return (0);
}

static bool _shatterable(const actor *act)
{
    if (act->atype() != ACT_MONSTER)
        return true; // no player ghostlies... at least user-controllable ones
    return _shatter_mon_dice(act->as_monster());
}

bool cast_shatter(int pow)
{
    {
        int r_min = std::min(3 + you.skill(SK_EARTH_MAGIC) / 5, 7);
        targetter_los hitfunc(&you, LOS_ARENA, r_min, std::min(r_min + 1, 7));
        if (stop_attack_prompt(hitfunc, "harm", _shatterable))
            return false;
    }

    const bool silence = truly_silenced(you.pos());

    if (silence)
        mpr("The dungeon shakes!");
    else
    {
        noisy(30, you.pos());
        mpr("The dungeon rumbles!", MSGCH_SOUND);
    }

    int rad = 2 + (you.skill(SK_EARTH_MAGIC) / 5);

    apply_area_within_radius(_shatter_items, you.pos(), pow, rad, 0, &you);
    apply_area_within_radius(_shatter_monsters, you.pos(), pow, rad, 0, &you);
    int dest = apply_area_within_radius(_shatter_walls, you.pos(),
                                        pow, rad, 0, &you);

    if (dest && !silence)
        mpr("Ka-crash!", MSGCH_SOUND);
    return true;
}

static int _ignite_poison_affect_item(item_def& item, bool in_inv)
{
    int strength = 0;

    // Poison branding becomes fire branding.
    // don't affect non-wielded weapons, they don't start dripping poison until
    // you wield them. -doy
    if (&item == you.weapon()
        && you.duration[DUR_WEAPON_BRAND]
        && get_weapon_brand(item) == SPWPN_VENOM)
    {
        if (set_item_ego_type(item, OBJ_WEAPONS, SPWPN_FLAMING))
        {
            mprf("%s bursts into flame!",
                 item.name(DESC_CAP_YOUR).c_str());

            you.wield_change = true;

            int increase = 1 + you.duration[DUR_WEAPON_BRAND]
                               /(2 * BASELINE_DELAY);

            you.increase_duration(DUR_WEAPON_BRAND, increase, 80);
        }

        // and don't destroy it
        return 0;
    }
    else if (item.base_type == OBJ_MISSILES && item.special == SPMSL_POISONED)
    {
        // Burn poison ammo.
        strength = item.quantity;
    }
    else if (item.base_type == OBJ_POTIONS)
    {
        // Burn poisonous potions.
        switch (item.sub_type)
        {
        case POT_STRONG_POISON:
            strength = 20 * item.quantity;
            break;
        case POT_DEGENERATION:
        case POT_POISON:
            strength = 10 * item.quantity;
            break;
        default:
            break;
        }
    }
    else if (item.base_type == OBJ_CORPSES &&
             item.sub_type == CORPSE_BODY &&
             chunk_is_poisonous(mons_corpse_effect(item.plus)))
    {
        strength = mons_weight(item.plus) / 25;
    }
    else if (item.base_type == OBJ_FOOD &&
             item.sub_type == FOOD_CHUNK &&
             chunk_is_poisonous(mons_corpse_effect(item.plus)))
    {
        strength += 30 * item.quantity;
    }

    if (strength)
    {
        if (in_inv)
        {
            if (item.base_type == OBJ_POTIONS)
                mprf("%s explode%s!",
                     item.name(DESC_PLAIN).c_str(), item.quantity == 1 ? "s" : "");
            else
                mprf("Your %s burn%s!",
                     item.name(DESC_PLAIN).c_str(), item.quantity == 1 ? "s" : "");
        }

        if (item.base_type == OBJ_CORPSES &&
            item.sub_type == CORPSE_BODY &&
            mons_skeleton(item.plus))
        {
            turn_corpse_into_skeleton(item);
        }
        else
        {
            if (in_inv && &item == you.weapon())
            {
                unwield_item();
                canned_msg(MSG_EMPTY_HANDED);
            }
            item_was_destroyed(item);
            if (in_inv)
                destroy_item(item);
            else
                destroy_item(item.index());
        }
    }

    return strength;
}

static int _ignite_poison_objects(coord_def where, int pow, int, actor *actor)
{
    UNUSED(pow);

    int strength = 0;

    for (stack_iterator si(where); si; ++si)
    {
        strength += _ignite_poison_affect_item(*si, false);
    }

    if (strength > 0)
    {
        place_cloud(CLOUD_FIRE, where,
                    strength + roll_dice(3, strength / 4), actor);
    }

    return (strength);
}

static int _ignite_poison_clouds(coord_def where, int pow, int, actor *actor)
{
    UNUSED(pow);

    const int i = env.cgrid(where);
    if (i != EMPTY_CLOUD)
    {
        cloud_struct& cloud = env.cloud[i];
        if (cloud.type == CLOUD_STINK)
        {
            cloud.decay /= 2;

            if (cloud.decay < 1)
                cloud.decay = 1;
        }
        else if (cloud.type != CLOUD_POISON)
            return false;

        cloud.type = CLOUD_FIRE;
        cloud.whose = actor->kill_alignment();
        cloud.killer = actor->is_player() ? KILL_YOU_MISSILE : KILL_MON_MISSILE;
        cloud.source = actor->mid;
        return true;
    }

    return false;
}

static int _ignite_poison_monsters(coord_def where, int pow, int, actor *actor)
{
    bolt beam;
    beam.flavour = BEAM_FIRE;   // This is dumb, only used for adjust!

    dice_def dam_dice(0, 5 + pow/7);  // Dice added below if applicable.

    // If a monster casts Ignite Poison, it can't hit itself.
    // This doesn't apply to the other functions: it can ignite
    // clouds or items where it's standing!

    monster* mon = monster_at(where);
    if (mon == NULL || mon == actor)
        return (0);

    // Monsters which have poison corpses or poisonous attacks.
    if (mons_is_poisoner(mon))
        dam_dice.num = 3;

    // Monsters which are poisoned:
    int strength = 0;

    // First check for player poison.
    mon_enchant ench = mon->get_ench(ENCH_POISON);
    if (ench.ench != ENCH_NONE)
        strength += ench.degree;

    // Strength is now the sum of both poison types
    // (although only one should actually be present at a given time).
    dam_dice.num += strength;

    int damage = dam_dice.roll();
    if (damage > 0)
    {
        damage = mons_adjust_flavoured(mon, beam, damage);
        simple_monster_message(mon, " seems to burn from within!");

        dprf("Dice: %dd%d; Damage: %d", dam_dice.num, dam_dice.size, damage);

        mon->hurt(actor, damage);

        if (mon->alive())
        {
            behaviour_event(mon, ME_WHACK, actor->mindex());

            // Monster survived, remove any poison.
            mon->del_ench(ENCH_POISON);
            print_wounds(mon);
        }
        else
        {
            monster_die(mon,
                        actor->is_player() ? KILL_YOU : KILL_MON,
                        actor->mindex());
        }

        return (1);
    }

    return (0);
}

// The self effects of Ignite Poison are beautiful and
// shouldn't be thrown out. Let's save them for a monster
// version of the spell!

static int _ignite_poison_player(coord_def where, int pow, int, actor *actor)
{
    if (actor->is_player() || where != you.pos())
        return (0);

    int totalstrength = 0;

    for (int i = 0; i < ENDOFPACK; ++i)
    {
        item_def& item = you.inv[i];
        if (!item.defined())
            continue;

        totalstrength += _ignite_poison_affect_item(item, true);
    }

    if (totalstrength)
    {
        place_cloud(
            CLOUD_FIRE, you.pos(),
            random2(totalstrength / 4 + 1) + random2(totalstrength / 4 + 1) +
            random2(totalstrength / 4 + 1) + random2(totalstrength / 4 + 1) + 1,
            actor);
    }

    int damage = 0;
    // Player is poisonous.
    if (player_mutation_level(MUT_SPIT_POISON)
        || player_mutation_level(MUT_STINGER)
        || you.form == TRAN_SPIDER // poison attack
        || (!player_is_shapechanged()
            && (you.species == SP_GREEN_DRACONIAN       // poison breath
                || you.species == SP_KOBOLD             // poisonous corpse
                || you.species == SP_NAGA)))            // spit poison
    {
        damage = roll_dice(3, 5 + pow / 7);
    }

    // Player is poisoned.
    damage += roll_dice(you.duration[DUR_POISONING], 6);

    if (damage)
    {
        const int resist = player_res_fire();
        if (resist > 0)
        {
            mpr("You feel like your blood is boiling!");
            damage /= 3;
        }
        else if (resist < 0)
        {
            mpr("The poison in your system burns terribly!");
            damage *= 3;
        }
        else
            mpr("The poison in your system burns!");

        ouch(damage, actor->as_monster()->mindex(), KILLED_BY_MONSTER,
             actor->as_monster()->name(DESC_NOCAP_A).c_str());

        if (you.duration[DUR_POISONING] > 0)
        {
            mpr("You feel that the poison has left your system.");
            you.duration[DUR_POISONING] = 0;
        }
    }

    if (damage || totalstrength)
        return (1);
    else
        return (0);
}

void cast_ignite_poison(int pow)
{
    flash_view(RED);

    apply_area_visible(_ignite_poison_clouds, pow, true, &you);
    apply_area_visible(_ignite_poison_objects, pow, true, &you);
    apply_area_visible(_ignite_poison_monsters, pow, true, &you);
// Not currently relevant - nothing will ever happen as long as
// the actor is &you.
    apply_area_visible(_ignite_poison_player, pow, false, &you);

#ifndef USE_TILE
    delay(100); // show a brief flash
#endif
    flash_view(0);
}

bool cast_discharge(int pow)
{
    bolt beam;

    beam.flavour      = BEAM_ELECTRICITY;
    beam.glyph        = dchar_glyph(DCHAR_FIRED_BURST);
    beam.damage       = dice_def(2, (15 + 2 * pow / 5));
    beam.target       = you.pos();
    beam.name         = "static discharge";
    beam.colour       = LIGHTCYAN;
    beam.ex_size      = 1;
    beam.is_explosion = true;
    beam.aux_source.clear();
    beam.thrower      = KILL_YOU_MISSILE;
    beam.is_tracer    = true;

    beam.explode(false, true);

    if(beam.beam_cancelled)
    {
        //Player aborted the casting.
        canned_msg(MSG_OK);
        return false;
    }

    beam.is_tracer          = false;
    //Failure to reset in_explosion_phase means failing an assert.
    beam.in_explosion_phase = false;
    
    mpr("Electricity rips through the air around you!");

    beam.explode(true, true);
    
    return true;
}

// Really this is just applying the best of Band/Warp weapon/Warp field
// into a spell that gives the "make monsters go away" benefit without
// the insane damage potential. - bwr
static int _disperse_monster(monster* mon, int pow)
{
    if (!mon)
        return (0);
    if (mons_is_projectile(mon->type))
        return (0);

    if (mons_genus(mon->type) == MONS_BLINK_FROG)
    {
        simple_monster_message(mon, " resists.");
        return (1);
    }
    else if (int res_margin = mon->check_res_magic(pow) > 0)
    {
        // XXX: Note that this might affect magic-immunes!
        if (coinflip())
        {
            simple_monster_message(mon, " partially resists.");
            monster_blink(mon);
        }
        else
        {
            simple_monster_message(mon,
                               mons_resist_string(mon, res_margin).c_str());
        }

        return (1);
    }
    else
    {
        monster_teleport(mon, true);
        return (1);
    }

    return (0);
}

void cast_dispersal(int pow)
{
    if (!apply_monsters_around_square(_disperse_monster, you.pos(), pow))
        mpr("The air shimmers briefly around you.");
}

bool cast_fragmentation(int pow, const dist& spd)
{
    bool explode = false;
    bool hole    = true;
    const char *what = NULL;

    if (!exists_ray(you.pos(), spd.target))
    {
        mpr("There's something in the way!");
        return (false);
    }

    //FIXME: If (player typed '>' to attack floor) goto do_terrain;

    bolt beam;

    beam.flavour     = BEAM_FRAG;
    beam.glyph       = dchar_glyph(DCHAR_FIRED_BURST);
    beam.beam_source = MHITYOU;
    beam.thrower     = KILL_YOU;
    beam.ex_size     = 1;
    beam.source      = you.pos();
    beam.hit         = AUTOMATIC_HIT;

    beam.set_target(spd);
    beam.aux_source.clear();

    // Number of dice vary... 3 is easy/common, but it can get as high as 6.
    beam.damage = dice_def(0, 5 + pow / 5);

    const dungeon_feature_type grid = grd(spd.target);

    if (monster* mon = monster_at(spd.target))
    {
        // Save the monster's name in case it isn't available later.
        const std::string name_cap_the = mon->name(DESC_CAP_THE);

        switch (mon->type)
        {
        case MONS_WOOD_GOLEM:
            explode         = true;
            beam.name       = "blast of splinters";
            beam.colour     = BROWN;
            beam.damage.num = 2;
            if (_player_hurt_monster(*mon, beam.damage.roll(),
                                     BEAM_DISINTEGRATION))
                beam.damage.num++;
            break;

        case MONS_TOENAIL_GOLEM:
            explode         = true;
            beam.name       = "blast of toenail fragments";
            beam.colour     = RED;
            beam.damage.num = 2;
            if (_player_hurt_monster(*mon, beam.damage.roll(),
                                     BEAM_DISINTEGRATION))
                beam.damage.num++;
            break;

        case MONS_IRON_ELEMENTAL:
        case MONS_IRON_GOLEM:
        case MONS_METAL_GARGOYLE:
            explode         = true;
            beam.name       = "blast of metal fragments";
            beam.colour     = CYAN;
            beam.damage.num = 4;
            if (_player_hurt_monster(*mon, beam.damage.roll(),
                                     BEAM_DISINTEGRATION))
                beam.damage.num += 2;
            break;

        case MONS_EARTH_ELEMENTAL:
        case MONS_CLAY_GOLEM:
        case MONS_STONE_GOLEM:
        case MONS_STATUE:
        case MONS_GARGOYLE:
            explode         = true;
            beam.ex_size    = 2;
            beam.name       = "blast of rock fragments";
            beam.colour     = BROWN;
            beam.damage.num = 3;
            if (_player_hurt_monster(*mon, beam.damage.roll(),
                                     BEAM_DISINTEGRATION))
                beam.damage.num++;
            break;

        case MONS_SILVER_STATUE:
        case MONS_ORANGE_STATUE:
            explode         = true;
            beam.ex_size    = 2;
            beam.damage.num = 4;
            if (mon->type == MONS_SILVER_STATUE)
            {
                beam.name       = "blast of silver fragments";
                beam.colour     = WHITE;
            }
            else
            {
                beam.name       = "blast of orange crystal shards";
                beam.colour     = LIGHTRED;
            }

            {
                int statue_damage = beam.damage.roll() * 2;
                if (pow >= 50 && one_chance_in(10))
                    statue_damage = mon->hit_points;

                if (_player_hurt_monster(*mon, statue_damage,
                                         BEAM_DISINTEGRATION))
                    beam.damage.num += 2;
            }
            break;

        case MONS_CRYSTAL_GOLEM:
        case MONS_ROXANNE:
            explode         = true;
            beam.ex_size    = 2;
            beam.damage.num = 4;
            if (mon->type == MONS_CRYSTAL_GOLEM)
            {
                beam.name       = "blast of crystal shards";
                beam.colour     = GREEN;
            }
            else
            {
                beam.name       = "blast of sapphire shards";
                beam.colour     = BLUE;
            }
            if (_player_hurt_monster(*mon, beam.damage.roll(),
                                     BEAM_DISINTEGRATION))
                beam.damage.num += 2;
            break;

        default:
            const bool petrifying = mon->petrifying();
            const bool petrified = mon->petrified() && !petrifying;

            // Petrifying or petrified monsters can be exploded.
            if (petrifying || petrified)
            {
                explode         = true;
                beam.ex_size    = petrifying ? 1 : 2;
                beam.name       = "blast of petrified fragments";
                beam.colour     = mons_class_colour(mon->type);
                beam.damage.num = petrifying ? 2 : 3;
                if (_player_hurt_monster(*mon, beam.damage.roll(),
                                         BEAM_DISINTEGRATION))
                    beam.damage.num++;
                break;
            }
            else if (mon->is_icy()) // blast of ice
            {
                explode         = true;
                beam.name       = "icy blast";
                beam.colour     = WHITE;
                beam.damage.num = 2;
                beam.flavour    = BEAM_ICE;
                if (_player_hurt_monster(*mon, beam.damage.roll()),
                                         BEAM_DISINTEGRATION)
                    beam.damage.num++;
                break;
            }
            else if (mon->is_skeletal()) // blast of bone
            {
                mprf("The %s explodes into sharp fragments of bone!",
                     (mon->type == MONS_FLYING_SKULL) ? "skull" : "skeleton");

                explode     = true;
                beam.name   = "blast of bone shards";
                beam.colour = LIGHTGREY;

                if (x_chance_in_y(pow / 5, 50)) // potential insta-kill
                {
                    monster_die(mon, KILL_YOU, NON_MONSTER);
                    beam.damage.num = 4;
                }
                else
                {
                      beam.damage.num = 2;
                      if (_player_hurt_monster(*mon, beam.damage.roll(),
                                               BEAM_DISINTEGRATION))
                          beam.damage.num += 2;
                }
                goto all_done; // Messaging already handled for skeletons.
            }
            goto do_terrain; // Targeted monster not shatterable.
        }

        mprf("%s shatters!", name_cap_the.c_str());
        goto all_done;
    }

    for (stack_iterator si(spd.target, true); si; ++si)
    {
        if (si->base_type == OBJ_CORPSES)
        {
            std::string nm = si->name(DESC_CAP_THE);
            if (si->sub_type == CORPSE_BODY)
            {
                if (!explode_corpse(*si, spd.target))
                {
                    mprf("%s seems to be exceptionally well connected.",
                         nm.c_str());

                    goto all_done;
                }
            }

            mprf("%s explodes!", nm.c_str());
            destroy_item(si.link());
            // si invalid now!
            goto all_done;
        }
    }

    if (env.markers.property_at(spd.target, MAT_ANY,
                                "veto_fragmentation") == "veto")
    {
        mprf("%s seems to be unnaturally hard.",
             feature_description(spd.target, false, DESC_CAP_THE, false).c_str());
        canned_msg(MSG_SPELL_FIZZLES);
        return (true);
    }

  do_terrain:

    switch (grid)
    {
    //
    // Stone and rock terrain
    //
    case DNGN_ROCK_WALL:
    case DNGN_SECRET_DOOR:
        beam.colour = env.rock_colour;
        // fall-through
    case DNGN_STONE_WALL:
        if (player_in_branch(BRANCH_HALL_OF_ZOT))
            beam.colour = env.rock_colour;
    case DNGN_CLEAR_ROCK_WALL:
    case DNGN_CLEAR_STONE_WALL:
        if (beam.colour == 0)
            beam.colour = LIGHTCYAN;
        what = "wall";
        // fall-through
    case DNGN_ORCISH_IDOL:
        if (what == NULL)
            what = "stone idol";
        if (beam.colour == 0)
            beam.colour = DARKGREY;
        // fall-through
    case DNGN_GRANITE_STATUE:   // normal rock -- big explosion
        if (what == NULL)
            what = "statue";

        explode = true;

        beam.name       = "blast of rock fragments";
        beam.damage.num = 3;
        if (beam.colour == 0)
            beam.colour = LIGHTGREY;

        if ((grid == DNGN_ORCISH_IDOL
             || grid == DNGN_GRANITE_STATUE
             || grid == DNGN_GRATE
             || pow >= 40 && grid == DNGN_ROCK_WALL && one_chance_in(3)
             || pow >= 40 && grid == DNGN_CLEAR_ROCK_WALL
                 && one_chance_in(3)
             || pow >= 60 && grid == DNGN_STONE_WALL && one_chance_in(10)
             || pow >= 60 && grid == DNGN_CLEAR_STONE_WALL
                 && one_chance_in(10)))
        {
            // terrain blew up real good:
            beam.ex_size        = 2;
            nuke_wall(spd.target);
        }
        break;

    //
    // Metal -- small but nasty explosion
    //

    case DNGN_METAL_WALL:
        what            = "metal wall";
        // fall through
    case DNGN_GRATE:
        if (what == NULL)
            what        = "iron grate";
        beam.colour     = CYAN;
        explode         = true;
        beam.name       = "blast of metal fragments";
        beam.damage.num = 4;

        if (pow >= 80 && x_chance_in_y(pow / 5, 500) || grid == DNGN_GRATE)
        {
            beam.damage.num += 2;
            nuke_wall(spd.target);
        }
        break;

    //
    // Crystal
    //

    case DNGN_GREEN_CRYSTAL_WALL:       // crystal -- large & nasty explosion
        what            = "crystal wall";
        beam.colour     = GREEN;
        explode         = true;
        beam.ex_size    = 2;
        beam.name       = "blast of crystal shards";
        beam.damage.num = 5;

        if (coinflip())
        {
            beam.ex_size    = coinflip() ? 3 : 2;
            nuke_wall(spd.target);
        }
        break;

    //
    // Traps
    //

    case DNGN_UNDISCOVERED_TRAP:
    case DNGN_TRAP_MECHANICAL:
    {
        trap_def* ptrap = find_trap(spd.target);
        ASSERT(ptrap);
        if (ptrap->category() != DNGN_TRAP_MECHANICAL)
        {
            // Non-mechanical traps don't explode with this spell. -- bwr
            break;
        }

        // Undiscovered traps appear as exploding from the floor. -- bwr
        what = ((grid == DNGN_UNDISCOVERED_TRAP) ? "floor" : "trap");

        explode         = true;
        hole            = false;    // to hit monsters standing on traps
        beam.name       = "blast of fragments";
        beam.colour     = env.floor_colour;  // in order to blend in
        beam.damage.num = 2;

        // Exploded traps are nonfunctional, ammo is also ruined -- bwr
        ptrap->destroy();
        break;
    }

    //
    // Stone doors and arches
    //

    case DNGN_OPEN_DOOR:
    case DNGN_CLOSED_DOOR:
    case DNGN_DETECTED_SECRET_DOOR:
        // Doors always blow up, stone arches never do (would cause problems).
        nuke_wall(spd.target);

        // fall-through
    case DNGN_STONE_ARCH:          // Floor -- small explosion.
        explode         = true;
        hole            = false;  // to hit monsters standing on doors
        beam.name       = "blast of rock fragments";
        beam.colour     = LIGHTGREY;
        beam.damage.num = 2;
        break;

    default:
        // FIXME: cute message for water?
        break;
    }

    // If it was recoloured, use that colour instead.
    if (env.grid_colours(spd.target))
        beam.colour = env.grid_colours(spd.target);

  all_done:
    if (explode && beam.damage.num > 0)
    {
        if (what != NULL)
            mprf("The %s shatters!", what);

        beam.explode(true, hole);

        if (grid == DNGN_ORCISH_IDOL)
            did_god_conduct(DID_DESTROY_ORCISH_IDOL, 8);
    }
    else if (beam.damage.num == 0)
    {
        // If damage dice are zero, assume that nothing happened at all.
        canned_msg(MSG_SPELL_FIZZLES);
    }

    return (true);
}

int wielding_rocks()
{
    const item_def* wpn = you.weapon();
    if (!wpn || wpn->base_type != OBJ_MISSILES)
        return (0);
    else if (wpn->sub_type == MI_STONE)
        return (1);
    else if (wpn->sub_type == MI_LARGE_ROCK)
        return (2);
    else
        return (0);
}

bool cast_sandblast(int pow, bolt &beam)
{
    zap_type zap = ZAP_SMALL_SANDBLAST;
    switch (wielding_rocks())
    {
    case 1:
        zap = ZAP_SANDBLAST;
        break;
    case 2:
        zap = ZAP_LARGE_SANDBLAST;
        break;
    default:
        break;
    }

    const bool success = zapping(zap, pow, beam, true);

    if (success && zap != ZAP_SMALL_SANDBLAST)
        dec_inv_item_quantity(you.equip[EQ_WEAPON], 1);

    return (success);
}

// Find an enemy who would suffer from Awaken Forest.
actor* forest_near_enemy(const actor *mon)
{
    const coord_def pos = mon->pos();

    for (radius_iterator ri(pos, LOS_RADIUS); ri; ++ri)
    {
        actor* foe = actor_at(*ri);
        if (!foe || mons_aligned(foe, mon))
            continue;

        for (adjacent_iterator ai(*ri); ai; ++ai)
            if (feat_is_tree(grd(*ai)) && cell_see_cell(pos, *ai, LOS_DEFAULT))
                return (foe);
    }

    return (NULL);
}

// Print a message only if you can see any affected trees.
void forest_message(const coord_def pos, const std::string &msg, msg_channel_type ch)
{
    for (radius_iterator ri(pos, LOS_RADIUS); ri; ++ri)
        if (feat_is_tree(grd(*ri))
            && cell_see_cell(you.pos(), *ri, LOS_DEFAULT)
            && cell_see_cell(pos, *ri, LOS_DEFAULT))
        {
            mpr(msg, ch);
            return;
        }
}

void forest_damage(const actor *mon)
{
    const coord_def pos = mon->pos();

    if (one_chance_in(4))
        forest_message(pos, random_choose(
            "The trees move their gnarly branches around.",
            "You feel roots moving beneath the ground.",
            "Branches wave dangerously above you.",
            "Trunks creak and shift.",
            "Tree limbs sway around you.",
            0), MSGCH_TALK_VISUAL);

    for (radius_iterator ri(pos, LOS_RADIUS); ri; ++ri)
    {
        actor* foe = actor_at(*ri);
        if (!foe || mons_aligned(foe, mon))
            continue;

        for (adjacent_iterator ai(*ri); ai; ++ai)
            if (feat_is_tree(grd(*ai)) && cell_see_cell(pos, *ai, LOS_DEFAULT))
            {
                const int damage = 5 + random2(10);
                if (foe->atype() == ACT_PLAYER)
                {
                    mpr(random_choose(
                        "You are hit by a branch!",
                        "A tree reaches out and hits you!",
                        "A root smacks you from below.",
                        0));
                    ouch(damage, mon->mindex(), KILLED_BY_BEAM,
                         "angry trees", true);
                }
                else
                {
                    if (you.see_cell(foe->pos()))
                    {
                        const char *msg = random_choose(
                            "%s is hit by a branch!",
                            "A tree reaches out and hits %s!",
                            "A root smacks %s from below.",
                            0);
                        const bool up = *msg == '%';
                        // "it" looks butt-ugly here...
                        mprf(msg, foe->visible_to(&you) ?
                                      foe->name(up ? DESC_CAP_THE
                                                   : DESC_NOCAP_THE).c_str()
                                    : up ? "Something" : "something");
                    }
                    foe->hurt(mon, damage);
                }
                break;
            }
    }
}
