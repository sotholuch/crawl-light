/**
 * @file
 * @brief Potion and potion-like effects.
**/

#include "AppHdr.h"

#include "potion.h"

#include <stdio.h>
#include <string.h>

#include "externs.h"

#include "areas.h"
#include "artefact.h"
#include "beam.h"
#include "effects.h"
#include "env.h"
#include "exercise.h"
#include "food.h"
#include "godabil.h"
#include "godconduct.h"
#include "godwrath.h"
#include "hints.h"
#include "item_use.h"
#include "itemprop.h"
#include "misc.h"
#include "mutation.h"
#include "player.h"
#include "player-equip.h"
#include "player-stats.h"
#include "spl-miscast.h"
#include "stuff.h"
#include "terrain.h"
#include "transform.h"
#include "xom.h"

/*
 * Apply the effect of a potion to the player.
 *
 * This is called when the player quaff a potion, but also for some cards,
 * beams, sparkling fountains, god effects and miscasts.
 *
 * @param pot_eff        The potion type.
 * @param pow            The power of the effect. 40 for actual potions.
 * @param drank_it       Whether the player actually quaffed (potions and fountains).
 * @param already_warned Whether the user was already warned about overcontamination.
 *
 * @return If the effect was used, rather than aborted.
 */
bool potion_effect(potion_type pot_eff, int pow, bool drank_it, bool already_warned)
{
    pow = std::min(pow, 150);

    //Used to be that vampires could have reduced effects; this variable covered it.
    int factor = 1;

    // Knowingly drinking bad potions is much less amusing.
    int xom_factor = factor;
    if (drank_it)
    {
        xom_factor *= 2;
        if (!player_in_a_dangerous_place())
            xom_factor *= 3;
    }

    switch (pot_eff)
    {
    case POT_CURING:
        if (you.duration[DUR_DEATHS_DOOR])
        {
            mpr("You feel queasy.");
            break;
        }

        if(you.challenge != CHALLENGE_VEHUMET)
        {
            inc_hp((5 + random2(7)) / factor, false);
            mpr("You feel better.");
            // Only fix rot when healed to full.
            if (you.hp == you.hp_max)
            {
                unrot_hp(1);
                set_hp(you.hp_max, false);
            }
        }
        else
        {
            mpr("You don't feel much better.");
        }

        you.duration[DUR_POISONING] = 0;
        you.rotting = 0;
        you.disease = 0;
        you.duration[DUR_CONF] = 0;
        you.duration[DUR_MISLED] = 0;
        break;

    case POT_HEAL_WOUNDS:
        if (you.duration[DUR_DEATHS_DOOR])
        {
            mpr("You feel queasy.");
            break;
        }
        else if (you.challenge == CHALLENGE_VEHUMET)
        {
            mpr("You don't feel any better.");
            break;
        }

        inc_hp((10 + random2avg(28, 3)) / factor, false);
        mpr("You feel much better.");

        // only fix rot when healed to full
        if (you.hp == you.hp_max)
        {
            unrot_hp((2 + random2avg(5, 2)) / factor);
            set_hp(you.hp_max, false);
        }
        break;

    case POT_SPEED:
        // Actual contamination is handled in haste_player.
        if (!already_warned && !contamination_warning_prompt(HASTE_GLOW_COST))
        {
            canned_msg(MSG_OK);
            return false;
        }
        if (haste_player((40 + random2(pow)) / factor))
            did_god_conduct(DID_HASTY, 10, true);
        break;

    case POT_MIGHT:
    {
        // Might always incurs some glow cost.
        if (!already_warned && !contamination_warning_prompt(MIGHT_GLOW_COST))
        {
            canned_msg(MSG_OK);
            return false;
        }
        contaminate_player(MIGHT_GLOW_COST, true);
        const bool were_mighty = you.duration[DUR_MIGHT] > 0;

        mprf(MSGCH_DURATION, "You feel %s all of a sudden.",
             were_mighty ? "mightier" : "very mighty");

        // conceivable max gain of +184 {dlb}
        you.increase_duration(DUR_MIGHT, (35 + random2(pow)) / factor, 80);

        if (!were_mighty)
            notify_stat_change(STAT_STR, 5, true, "");
        break;
    }

    case POT_BRILLIANCE:
    {
        // Brilliance always incurs some glow cost.
        if (!already_warned && !contamination_warning_prompt(BRILLIANCE_GLOW_COST))
        {
            canned_msg(MSG_OK);
            return false;
        }
        contaminate_player(BRILLIANCE_GLOW_COST, true);
        const bool were_brilliant = you.duration[DUR_BRILLIANCE] > 0;

        mprf(MSGCH_DURATION, "You feel %s all of a sudden.",
             were_brilliant ? "clever" : "more clever");

        you.increase_duration(DUR_BRILLIANCE,
                              (35 + random2(pow)) / factor, 80);

        if (!were_brilliant)
            notify_stat_change(STAT_INT, 5, true, "");
        break;
    }

    case POT_AGILITY:
    {
        // Agility always incurs some glow cost.
        if (!already_warned && !contamination_warning_prompt(AGILITY_GLOW_COST))
        {
            canned_msg(MSG_OK);
            return false;
        }
        contaminate_player(AGILITY_GLOW_COST, true);
        const bool were_agile = you.duration[DUR_AGILITY] > 0;

        mprf(MSGCH_DURATION, "You feel %s all of a sudden.",
             were_agile ? "agile" : "more agile");
        
        int duration = (35 + random2(pow)) / factor;
        you.increase_duration(DUR_AGILITY, duration, 80);
        you.increase_duration(DUR_SWIFTNESS, duration, 80);

        if (!were_agile)
            notify_stat_change(STAT_DEX, 5, true, "");
        break;
    }

    case POT_GAIN_STRENGTH:
        if (mutate(MUT_STRONG, true, false, false, true))
            learned_something_new(HINT_YOU_MUTATED);
        break;

    case POT_GAIN_DEXTERITY:
        if (mutate(MUT_AGILE, true, false, false, true))
            learned_something_new(HINT_YOU_MUTATED);
        break;

    case POT_GAIN_INTELLIGENCE:
        if (mutate(MUT_CLEVER, true, false, false, true))
            learned_something_new(HINT_YOU_MUTATED);
        break;

    case POT_LEVITATION:
        if (liquefied(you.pos()) && you.ground_level())
        {
            mprf(MSGCH_WARN, "This potion isn't strong enough to pull you from the ground!");
            break;
        }

        you.attribute[ATTR_LEV_UNCANCELLABLE] = 1;
        levitate_player(pow);
        break;

    case POT_POISON:
    case POT_STRONG_POISON:
        if (player_res_poison())
        {
            mprf("You feel %s nauseous.",
                 (pot_eff == POT_POISON) ? "slightly" : "quite");
        }
        else
        {
            mprf(MSGCH_WARN,
                 "That liquid tasted %s nasty...",
                 (pot_eff == POT_POISON) ? "very" : "extremely");

            if (pot_eff == POT_POISON)
                poison_player(1 + random2avg(5, 2), "", "a potion of poison");
            else
                poison_player(3 + random2avg(13, 2), "", "a potion of strong poison");
            xom_is_stimulated(100 / xom_factor);
        }
        break;

    case POT_SLOWING:
        if (slow_player((10 + random2(pow)) / factor))
            xom_is_stimulated(50 / xom_factor);
        break;

    case POT_PARALYSIS:
        you.paralyse(NULL,
                     (2 + random2(6 + you.duration[DUR_PARALYSIS]
                                       / BASELINE_DELAY)) / factor);
        xom_is_stimulated(50 / xom_factor);
        break;

    case POT_CONFUSION:
        if (confuse_player((3 + random2(8)) / factor))
            xom_is_stimulated(100 / xom_factor);
        break;

    case POT_INVISIBILITY:
        // Invisibility is very costly!
        if (!already_warned && !contamination_warning_prompt(INVIS_GLOW_COST))
        {
            canned_msg(MSG_OK);
            return false;
        }
        contaminate_player(INVIS_GLOW_COST, true);
        if (you.haloed() || you.glows_naturally())
        {
            // You can't turn invisible while haloed or glowing
            // naturally, but identify the effect anyway.
            mpr("You briefly turn translucent.");

            // And also cancel corona (for whatever good that will do).
            you.duration[DUR_CORONA] = 0;
            return (true);
        }

        if (get_contamination_level() > 1)
        {
            mprf(MSGCH_DURATION,
                 "You become %stransparent, but the glow from your "
                 "magical contamination prevents you from becoming "
                 "completely invisible.",
                 you.duration[DUR_INVIS] ? "more " : "");
        }
        else
        {
            mpr(!you.duration[DUR_INVIS] ? "You fade into invisibility!"
                                         : "You fade further into invisibility.",
                MSGCH_DURATION);
        }

        // Invisibility cancels corona.
        you.duration[DUR_CORONA] = 0;

        // Now multiple invisiblity casts aren't as good. -- bwr
        if (!you.duration[DUR_INVIS])
            you.set_duration(DUR_INVIS, 15 + random2(pow), 100);
        else
            you.increase_duration(DUR_INVIS, random2(pow), 100);

        if (drank_it)
            you.attribute[ATTR_INVIS_UNCANCELLABLE] = 1;

        break;

    case POT_DEGENERATION:
        if (drank_it)
            mpr("There was something very wrong with that liquid!");

        if (lose_stat(STAT_RANDOM, (1 + random2avg(4, 2)) / factor, false,
                      "drinking a potion of degeneration"))
        {
            xom_is_stimulated(50 / xom_factor);
        }
        break;

    // Don't generate randomly - should be rare and interesting.
    case POT_DECAY:
        if (you.rot(&you, (10 + random2(10)) / factor))
            xom_is_stimulated(50 / xom_factor);
        break;

    case POT_FIZZING:
    case POT_WATER:
        if (you.species == SP_VAMPIRE)
            mpr("Blech - this tastes like water.");
        else
            mpr("This tastes like water.");
        break;

    case POT_EXPERIENCE:
        if (you.experience_level < 27)
        {
            mpr("You feel more experienced!");
            adjust_level(1, true);

            // Deferred calling level_change() into item_use.cc:3919, after
            // dec_inv_item_quantity. This prevents using SIGHUP to get infinite
            // potions of experience. Confer Mantis #3245. [due]
        }
        else if (you.challenge != CHALLENGE_SIF_MUNA)
            mpr("A flood of memories washes over you.");
        else
            mpr("A flood of useless memories washes over you.");
        
        if (you.challenge != CHALLENGE_SIF_MUNA)
            you.exp_available += 750 * you.experience_level;
        autotrain();
        break;

    case POT_MAGIC:
        inc_mp((10 + random2avg(28, 3)), false);
        mpr("Magic courses through your body.");
        break;

    case POT_RESTORE_ABILITIES:
    {
        bool nothing_happens = true;
        if (you.duration[DUR_BREATH_WEAPON])
        {
            mpr("You have got your breath back.", MSGCH_RECOVERY);
            you.duration[DUR_BREATH_WEAPON] = 0;
            nothing_happens = false;
        }

        // Give a message if no message otherwise.
        if (!restore_stat(STAT_ALL, 0, false) && nothing_happens)
            mpr("You feel refreshed.");
        break;
    }

    case POT_BERSERK_RAGE:
        // Actual contamination is handled in go_berserk.
        if (!already_warned && !contamination_warning_prompt(BERSERK_GLOW_COST))
        {
            canned_msg(MSG_OK);
            return false;
        }
        if (go_berserk(true, true))
            xom_is_stimulated(50);
        break;

    case POT_CURE_MUTATION:
        mpr("It has a very clean taste.");
        for (int i = 0; i < 7; i++)
            if (random2(9) >= i)
                delete_mutation(RANDOM_MUTATION, false);
        break;

    case POT_MUTATION:
        mpr("You feel extremely strange.");
        for (int i = 0; i < 3; i++)
            mutate(RANDOM_MUTATION, false);

        learned_something_new(HINT_YOU_MUTATED);
        did_god_conduct(DID_DELIBERATE_MUTATING, 10, true);
        break;

    case POT_RESISTANCE:
        if (!already_warned && !contamination_warning_prompt(1))
        {
            canned_msg(MSG_OK);
            return false;
        }
        mpr("You feel protected.", MSGCH_DURATION);
        you.increase_duration(DUR_RESIST_FIRE,   (random2(pow) + 35) / factor);
        you.increase_duration(DUR_RESIST_COLD,   (random2(pow) + 35) / factor);
        you.increase_duration(DUR_RESIST_POISON, (random2(pow) + 35) / factor);
        you.increase_duration(DUR_INSULATION,    (random2(pow) + 35) / factor);

        // Just one point of contamination. These potions are really rare,
        // and contamination is nastier.
        contaminate_player(1, true);
        break;

    case NUM_POTIONS:
        mpr("You feel bugginess flow through your body.");
        break;
    }
    
    //Beogh followers with sufficient piety will share their potions.
    if(drank_it)
        beogh_share_potion(pot_eff);

    return true;
}
