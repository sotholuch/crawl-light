#include "AppHdr.h"

#include "player-equip.h"

#include "areas.h"
#include "artefact.h"
#include "delay.h"
#include "describe.h"
#include "food.h"
#include "goditem.h"
#include "godpassive.h"
#include "hints.h"
#include "item_use.h"
#include "itemname.h"
#include "itemprop.h"
#include "items.h"
#include "misc.h"
#include "notes.h"
#include "options.h"
#include "player.h"
#include "player-stats.h"
#include "religion.h"
#include "shopping.h"
#include "skills2.h"
#include "spl-book.h"
#include "spl-cast.h"
#include "spl-miscast.h"
#include "state.h"
#include "stuff.h"
#include "transform.h"
#include "xom.h"

#include <cmath>

static void _equip_effect(equipment_type slot, int item_slot, bool unmeld,
                          bool msg);
static void _unequip_effect(equipment_type slot, int item_slot, bool meld,
                            bool msg);

// Fill an empty equipment slot.
void equip_item(equipment_type slot, int item_slot, bool msg)
{
    ASSERT(slot > EQ_NONE && slot < NUM_EQUIP);
    ASSERT(you.equip[slot] == -1);
    ASSERT(!you.melded[slot]);

    you.equip[slot] = item_slot;

    _equip_effect(slot, item_slot, false, msg);
}

// Clear an equipment slot (possibly melded).
bool unequip_item(equipment_type slot, bool msg)
{
    ASSERT(slot > EQ_NONE && slot < NUM_EQUIP);
    ASSERT(!you.melded[slot] || you.equip[slot] != -1);

    const int item_slot = you.equip[slot];
    if (item_slot == -1)
        return (false);
    else
    {
        you.equip[slot] = -1;
        if (!you.melded[slot])
            _unequip_effect(slot, item_slot, false, msg);
        else
            you.melded[slot] = false;
        return (true);
    }
}

// Meld a slot (if equipped).
bool meld_slot(equipment_type slot, bool msg)
{
    ASSERT(slot > EQ_NONE && slot < NUM_EQUIP);
    ASSERT(!you.melded[slot] || you.equip[slot] != -1);

    if (you.equip[slot] != -1 && !you.melded[slot])
    {
        you.melded[slot] = true;
        _unequip_effect(slot, you.equip[slot], true, msg);
        return (true);
    }
    return (false);
}

bool unmeld_slot(equipment_type slot, bool msg)
{
    ASSERT(slot > EQ_NONE && slot < NUM_EQUIP);
    ASSERT(!you.melded[slot] || you.equip[slot] != -1);

    if (you.equip[slot] != -1 && you.melded[slot])
    {
        you.melded[slot] = false;
        _equip_effect(slot, you.equip[slot], true, msg);
        return (true);
    }
    return (false);
}

static void _equip_weapon_effect(item_def& item, bool showMsgs, bool unmeld);
static void _unequip_weapon_effect(item_def& item, bool showMsgs, bool meld);
static void _equip_armour_effect(item_def& arm, bool unmeld);
static void _unequip_armour_effect(item_def& item, bool meld);
static void _equip_jewellery_effect(item_def &item, bool unmeld);
static void _unequip_jewellery_effect(item_def &item, bool mesg);
static void _equip_use_warning(const item_def& item);

static void _equip_effect(equipment_type slot, int item_slot, bool unmeld,
                          bool msg)
{
    item_def& item = you.inv[item_slot];
    equipment_type eq = get_item_slot(item);

    if (slot == EQ_WEAPON && eq != EQ_WEAPON)
        return;

    ASSERT(slot == eq
           || eq == EQ_RINGS
              && (slot == EQ_LEFT_RING || slot == EQ_RIGHT_RING));

    if (msg)
        _equip_use_warning(item);

    if (slot == EQ_WEAPON)
        _equip_weapon_effect(item, msg, unmeld);
    else if (slot >= EQ_CLOAK && slot <= EQ_BODY_ARMOUR)
        _equip_armour_effect(item, unmeld);
    else if (slot >= EQ_LEFT_RING && slot <= EQ_AMULET)
        _equip_jewellery_effect(item, unmeld);
}

static void _unequip_effect(equipment_type slot, int item_slot, bool meld,
                            bool msg)
{
    item_def& item = you.inv[item_slot];
    equipment_type eq = get_item_slot(item);

    if (slot == EQ_WEAPON && eq != EQ_WEAPON)
        return;

    ASSERT(slot == eq
           || eq == EQ_RINGS
              && (slot == EQ_LEFT_RING || slot == EQ_RIGHT_RING));

    if (slot == EQ_WEAPON)
        _unequip_weapon_effect(item, msg, meld);
    else if (slot >= EQ_CLOAK && slot <= EQ_BODY_ARMOUR)
        _unequip_armour_effect(item, meld);
    else if (slot >= EQ_LEFT_RING && slot <= EQ_AMULET)
        _unequip_jewellery_effect(item, msg);
}

void _hp_artefact()
{
    // Rounding must be down or Deep Dwarves would abuse certain values.
    // We can reduce errors by a factor of 100 by using partial hp we have.
    int old_max = you.hp_max;
    int hp = you.hp * 100 + you.hit_points_regeneration;
    calc_hp();
    int new_max = you.hp_max;
    hp = hp * new_max / old_max;
    if (hp < 100)
        hp = 100;
    you.hp = std::min(hp / 100, you.hp_max);
    you.hit_points_regeneration = hp % 100;
    if (you.hp_max <= 0) // Borgnjor's abusers...
        ouch(0, NON_MONSTER, KILLED_BY_DRAINING);
}

///////////////////////////////////////////////////////////
// Actual equip and unequip effect implementation below
//

static void _equip_artefact_effect(item_def &item, bool *show_msgs, bool unmeld)
{
    ASSERT(is_artefact(item));

    // Call unrandart equip function first, so that it can modify the
    // artefact's properties before they're applied.
    if (is_unrandom_artefact(item))
    {
        const unrandart_entry *entry = get_unrand_entry(item.special);

        if (entry->equip_func)
            entry->equip_func(&item, show_msgs, unmeld);

        if (entry->world_reacts_func)
        {
            equipment_type eq = get_item_slot(item.base_type, item.sub_type);
            you.unrand_reacts |= (1 << eq);
        }
    }

    const bool msg          = !show_msgs || *show_msgs;

    artefact_properties_t  proprt;
    artefact_known_props_t known;
    artefact_wpn_properties(item, proprt, known);

    // Only give property messages for previously unknown properties.
    if (proprt[ARTP_AC])
    {
        you.redraw_armour_class = true;
    }

    if (proprt[ARTP_EVASION])
    {
        you.redraw_evasion = true;
    }

    if (proprt[ARTP_PONDEROUS] && !unmeld)
    {
        if (msg)
            mpr("You feel rather ponderous.");
        che_handle_change(CB_PONDEROUS_COUNT, 1);
    }

    if (proprt[ARTP_EYESIGHT])
        autotoggle_autopickup(false);

    // Modify ability scores.
    // Output result even when identified (because of potential fatality).
    notify_stat_change(STAT_STR,     proprt[ARTP_STRENGTH], !msg, item);
    notify_stat_change(STAT_INT, proprt[ARTP_INTELLIGENCE], !msg, item);
    notify_stat_change(STAT_DEX,    proprt[ARTP_DEXTERITY], !msg, item);

    if (proprt[ARTP_NOISES])
        you.attribute[ATTR_NOISES] = 1;

    if (proprt[ARTP_HP])
        _hp_artefact();

    // Let's try this here instead of up there.
    if (proprt[ARTP_MAGICAL_POWER])
        calc_mp();
}

static void _unequip_artefact_effect(const item_def &item,
                                     bool *show_msgs = NULL, bool meld = false)
{
    ASSERT(is_artefact(item));

    artefact_properties_t proprt;
    artefact_known_props_t known;
    artefact_wpn_properties(item, proprt, known);
    const bool msg = !show_msgs || *show_msgs;

    if (proprt[ARTP_AC])
    {
        you.redraw_armour_class = true;
    }

    if (proprt[ARTP_EVASION])
    {
        you.redraw_evasion = true;
    }

    if (proprt[ARTP_PONDEROUS] && !meld)
    {
        if (msg)
            mpr("That put a bit of spring back into your step.");
        che_handle_change(CB_PONDEROUS_COUNT, -1);
    }

    // Modify ability scores; always output messages.
    notify_stat_change(STAT_STR, -proprt[ARTP_STRENGTH],     !msg, item,
                       true);
    notify_stat_change(STAT_INT, -proprt[ARTP_INTELLIGENCE], !msg, item,
                       true);
    notify_stat_change(STAT_DEX, -proprt[ARTP_DEXTERITY],    !msg, item,
                       true);

    if (proprt[ARTP_NOISES] != 0)
        you.attribute[ATTR_NOISES] = 0;

    if (proprt[ARTP_LEVITATE] != 0
        && you.duration[DUR_LEVITATION]
        && !you.attribute[ATTR_LEV_UNCANCELLABLE]
        && !you.permanent_levitation()
        && !player_evokable_levitation())
    {
        you.duration[DUR_LEVITATION] = 0;
        land_player();
    }

    if (proprt[ARTP_INVISIBLE] != 0
        && you.duration[DUR_INVIS] > 1
        && !you.attribute[ATTR_INVIS_UNCANCELLABLE]
        && !player_evokable_invis())
    {
        you.duration[DUR_INVIS] = 1;
    }
    
    if (proprt[ARTP_HP])
        _hp_artefact();

    if (proprt[ARTP_MAGICAL_POWER])
        calc_mp();

    if (is_unrandom_artefact(item))
    {
        const unrandart_entry *entry = get_unrand_entry(item.special);

        if (entry->unequip_func)
            entry->unequip_func(&item, show_msgs);

        if (entry->world_reacts_func)
        {
            equipment_type eq = get_item_slot(item.base_type, item.sub_type);
            you.unrand_reacts &= ~(1 << eq);
        }
    }
}

static void _equip_use_warning(const item_def& item)
{
    if (is_holy_item(item) && you.religion == GOD_YREDELEMNUL)
        mpr("You really shouldn't be using a holy item like this.");
    else if (is_unholy_item(item) && is_good_god(you.religion))
        mpr("You really shouldn't be using an unholy item like this.");
    else if (is_corpse_violating_item(item) && you.religion == GOD_FEDHAS)
        mpr("You really shouldn't be using a corpse-violating item like this.");
    else if (is_evil_item(item) && is_good_god(you.religion))
        mpr("You really shouldn't be using an evil item like this.");
    else if (is_unclean_item(item) && you.religion == GOD_ZIN)
        mpr("You really shouldn't be using an unclean item like this.");
    else if (is_chaotic_item(item) && you.religion == GOD_ZIN)
        mpr("You really shouldn't be using a chaotic item like this.");
    else if (is_hasty_item(item) && you.religion == GOD_CHEIBRIADOS)
        mpr("You really shouldn't be using a hasty item like this.");
    else if (is_poisoned_item(item) && you.religion == GOD_SHINING_ONE)
        mpr("You really shouldn't be using a poisoned item like this.");
    else if (is_edicted_item(item) && you.religion == GOD_ZIN)
        mpr("You really shouldn't be using a proscribed item like this."); 
}

// Provide a function for handling initial wielding of 'special'
// weapons, or those whose function is annoying to reproduce in
// other places *cough* auto-butchering *cough*.    {gdl}
static void _equip_weapon_effect(item_def& item, bool showMsgs, bool unmeld)
{
    int special = 0;

    const bool artefact     = is_artefact(item);

    // And here we finally get to the special effects of wielding. {dlb}
    switch (item.base_type)
    {
    case OBJ_MISCELLANY:
    {
        if (item.sub_type == MISC_LANTERN_OF_SHADOWS)
        {
            if (showMsgs)
                mpr("The area is filled with flickering shadows.");

            you.attribute[ATTR_SHADOWS] = 1;
            update_vision_range();
        }
        break;
    }

    case OBJ_STAVES:
    {
        set_ident_flags(item, ISFLAG_IDENT_MASK);
        set_ident_type(OBJ_STAVES, item.sub_type, ID_KNOWN_TYPE);

        if (item.sub_type == STAFF_POWER)
        {
            int mp = item.special - you.elapsed_time / POWER_DECAY;

            if (mp > 0)
                you.magic_points += mp;

            if ((you.max_magic_points + 13) *
                (1.0+player_mutation_level(MUT_HIGH_MAGIC)/10.0) > 50)
                mpr("You feel your mana capacity is already quite full.");
            else
                canned_msg(MSG_MANA_INCREASE);

            calc_mp();
        }
        break;
    }

    case OBJ_WEAPONS:
    {
        // Call unrandart equip func before item is identified.
        if (artefact)
            _equip_artefact_effect(item, &showMsgs, unmeld);

        set_ident_flags(item, ISFLAG_EQ_WEAPON_MASK);

        special = item.special;

        if (artefact)
        {
            special = artefact_wpn_property(item, ARTP_BRAND);
        }

        if (special != SPWPN_NORMAL)
        {
            // message first
            if (showMsgs)
            {
                switch (special)
                {
                case SPWPN_FLAMING:
                    mpr("It bursts into flame!");
                    break;

                case SPWPN_FREEZING:
                    mpr("It glows with a cold blue light!");
                    break;

                case SPWPN_HOLY_WRATH:
                    mpr("It softly glows with a divine radiance!");
                    break;

                case SPWPN_ELECTROCUTION:
                    if (!truly_silenced(you.pos()))
                    {
                        mpr("You hear the crackle of electricity.",
                            MSGCH_SOUND);
                    }
                    else
                        mpr("You see sparks fly.");
                    break;

                case SPWPN_ORC_SLAYING:
                    mpr((you.species == SP_HILL_ORC)
                            ? "You feel a sudden desire to commit suicide."
                            : "You feel a sudden desire to kill orcs!");
                    break;

                case SPWPN_DRAGON_SLAYING:
                    mpr(player_genus(GENPC_DRACONIAN)
                        || you.form == TRAN_DRAGON
                            ? "You feel a sudden desire to commit suicide."
                            : "You feel a sudden desire to slay dragons!");
                    break;

                case SPWPN_VENOM:
                    mpr("It begins to drip with poison!");
                    break;

                case SPWPN_PROTECTION:
                    mpr("You feel protected!");
                    break;

                case SPWPN_EVASION:
                    mpr("You feel nimbler!");
                    break;

                case SPWPN_DRAINING:
                    mpr("You sense an unholy aura.");
                    break;

                case SPWPN_SPEED:
                    mprf("Your %s tingle!",
                         you.hand_name(true).c_str());
                    break;

                case SPWPN_FLAME:
                    mpr("It bursts into flame!");
                    break;

                case SPWPN_FROST:
                    mpr("It is covered in frost.");
                    break;

                case SPWPN_VAMPIRICISM:
                    if (you.species == SP_VAMPIRE)
                    {
                        mpr("You feel a bloodthirsty glee!");
                        break;
                    }

                    if (you.is_undead == US_ALIVE)
                    {
                        mpr("You feel a dreadful hunger.");
                    }
                    else
                        mpr("You feel an empty sense of dread.");
                    break;

                case SPWPN_RETURNING:
                    mpr("It wiggles slightly.");
                    break;

                case SPWPN_PAIN:
                    if (you.skill(SK_NECROMANCY) == 0)
                        mpr("You have a feeling of ineptitude.");
                    else if (you.skill(SK_NECROMANCY) <= 6)
                        mpr("Pain shudders through your arm!");
                    else
                        mpr("A searing pain shoots up your arm!");
                    break;

                case SPWPN_CHAOS:
                    mpr("It is briefly surrounded by a scintillating aura "
                        "of random colours.");
                    break;

                case SPWPN_PENETRATION:
                    mprf("Your %s briefly pass through it before you manage "
                         "to get a firm grip on it.",
                         you.hand_name(true).c_str());
                    break;

                case SPWPN_REAPING:
                    mpr("It is briefly surrounded by shifting shadows.");
                    break;

                case SPWPN_ANTIMAGIC:
                    // Even if your maxmp is 0.
                    mpr("You feel magic leave you.");
                    break;

                default:
                    break;
                }
            }

            // effect second
            switch (special)
            {
            case SPWPN_PROTECTION:
                you.redraw_armour_class = true;
                break;

            case SPWPN_EVASION:
                you.redraw_evasion = true;
                break;

            case SPWPN_DISTORTION:
                mpr("Space warps around you for a moment!");
                break;

            case SPWPN_ANTIMAGIC:
                calc_mp();
                break;

            default:
                break;
            }
        }
        break;
    }
    default:
        break;
    }

    if (showMsgs)
        warn_shield_penalties();

    you.attribute[ATTR_WEAPON_SWAP_INTERRUPTED] = 0;
}

static void _unequip_weapon_effect(item_def& item, bool showMsgs, bool meld)
{
    you.wield_change = true;
    you.m_quiver->on_weapon_changed();

    // Call this first, so that the unrandart func can set showMsgs to
    // false if it does its own message handling.
    if (is_artefact(item))
        _unequip_artefact_effect(item, &showMsgs);

    if (item.base_type == OBJ_MISCELLANY
        && item.sub_type == MISC_LANTERN_OF_SHADOWS)
    {
        you.attribute[ATTR_SHADOWS] = 0;
        update_vision_range();
    }
    else if (item.base_type == OBJ_WEAPONS)
    {
        const int brand = get_weapon_brand(item);

        if (brand != SPWPN_NORMAL)
        {
            const std::string msg = item.name(DESC_CAP_YOUR);

            switch (brand)
            {
            case SPWPN_FLAMING:
                if (showMsgs)
                    mprf("%s stops flaming.", msg.c_str());
                break;

            case SPWPN_FREEZING:
            case SPWPN_HOLY_WRATH:
                if (showMsgs)
                    mprf("%s stops glowing.", msg.c_str());
                break;

            case SPWPN_ELECTROCUTION:
                if (showMsgs)
                    mprf("%s stops crackling.", msg.c_str());
                break;

            case SPWPN_VENOM:
                if (showMsgs)
                    mprf("%s stops dripping with poison.", msg.c_str());
                break;

            case SPWPN_PROTECTION:
                if (showMsgs)
                    mpr("You feel less protected.");
                you.redraw_armour_class = true;
                break;

            case SPWPN_EVASION:
                if (showMsgs)
                    mpr("You feel like more of a target.");
                you.redraw_evasion = true;
                break;

            case SPWPN_VAMPIRICISM:
                if (showMsgs)
                {
                    if (you.species == SP_VAMPIRE)
                        mpr("You feel your glee subside.");
                    else
                        mpr("You feel the dreadful sensation subside.");
                }
                break;

            case SPWPN_DISTORTION:
                // Removing the translocations skill reduction of effect,
                // it might seem sensible, but this brand is supposed
                // to be dangerous because it does large bonus damage,
                // as well as free teleport other side effects, and
                // even with the miscast effects you can rely on the
                // occasional spatial bonus to mow down some opponents.
                // It's far too powerful without a real risk, especially
                // if it's to be allowed as a player spell. -- bwr

                // int effect = 9 -
                //        random2avg(you.skills[SK_TRANSLOCATIONS] * 2, 2);

                if (you.duration[DUR_WEAPON_BRAND] == 0 && !meld)
                {
                    // Makes no sense to discourage unwielding a temporarily
                    // branded weapon since you can wait it out. This also
                    // fixes problems with unwield prompts (mantis #793).
                    
                    // Lugonu will let you unwield distortion without harm.
                    if(you.religion == GOD_LUGONU && !player_under_penance())
                        simple_god_message(" protects you from your twisted weapon.");
                    else
                    {
                        MiscastEffect(&you, WIELD_MISCAST, SPTYP_TRANSLOCATION,
                                      9, 90, "distortion unwield");
                    }
                }
                break;

            case SPWPN_ANTIMAGIC:
                calc_mp();
                mpr("You feel magic returning to you.");
                break;

                // NOTE: When more are added here, *must* duplicate unwielding
                // effect in vorpalise weapon scroll effect in read_scoll.
            }

            if (you.duration[DUR_WEAPON_BRAND])
            {
                you.duration[DUR_WEAPON_BRAND] = 0;
                set_item_ego_type(item, OBJ_WEAPONS, SPWPN_NORMAL);

                // We're letting this through even if hiding messages.
                mpr("Your branding evaporates.");
            }
        }
    }
    else if (item.base_type == OBJ_STAVES && item.sub_type == STAFF_POWER)
    {
        int mp = you.magic_points;
        calc_mp();
        mp -= you.magic_points;

        // Store the MP in case you'll re-wield quickly.
        item.special = mp + you.elapsed_time / POWER_DECAY;

        canned_msg(MSG_MANA_DECREASE);
    }
}

static void _equip_armour_effect(item_def& arm, bool unmeld)
{
    int ego = get_armour_ego_type(arm);
    if (ego != SPARM_NORMAL)
    {
        switch (ego)
        {
        case SPARM_RUNNING:
            if (!you.fishtail)
                mpr("You feel quick.");
            break;

        case SPARM_FIRE_RESISTANCE:
            mpr("You feel resistant to fire.");
            break;

        case SPARM_COLD_RESISTANCE:
            mpr("You feel resistant to cold.");
            break;

        case SPARM_POISON_RESISTANCE:
            mpr("You feel healthy.");
            break;

        case SPARM_SEE_INVISIBLE:
            mpr("You feel perceptive.");
            autotoggle_autopickup(false);
            break;

        case SPARM_DARKNESS:
            if (!you.duration[DUR_INVIS])
                mpr("You become transparent for a moment.");
            break;

        case SPARM_STRENGTH:
            notify_stat_change(STAT_STR, 3, false, arm);
            break;

        case SPARM_DEXTERITY:
            notify_stat_change(STAT_DEX, 3, false, arm);
            break;

        case SPARM_INTELLIGENCE:
            notify_stat_change(STAT_INT, 3, false, arm);
            break;

        case SPARM_PONDEROUSNESS:
            if (!unmeld)
            {
                mpr("You feel rather ponderous.");
                che_handle_change(CB_PONDEROUS_COUNT, 1);
                you.redraw_evasion = true;
            }
            break;

        case SPARM_LEVITATION:
            mpr("You feel rather light.");
            break;

        case SPARM_MAGIC_RESISTANCE:
            mpr("You feel resistant to hostile enchantments.");
            break;

        case SPARM_PROTECTION:
            mpr("You feel protected.");
            break;

        case SPARM_STEALTH:
            mpr("You feel stealthy.");
            break;

        case SPARM_RESISTANCE:
            mpr("You feel resistant to extremes of temperature.");
            break;

        case SPARM_POSITIVE_ENERGY:
            mpr("Your life force is being protected.");
            break;

        case SPARM_ARCHMAGI:
            if (!you.skill(SK_SPELLCASTING))
                mpr("You feel strangely lacking in power.");
            else
                mpr("You feel powerful.");
            break;

        case SPARM_SPIRIT_SHIELD:
            if (player_spirit_shield() < 2)
            {
                set_mp(0, false);
                mpr("You feel spirits watching over you.");
                if (you.species == SP_DEEP_DWARF)
                    mpr("Now linked to your health, your magic stops regenerating.");
            }
            break;

        case SPARM_ARCHERY:
            mpr("You feel that your aim is more steady.");
            break;
        }
    }

    if (is_artefact(arm))
    {
        bool show_msgs = true;
        _equip_artefact_effect(arm, &show_msgs, unmeld);
    }

    if (get_item_slot(arm) == EQ_SHIELD)
        warn_shield_penalties();

    if (get_item_slot(arm) == EQ_BODY_ARMOUR)
        warn_armour_penalties();

    you.redraw_armour_class = true;
    you.redraw_evasion = true;
}

static void _unequip_armour_effect(item_def& item, bool meld)
{
    you.redraw_armour_class = true;
    you.redraw_evasion = true;

    switch (get_armour_ego_type(item))
    {
    case SPARM_RUNNING:
        if (!you.fishtail)
            mpr("You feel rather sluggish.");
        break;

    case SPARM_FIRE_RESISTANCE:
        mpr("\"Was it this warm in here before?\"");
        break;

    case SPARM_COLD_RESISTANCE:
        mpr("You catch a bit of a chill.");
        break;

    case SPARM_POISON_RESISTANCE:
        if (player_res_poison() <= 0)
            mpr("You feel less healthy.");
        break;

    case SPARM_SEE_INVISIBLE:
        if (!you.can_see_invisible())
            mpr("You feel less perceptive.");
        break;

    case SPARM_DARKNESS:
        if (you.duration[DUR_INVIS]
            && !you.attribute[ATTR_INVIS_UNCANCELLABLE]
            && !player_evokable_invis())
        {
            you.duration[DUR_INVIS] = 1;
        }
        break;

    case SPARM_STRENGTH:
        notify_stat_change(STAT_STR, -3, false, item, true);
        break;

    case SPARM_DEXTERITY:
        notify_stat_change(STAT_DEX, -3, false, item, true);
        break;

    case SPARM_INTELLIGENCE:
        notify_stat_change(STAT_INT, -3, false, item, true);
        break;

    case SPARM_PONDEROUSNESS:
        if (!meld)
        {
            mpr("That put a bit of spring back into your step.");
            che_handle_change(CB_PONDEROUS_COUNT, -1);
        }
        break;

    case SPARM_LEVITATION:
        if (you.attribute[ATTR_PERM_LEVITATION] == 0)
            break;
        else if (you.species != SP_KENKU || you.experience_level < 15)
        {
            if (!player_equip_ego_type(EQ_ALL_ARMOUR, SPARM_LEVITATION))
                you.attribute[ATTR_PERM_LEVITATION] = 0;
        }
        land_player();
        break;

    case SPARM_MAGIC_RESISTANCE:
        mpr("You feel less resistant to hostile enchantments.");
        break;

    case SPARM_PROTECTION:
        mpr("You feel less protected.");
        break;

    case SPARM_STEALTH:
        mpr("You feel less stealthy.");
        break;

    case SPARM_RESISTANCE:
        mpr("You feel hot and cold all over.");
        break;

    case SPARM_POSITIVE_ENERGY:
        mpr("You feel vulnerable.");
        break;

    case SPARM_ARCHMAGI:
        mpr("You feel strangely numb.");
        break;

    case SPARM_SPIRIT_SHIELD:
        if (!player_spirit_shield())
        {
            mpr("You feel strangely alone.");
            if (you.species == SP_DEEP_DWARF)
                mpr("Your magic begins regenerating once more.");
        }
        break;

    case SPARM_ARCHERY:
        mpr("Your aim is not that steady anymore.");
        break;

    default:
        break;
    }

    if (is_artefact(item))
        _unequip_artefact_effect(item, NULL, meld);
}

static void _remove_amulet_of_faith(item_def &item)
{
    if (you.religion != GOD_NO_GOD
        && you.religion != GOD_XOM)
    {
        simple_god_message(" seems less interested in you.");

        const int piety_loss = div_rand_round(you.piety, 3);
        // Piety penalty for removing the Amulet of Faith.
        if (you.piety - piety_loss > 10)
        {
            mprf(MSGCH_GOD,
                 "%s leaches power out of you as you remove it.",
                 item.name(DESC_CAP_YOUR).c_str());
            dprf("%s: piety leach: %d",
                 item.name(DESC_PLAIN).c_str(), piety_loss);
            lose_piety(piety_loss);
        }
    }
}

static void _equip_jewellery_effect(item_def &item, bool unmeld)
{
    // Randart jewellery shouldn't auto-ID just because the base type
    // is known. Somehow the player should still be told, preferably
    // by message. (jpeg)
    const bool artefact     = is_artefact(item);

    switch (item.sub_type)
    {
    case RING_FIRE:
    case RING_ICE:
    case RING_LIFE_PROTECTION:
    case RING_POISON_RESISTANCE:
    case RING_PROTECTION_FROM_COLD:
    case RING_PROTECTION_FROM_FIRE:
    case RING_PROTECTION_FROM_MAGIC:
    case RING_SUSTAIN_ABILITIES:
    case RING_SUSTENANCE:
    case RING_SLAYING:
    case RING_TELEPORT_CONTROL:
        break;

    case RING_WIZARDRY:
        break;

    case RING_SEE_INVISIBLE:
        // We might have to turn autopickup back on again.
        // TODO: Check all monsters in LOS. If any of them are invisible
        //       (and thus become visible once the ring is worn), the ring
        //       should be autoidentified.
        autotoggle_autopickup(false);
        break;

    case RING_PROTECTION:
        you.redraw_armour_class = true;
        break;

    case RING_INVISIBILITY:
        if (!you.duration[DUR_INVIS])
        {
            mpr("You become transparent for a moment.");
        }
        break;

    case RING_EVASION:
        you.redraw_evasion = true;
        break;

    case RING_STRENGTH:
        if (item.plus)
        {
            notify_stat_change(STAT_STR, item.plus, false, item);
        }
        break;

    case RING_DEXTERITY:
        if (item.plus)
        {
            notify_stat_change(STAT_DEX, item.plus, false, item);
        }
        break;

    case RING_INTELLIGENCE:
        if (item.plus)
        {
            notify_stat_change(STAT_INT, item.plus, false, item);
        }
        break;

    case RING_MAGICAL_POWER:
        if ((you.max_magic_points + 9) *
            (1.0+player_mutation_level(MUT_HIGH_MAGIC)/10.0) > 50)
            mpr("You feel your mana capacity is already quite full.");
        else
            canned_msg(MSG_MANA_INCREASE);

        calc_mp();
        break;

    case RING_LEVITATION:
        if (!scan_artefacts(ARTP_LEVITATE))
        {
            if (you.airborne())
                mpr("You feel vaguely more buoyant than before.");
            else
                mpr("You feel buoyant.");
        }
        break;

    case RING_TELEPORTATION:
        if (crawl_state.game_is_sprint())
            mpr("You feel a slight, muted jump rush through you.");
        else
            mpr("You feel slightly jumpy.");
        break;

    case AMU_RAGE:
        if (!scan_artefacts(ARTP_BERSERK))
        {
            mpr("You feel a brief urge to hack something to bits.");
        }
        break;

    case AMU_FAITH:
        if (you.religion != GOD_NO_GOD)
        {
            mpr("You feel a surge of divine interest.", MSGCH_GOD);
        }
        break;

    case AMU_CONTROLLED_FLIGHT:
        break;

    case AMU_GUARDIAN_SPIRIT:
        if (player_spirit_shield() < 2)
        {
            set_mp(0, false);
            mpr("You feel your power drawn to a protective spirit.");
            if (you.species == SP_DEEP_DWARF)
                mpr("Now linked to your health, your magic stops regenerating.");
        }
        break;

    case RING_REGENERATION:
        break;

    case AMU_STASIS:
        // Berserk is possible with a Battlelust card or with a moth of wrath
        // that affects you while donning the amulet.
        int amount = you.duration[DUR_HASTE] + you.duration[DUR_SLOW]
                     + you.duration[DUR_BERSERK] + you.duration[DUR_FINESSE];
        if (you.duration[DUR_TELEPORT])
            amount += 30 + random2(150);
        if (amount)
        {
            mprf("The amulet engulfs you in a%s magical discharge!",
                 (amount > 250) ? " massive" :
                 (amount >  50) ? " violent" :
                                  "");

            contaminate_player(pow(amount, 0.333), true);

            int dir = 0;
            if (you.duration[DUR_HASTE])
                dir++;
            if (you.duration[DUR_SLOW])
                dir--;
            if (dir > 0)
                mpr("You abruptly slow down.", MSGCH_DURATION);
            else if (dir < 0)
                mpr("Your slowness suddenly goes away.", MSGCH_DURATION);
            if (you.duration[DUR_TELEPORT])
                mpr("You feel strangely stable.", MSGCH_DURATION);
            if (you.duration[DUR_BERSERK])
                mpr("You violently calm down.", MSGCH_DURATION);
            // my thesaurus says this usage is correct
            if (you.duration[DUR_FINESSE])
                mpr("Your hands get arrested.", MSGCH_DURATION);
            you.duration[DUR_HASTE] = 0;
            you.duration[DUR_SLOW] = 0;
            you.duration[DUR_TELEPORT] = 0;
            you.duration[DUR_BERSERK] = 0;
            you.duration[DUR_FINESSE] = 0;
        }
        break;

    // When making a jewel type auto-id, please update Ashenzari's list
    // in godpassive.cc as well.
    }

    // Artefacts have completely different appearance than base types
    // so we don't allow them to make the base types known.
    if (artefact)
    {
        bool show_msgs = true;
        _equip_artefact_effect(item, &show_msgs, unmeld);
    }

    mpr(item.name(DESC_INVENTORY_EQUIP).c_str());
}

static void _unequip_jewellery_effect(item_def &item, bool mesg)
{
    // The ring/amulet must already be removed from you.equip at this point.

    switch (item.sub_type)
    {
    case RING_FIRE:
    case RING_ICE:
    case RING_LIFE_PROTECTION:
    case RING_POISON_RESISTANCE:
    case RING_PROTECTION_FROM_COLD:
    case RING_PROTECTION_FROM_FIRE:
    case RING_PROTECTION_FROM_MAGIC:
    case RING_REGENERATION:
    case RING_SEE_INVISIBLE:
    case RING_SLAYING:
    case RING_SUSTAIN_ABILITIES:
    case RING_SUSTENANCE:
    case RING_TELEPORTATION:
    case RING_WIZARDRY:
    case RING_TELEPORT_CONTROL:
        break;

    case RING_PROTECTION:
        you.redraw_armour_class = true;
        break;

    case RING_EVASION:
        you.redraw_evasion = true;
        break;

    case RING_STRENGTH:
        notify_stat_change(STAT_STR, -item.plus, false, item, true);
        break;

    case RING_DEXTERITY:
        notify_stat_change(STAT_DEX, -item.plus, false, item, true);
        break;

    case RING_INTELLIGENCE:
        notify_stat_change(STAT_INT, -item.plus, false, item, true);
        break;

    case RING_LEVITATION:
        if (you.duration[DUR_LEVITATION] && !you.permanent_levitation()
            && !you.attribute[ATTR_LEV_UNCANCELLABLE]
            && !player_evokable_levitation())
        {
            you.duration[DUR_LEVITATION] = 0;
            land_player();
        }
        break;

    case RING_INVISIBILITY:
        if (you.duration[DUR_INVIS]
            && !you.attribute[ATTR_INVIS_UNCANCELLABLE]
            && !player_evokable_invis())
        {
            you.duration[DUR_INVIS] = 1;
        }
        break;

    case RING_MAGICAL_POWER:
        canned_msg(MSG_MANA_DECREASE);
        break;

    case AMU_FAITH:
        _remove_amulet_of_faith(item);
        break;

    case AMU_GUARDIAN_SPIRIT:
        if (you.species == SP_DEEP_DWARF)
            mpr("Your magic begins regenerating once more.");
        break;
    }

    if (is_artefact(item))
        _unequip_artefact_effect(item, &mesg);

    // Must occur after ring is removed. -- bwr
    calc_mp();
}

bool unwield_item(bool showMsgs)
{
    if (!you.weapon())
        return (false);

    if (you.berserk())
    {
        if (showMsgs)
            canned_msg(MSG_TOO_BERSERK);
        return (false);
    }

    item_def& item = *you.weapon();

    const bool is_weapon = get_item_slot(item) == EQ_WEAPON;

    if (is_weapon && !safe_to_remove(item))
        return (false);

    unequip_item(EQ_WEAPON, showMsgs);

    you.wield_change     = true;
    you.redraw_quiver    = true;
    you.attribute[ATTR_WEAPON_SWAP_INTERRUPTED] = 0;

    return (true);
}
