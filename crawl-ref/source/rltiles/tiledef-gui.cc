// This file has been automatically generated.

#include "tiledef-gui.h"

#include <string>
#include <cstring>
#include <cassert>
using namespace std;

unsigned int _tile_gui_count[TILEG_GUI_MAX - 0] =
{
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    3,
    1,
    1,
    3,
    1,
    1,
    3,
    1,
    1,
    3,
    1,
    1,
    3,
    1,
    1,
    3,
    1,
    1,
};

unsigned int tile_gui_count(tileidx_t idx)
{
    assert(idx >= 0 && idx < TILEG_GUI_MAX);
    return _tile_gui_count[idx - 0];
}

tileidx_t _tile_gui_basetiles[TILEG_GUI_MAX - 0] =
{
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    25,
    26,
    27,
    28,
    29,
    30,
    31,
    32,
    33,
    34,
    35,
    36,
    37,
    38,
    39,
    40,
    41,
    42,
    43,
    44,
    45,
    46,
    47,
    48,
    49,
    50,
    51,
    52,
    53,
    54,
    55,
    56,
    57,
    58,
    59,
    60,
    61,
    62,
    63,
    64,
    65,
    66,
    67,
    68,
    69,
    70,
    71,
    72,
    73,
    74,
    75,
    76,
    77,
    78,
    79,
    80,
    81,
    82,
    83,
    84,
    85,
    86,
    87,
    88,
    89,
    90,
    91,
    92,
    93,
    94,
    95,
    96,
    97,
    98,
    99,
    100,
    101,
    102,
    103,
    104,
    105,
    106,
    107,
    108,
    109,
    110,
    111,
    112,
    113,
    114,
    115,
    116,
    117,
    118,
    119,
    120,
    121,
    122,
    123,
    124,
    125,
    126,
    127,
    128,
    129,
    130,
    131,
    132,
    133,
    134,
    135,
    136,
    137,
    138,
    139,
    140,
    141,
    142,
    143,
    144,
    145,
    146,
    147,
    148,
    149,
    150,
    151,
    152,
    153,
    154,
    155,
    156,
    157,
    158,
    159,
    160,
    161,
    162,
    163,
    164,
    165,
    166,
    167,
    168,
    169,
    170,
    171,
    172,
    173,
    174,
    175,
    176,
    177,
    178,
    179,
    180,
    181,
    182,
    183,
    184,
    185,
    186,
    187,
    188,
    189,
    190,
    191,
    192,
    193,
    194,
    195,
    196,
    197,
    198,
    199,
    200,
    201,
    202,
    203,
    204,
    205,
    206,
    207,
    208,
    209,
    210,
    211,
    212,
    213,
    214,
    215,
    216,
    217,
    218,
    219,
    220,
    221,
    222,
    223,
    224,
    225,
    226,
    227,
    228,
    229,
    230,
    231,
    232,
    233,
    234,
    235,
    236,
    237,
    238,
    239,
    240,
    241,
    242,
    243,
    244,
    245,
    246,
    247,
    248,
    249,
    250,
    251,
    252,
    253,
    254,
    255,
    256,
    257,
    257,
    257,
    260,
    260,
    260,
    263,
    263,
    263,
    266,
    266,
    266,
    269,
    269,
    269,
    272,
    272,
    272,
};

tileidx_t tile_gui_basetile(tileidx_t idx)
{
    assert(idx >= 0 && idx < TILEG_GUI_MAX);
    return _tile_gui_basetiles[idx - 0];
}

int _tile_gui_probs[TILEG_GUI_MAX - 0] =
{
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    3,
    1,
    2,
    3,
    1,
    2,
    3,
    1,
    2,
    3,
    1,
    2,
    3,
    1,
    2,
    3,
};

int tile_gui_probs(tileidx_t idx)
{
    assert(idx >= 0 && idx < TILEG_GUI_MAX);
    return _tile_gui_probs[idx - 0];
}

const char *_tile_gui_name[TILEG_GUI_MAX - 0] =
{
    "GUI_FILLER_0",
    "STARTUP_STONESOUP",
    "STARTUP_TUTORIAL",
    "STARTUP_CHALLENGE",
    "STARTUP_HINTS",
    "STARTUP_SPRINT",
    "STARTUP_ZOTDEF",
    "STARTUP_INSTRUCTIONS",
    "STARTUP_ARENA",
    "TAVERN",
    "SPELL_AIR",
    "SPELL_CONJURATION",
    "SPELL_DIVINATION",
    "SPELL_EARTH",
    "SPELL_ENCHANTMENT",
    "SPELL_FIRE",
    "SPELL_ICE",
    "SPELL_NECROMANCY",
    "SPELL_POISON",
    "SPELL_SUMMONING",
    "SPELL_TRANSLOCATION",
    "SPELL_TRANSMUTATION",
    "AIRSTRIKE",
    "CHAIN_LIGHTNING",
    "CONJURE_BALL_LIGHTNING",
    "DEFLECT_MISSILES",
    "FLIGHT",
    "INSULATION",
    "LEVITATION",
    "LIGHTNING_BOLT",
    "REPEL_MISSILES",
    "SHOCK",
    "SILENCE",
    "STATIC_DISCHARGE",
    "SWIFTNESS",
    "TORNADO",
    "ISKENDERUNS_MYSTIC_BLAST",
    "MAGIC_DART",
    "IOOD",
    "IRON_SHOT",
    "DIG",
    "LEES_RAPID_DECONSTRUCTION",
    "LEHUDIBS_CRYSTAL_SPEAR",
    "LEDAS_LIQUEFACTION",
    "PASSWALL",
    "PETRIFY",
    "SANDBLAST",
    "SHATTER",
    "STATUE_FORM",
    "STONE_ARROW",
    "STONESKIN",
    "BERSERKER_RAGE",
    "CAUSE_FEAR",
    "CONFUSE",
    "CONFUSING_TOUCH",
    "CORONA",
    "DARKNESS",
    "ENSLAVEMENT",
    "EXTENSION",
    "HASTE",
    "INVISIBILITY",
    "MASS_CONFUSION",
    "PROJECTED_NOISE",
    "SEE_INVISIBLE",
    "SLOW",
    "SURE_BLADE",
    "TUKIMAS_DANCE",
    "SELECTIVE_AMNESIA",
    "BOLT_OF_FIRE",
    "BOLT_OF_MAGMA",
    "CONJURE_FLAME",
    "DELAYED_FIREBALL",
    "DRAGON_FORM",
    "EVAPORATE",
    "FIRE_BRAND",
    "FIRE_STORM",
    "FIREBALL",
    "FLAME_TONGUE",
    "IGNITE_POISON",
    "RING_OF_FLAMES",
    "STICKY_FLAME",
    "THROW_FLAME",
    "INNER_FLAME",
    "BOLT_OF_COLD",
    "CONDENSATION_SHIELD",
    "ENSORCELLED_HIBERNATION",
    "FREEZE",
    "FREEZING_AURA",
    "FREEZING_CLOUD",
    "THROW_ICICLE",
    "ICE_FORM",
    "ICE_STORM",
    "METABOLIC_ENGLACIATION",
    "OZOCUBUS_ARMOUR",
    "OZOCUBUS_REFRIGERATION",
    "THROW_FROST",
    "AGONY",
    "ANIMATE_DEAD",
    "ANIMATE_SKELETON",
    "BOLT_OF_DRAINING",
    "BORGNJORS_REVIVIFICATION",
    "CIGOTUVIS_DEGENERATION",
    "CONTROL_UNDEAD",
    "CORPSE_ROT",
    "DEATH_CHANNEL",
    "DEATHS_DOOR",
    "DISPEL_UNDEAD",
    "EXCRUCIATING_WOUNDS",
    "FULSOME_DISTILLATION",
    "HAUNT",
    "LETHAL_INFUSION",
    "NECROMUTATION",
    "PAIN",
    "REGENERATION",
    "SIMULACRUM",
    "SUBLIMATION_OF_BLOOD",
    "SYMBOL_OF_TORMENT",
    "TWISTED_RESURRECTION",
    "VAMPIRIC_DRAINING",
    "ALISTAIRS_INTOXICATION",
    "CURE_POISON",
    "MEPHITIC_CLOUD",
    "OLGREBS_TOXIC_RADIANCE",
    "POISON_ARROW",
    "POISON_BRAND",
    "POISONOUS_CLOUD",
    "RESIST_POISON",
    "SPIDER_FORM",
    "STING",
    "VENOM_BOLT",
    "ILLUDE",
    "TOXIC_SHIELD",
    "ABJURATION",
    "CALL_CANINE_FAMILIAR",
    "CALL_IMP",
    "DEMONIC_HORDE",
    "MALIGN_GATEWAY",
    "RECALL",
    "STICKS_TO_SNAKES",
    "SUMMON_BUTTERFLIES",
    "SUMMON_DEMON",
    "SUMMON_DRAGON",
    "SUMMON_ELEMENTAL",
    "SUMMON_GREATER_DEMON",
    "SUMMON_HORRIBLE_THINGS",
    "SUMMON_HYDRA",
    "SUMMON_ICE_BEAST",
    "SUMMON_SCORPIONS",
    "SUMMON_SHADOW_CREATURES",
    "SUMMON_SMALL_MAMMALS",
    "SUMMON_UGLY_THING",
    "APPORTATION",
    "BANISHMENT",
    "BLINK",
    "CONTROLLED_BLINK",
    "CONTROLLED_TELEPORT",
    "DISPERSAL",
    "PASSAGE_OF_GOLUBRIA",
    "PHASE_SHIFT",
    "PORTAL_PROJECTILE",
    "TELEPORT",
    "TELEPORT_OTHER",
    "WARP_WEAPON",
    "ALTER_SELF",
    "BLADE_HANDS",
    "POLYMORPH_OTHER",
    "HOMUNCULUS",
    "MEMORISE",
    "FIGHTING_ON",
    "FIGHTING_OFF",
    "SHORT_BLADES_ON",
    "SHORT_BLADES_OFF",
    "LONG_BLADES_ON",
    "LONG_BLADES_OFF",
    "AXES_ON",
    "AXES_OFF",
    "MACES_FLAILS_ON",
    "MACES_FLAILS_OFF",
    "POLEARMS_ON",
    "POLEARMS_OFF",
    "STAVES_ON",
    "STAVES_OFF",
    "SLINGS_ON",
    "SLINGS_OFF",
    "BOWS_ON",
    "BOWS_OFF",
    "CROSSBOWS_ON",
    "CROSSBOWS_OFF",
    "THROWING_ON",
    "THROWING_OFF",
    "ARMOUR_ON",
    "ARMOUR_OFF",
    "DODGING_ON",
    "DODGING_OFF",
    "STEALTH_ON",
    "STEALTH_OFF",
    "STABBING_ON",
    "STABBING_OFF",
    "SHIELDS_ON",
    "SHIELDS_OFF",
    "TRAPS_DOORS_ON",
    "TRAPS_DOORS_OFF",
    "UNARMED_COMBAT_ON",
    "UNARMED_COMBAT_OFF",
    "SPELLCASTING_ON",
    "SPELLCASTING_OFF",
    "CONJURATIONS_ON",
    "CONJURATIONS_OFF",
    "HEXES_ON",
    "HEXES_OFF",
    "CHARMS_ON",
    "CHARMS_OFF",
    "SUMMONINGS_ON",
    "SUMMONINGS_OFF",
    "NECROMANCY_ON",
    "NECROMANCY_OFF",
    "TRANSLOCATIONS_ON",
    "TRANSLOCATIONS_OFF",
    "TRANSMUTATIONS_ON",
    "TRANSMUTATIONS_OFF",
    "FIRE_MAGIC_ON",
    "FIRE_MAGIC_OFF",
    "ICE_MAGIC_ON",
    "ICE_MAGIC_OFF",
    "AIR_MAGIC_ON",
    "AIR_MAGIC_OFF",
    "EARTH_MAGIC_ON",
    "EARTH_MAGIC_OFF",
    "POISON_MAGIC_ON",
    "POISON_MAGIC_OFF",
    "INVOCATIONS_ON",
    "INVOCATIONS_OFF",
    "EVOCATIONS_ON",
    "EVOCATIONS_OFF",
    "CMD_REST",
    "CMD_EXPLORE",
    "CMD_INTERLEVEL_TRAVEL",
    "CMD_AUTOFIGHT",
    "CMD_SEARCH",
    "CMD_USE_ABILITY",
    "CMD_PRAY",
    "CMD_SEARCH_STASHES",
    "CMD_REPLAY_MESSAGES",
    "CMD_RESISTS_SCREEN",
    "CMD_DISPLAY_OVERMAP",
    "CMD_DISPLAY_RELIGION",
    "CMD_DISPLAY_MUTATIONS",
    "CMD_DISPLAY_SKILLS",
    "CMD_DISPLAY_CHARACTER_STATUS",
    "CMD_KNOWN_ITEMS",
    "CMD_SAVE_GAME_NOW",
    "CMD_EDIT_PLAYER_TILE",
    "CMD_DISPLAY_COMMANDS",
    "CMD_CHARACTER_DUMP",
    "CMD_CYCLE_QUIVER_FORWARD",
    "TODO",
    "ERROR",
    "TAB_ITEM",
    "TAB_ITEM_1",
    "TAB_ITEM_2",
    "TAB_SPELL",
    "TAB_SPELL_1",
    "TAB_SPELL_2",
    "TAB_MEMORISE",
    "TAB_MEMORISE_1",
    "TAB_MEMORISE_2",
    "TAB_MONSTER",
    "TAB_MONSTER_1",
    "TAB_MONSTER_2",
    "TAB_SKILL",
    "TAB_SKILL_1",
    "TAB_SKILL_2",
    "TAB_COMMAND",
    "TAB_COMMAND_1",
    "TAB_COMMAND_2",
};

const char *tile_gui_name(tileidx_t idx)
{
    assert(idx >= 0 && idx < TILEG_GUI_MAX);
    return _tile_gui_name[idx - 0];
}

tile_info _tile_gui_info[TILEG_GUI_MAX - 0] =
{
    tile_info(32, 32, 0, 0, 0, 0, 32, 32),
    tile_info(32, 32, 1, 0, 32, 0, 62, 32),
    tile_info(32, 32, 4, 2, 62, 0, 85, 29),
    tile_info(32, 32, 1, 0, 85, 0, 115, 32),
    tile_info(32, 32, 8, 5, 115, 0, 131, 25),
    tile_info(32, 32, 5, 3, 131, 0, 155, 26),
    tile_info(32, 32, 5, 3, 155, 0, 177, 27),
    tile_info(32, 32, 3, 3, 177, 0, 203, 26),
    tile_info(32, 32, 5, 1, 203, 0, 227, 29),
    tile_info(32, 32, 1, 1, 227, 0, 257, 28),
    tile_info(32, 32, 0, 0, 257, 0, 289, 32),
    tile_info(32, 32, 0, 0, 289, 0, 321, 32),
    tile_info(32, 32, 0, 0, 321, 0, 353, 32),
    tile_info(32, 32, 0, 0, 353, 0, 385, 32),
    tile_info(32, 32, 0, 0, 385, 0, 417, 32),
    tile_info(32, 32, 0, 0, 417, 0, 449, 32),
    tile_info(32, 32, 0, 0, 449, 0, 481, 32),
    tile_info(32, 32, 0, 0, 481, 0, 513, 32),
    tile_info(32, 32, 0, 0, 513, 0, 545, 32),
    tile_info(32, 32, 0, 0, 545, 0, 577, 32),
    tile_info(32, 32, 0, 0, 577, 0, 609, 32),
    tile_info(32, 32, 0, 0, 609, 0, 641, 32),
    tile_info(32, 32, 4, 4, 641, 0, 667, 26),
    tile_info(32, 32, 2, 2, 667, 0, 694, 28),
    tile_info(32, 32, 3, 3, 694, 0, 721, 26),
    tile_info(32, 32, 3, 3, 721, 0, 748, 27),
    tile_info(32, 32, 5, 2, 748, 0, 772, 28),
    tile_info(32, 32, 2, 2, 772, 0, 798, 28),
    tile_info(32, 32, 8, 2, 798, 0, 814, 28),
    tile_info(32, 32, 5, 4, 814, 0, 839, 26),
    tile_info(32, 32, 3, 2, 839, 0, 865, 28),
    tile_info(32, 32, 7, 6, 865, 0, 884, 20),
    tile_info(32, 32, 3, 3, 884, 0, 910, 27),
    tile_info(32, 32, 2, 2, 910, 0, 938, 28),
    tile_info(32, 32, 5, 3, 938, 0, 963, 27),
    tile_info(32, 32, 3, 4, 963, 0, 990, 28),
    tile_info(32, 32, 2, 2, 990, 0, 1017, 28),
    tile_info(32, 32, 7, 6, 0, 32, 19, 52),
    tile_info(32, 32, 0, 0, 19, 32, 51, 64),
    tile_info(32, 32, 4, 5, 51, 32, 76, 57),
    tile_info(32, 32, 4, 13, 76, 32, 102, 49),
    tile_info(32, 32, 2, 2, 102, 32, 129, 59),
    tile_info(32, 32, 4, 3, 129, 32, 154, 57),
    tile_info(32, 32, 1, 6, 154, 32, 183, 53),
    tile_info(32, 32, 5, 5, 183, 32, 205, 57),
    tile_info(32, 32, 11, 3, 205, 32, 215, 59),
    tile_info(32, 32, 4, 4, 215, 32, 239, 56),
    tile_info(32, 32, 2, 2, 239, 32, 266, 60),
    tile_info(32, 32, 4, 2, 266, 32, 290, 60),
    tile_info(32, 32, 7, 8, 290, 32, 310, 52),
    tile_info(32, 32, 8, 3, 310, 32, 326, 58),
    tile_info(32, 32, 6, 3, 326, 32, 346, 58),
    tile_info(32, 32, 6, 4, 346, 32, 366, 58),
    tile_info(32, 32, 3, 2, 366, 32, 392, 60),
    tile_info(32, 32, 2, 3, 392, 32, 420, 59),
    tile_info(32, 32, 4, 2, 420, 32, 444, 59),
    tile_info(32, 32, 5, 2, 444, 32, 466, 60),
    tile_info(32, 32, 2, 3, 466, 32, 493, 59),
    tile_info(32, 32, 2, 4, 493, 32, 520, 58),
    tile_info(32, 32, 5, 3, 520, 32, 544, 59),
    tile_info(32, 32, 7, 2, 544, 32, 562, 60),
    tile_info(32, 32, 4, 3, 562, 32, 587, 57),
    tile_info(32, 32, 2, 2, 587, 32, 615, 60),
    tile_info(32, 32, 3, 2, 615, 32, 641, 60),
    tile_info(32, 32, 3, 3, 641, 32, 668, 59),
    tile_info(32, 32, 2, 5, 668, 32, 696, 56),
    tile_info(32, 32, 5, 4, 696, 32, 721, 57),
    tile_info(32, 32, 6, 2, 721, 32, 743, 60),
    tile_info(32, 32, 5, 4, 743, 32, 768, 58),
    tile_info(32, 32, 4, 3, 768, 32, 794, 59),
    tile_info(32, 32, 6, 2, 794, 32, 815, 60),
    tile_info(32, 32, 3, 2, 815, 32, 842, 60),
    tile_info(32, 32, 4, 2, 842, 32, 868, 60),
    tile_info(32, 32, 4, 3, 868, 32, 894, 59),
    tile_info(32, 32, 2, 3, 894, 32, 920, 58),
    tile_info(32, 32, 3, 3, 920, 32, 946, 59),
    tile_info(32, 32, 3, 2, 946, 32, 973, 59),
    tile_info(32, 32, 6, 4, 973, 32, 996, 54),
    tile_info(32, 32, 6, 2, 996, 32, 1018, 60),
    tile_info(32, 32, 2, 3, 0, 64, 28, 90),
    tile_info(32, 32, 4, 2, 28, 64, 52, 92),
    tile_info(32, 32, 8, 4, 52, 64, 72, 88),
    tile_info(32, 32, 7, 3, 72, 64, 90, 91),
    tile_info(32, 32, 2, 3, 90, 64, 116, 89),
    tile_info(32, 32, 6, 3, 116, 64, 136, 91),
    tile_info(32, 32, 6, 5, 136, 64, 156, 89),
    tile_info(32, 32, 8, 3, 156, 64, 175, 91),
    tile_info(32, 32, 3, 2, 175, 64, 199, 89),
    tile_info(32, 32, 5, 3, 199, 64, 223, 91),
    tile_info(32, 32, 3, 2, 223, 64, 250, 92),
    tile_info(32, 32, 2, 3, 250, 64, 277, 90),
    tile_info(32, 32, 2, 2, 277, 64, 304, 92),
    tile_info(32, 32, 2, 2, 304, 64, 332, 92),
    tile_info(32, 32, 7, 2, 332, 64, 350, 92),
    tile_info(32, 32, 5, 3, 350, 64, 372, 89),
    tile_info(32, 32, 7, 6, 372, 64, 391, 84),
    tile_info(32, 32, 3, 3, 391, 64, 416, 90),
    tile_info(32, 32, 5, 2, 416, 64, 440, 92),
    tile_info(32, 32, 2, 5, 440, 64, 468, 89),
    tile_info(32, 32, 2, 2, 468, 64, 496, 92),
    tile_info(32, 32, 2, 3, 496, 64, 524, 91),
    tile_info(32, 32, 3, 10, 524, 64, 551, 82),
    tile_info(32, 32, 2, 2, 551, 64, 579, 92),
    tile_info(32, 32, 2, 3, 579, 64, 606, 91),
    tile_info(32, 32, 5, 0, 606, 64, 630, 94),
    tile_info(32, 32, 0, 0, 630, 64, 662, 96),
    tile_info(32, 32, 2, 2, 662, 64, 690, 91),
    tile_info(32, 32, 2, 2, 690, 64, 718, 92),
    tile_info(32, 32, 4, 3, 718, 64, 743, 91),
    tile_info(32, 32, 2, 1, 743, 64, 772, 93),
    tile_info(32, 32, 2, 2, 772, 64, 800, 92),
    tile_info(32, 32, 2, 4, 800, 64, 828, 90),
    tile_info(32, 32, 1, 2, 828, 64, 857, 91),
    tile_info(32, 32, 2, 2, 857, 64, 885, 91),
    tile_info(32, 32, 3, 2, 885, 64, 912, 92),
    tile_info(32, 32, 1, 1, 912, 64, 942, 95),
    tile_info(32, 32, 2, 2, 942, 64, 970, 92),
    tile_info(32, 32, 2, 2, 970, 64, 997, 91),
    tile_info(32, 32, 6, 2, 997, 64, 1017, 92),
    tile_info(32, 32, 6, 2, 0, 96, 23, 124),
    tile_info(32, 32, 4, 3, 23, 96, 47, 123),
    tile_info(32, 32, 5, 4, 47, 96, 67, 122),
    tile_info(32, 32, 3, 7, 67, 96, 93, 116),
    tile_info(32, 32, 2, 4, 93, 96, 119, 122),
    tile_info(32, 32, 3, 3, 119, 96, 144, 121),
    tile_info(32, 32, 5, 3, 144, 96, 168, 123),
    tile_info(32, 32, 3, 5, 168, 96, 195, 121),
    tile_info(32, 32, 2, 3, 195, 96, 223, 123),
    tile_info(32, 32, 7, 6, 223, 96, 242, 117),
    tile_info(32, 32, 4, 4, 242, 96, 266, 120),
    tile_info(32, 32, 0, 0, 266, 96, 298, 126),
    tile_info(32, 32, 7, 2, 298, 96, 316, 124),
    tile_info(32, 32, 2, 2, 316, 96, 345, 124),
    tile_info(32, 32, 2, 5, 345, 96, 374, 121),
    tile_info(32, 32, 2, 3, 374, 96, 403, 123),
    tile_info(32, 32, 2, 2, 403, 96, 432, 124),
    tile_info(32, 32, 1, 1, 432, 96, 462, 125),
    tile_info(32, 32, 5, 5, 462, 96, 483, 121),
    tile_info(32, 32, 3, 2, 483, 96, 507, 124),
    tile_info(32, 32, 2, 3, 507, 96, 536, 123),
    tile_info(32, 32, 2, 0, 536, 96, 565, 126),
    tile_info(32, 32, 2, 1, 565, 96, 594, 125),
    tile_info(32, 32, 2, 2, 594, 96, 623, 124),
    tile_info(32, 32, 2, 2, 623, 96, 652, 124),
    tile_info(32, 32, 2, 2, 652, 96, 681, 124),
    tile_info(32, 32, 2, 2, 681, 96, 710, 124),
    tile_info(32, 32, 2, 4, 710, 96, 739, 122),
    tile_info(32, 32, 2, 6, 739, 96, 768, 120),
    tile_info(32, 32, 2, 2, 768, 96, 797, 124),
    tile_info(32, 32, 2, 3, 797, 96, 826, 123),
    tile_info(32, 32, 2, 4, 826, 96, 855, 122),
    tile_info(32, 32, 2, 3, 855, 96, 882, 123),
    tile_info(32, 32, 4, 5, 882, 96, 905, 119),
    tile_info(32, 32, 2, 3, 905, 96, 933, 122),
    tile_info(32, 32, 2, 3, 933, 96, 961, 123),
    tile_info(32, 32, 3, 2, 961, 96, 988, 124),
    tile_info(32, 32, 2, 3, 988, 96, 1016, 123),
    tile_info(32, 32, 3, 3, 0, 126, 27, 153),
    tile_info(32, 32, 5, 6, 27, 126, 49, 152),
    tile_info(32, 32, 0, 2, 49, 126, 79, 155),
    tile_info(32, 32, 3, 2, 79, 126, 106, 154),
    tile_info(32, 32, 2, 2, 106, 126, 134, 154),
    tile_info(32, 32, 1, 0, 134, 126, 162, 156),
    tile_info(32, 32, 2, 2, 162, 126, 190, 154),
    tile_info(32, 32, 2, 2, 190, 126, 215, 154),
    tile_info(32, 32, 2, 2, 215, 126, 243, 155),
    tile_info(32, 32, 7, 2, 243, 126, 260, 153),
    tile_info(32, 32, 0, 1, 260, 126, 292, 156),
    tile_info(32, 32, 0, 0, 292, 126, 324, 158),
    tile_info(32, 32, 0, 0, 324, 126, 356, 158),
    tile_info(32, 32, 0, 0, 356, 126, 388, 158),
    tile_info(32, 32, 0, 0, 388, 126, 420, 158),
    tile_info(32, 32, 0, 0, 420, 126, 452, 158),
    tile_info(32, 32, 0, 0, 452, 126, 484, 158),
    tile_info(32, 32, 0, 0, 484, 126, 516, 158),
    tile_info(32, 32, 0, 0, 516, 126, 548, 158),
    tile_info(32, 32, 0, 0, 548, 126, 580, 158),
    tile_info(32, 32, 0, 0, 580, 126, 612, 158),
    tile_info(32, 32, 0, 0, 612, 126, 644, 158),
    tile_info(32, 32, 0, 0, 644, 126, 676, 158),
    tile_info(32, 32, 0, 0, 676, 126, 708, 158),
    tile_info(32, 32, 0, 0, 708, 126, 740, 158),
    tile_info(32, 32, 0, 0, 740, 126, 772, 158),
    tile_info(32, 32, 0, 0, 772, 126, 804, 158),
    tile_info(32, 32, 0, 0, 804, 126, 836, 158),
    tile_info(32, 32, 0, 0, 836, 126, 868, 158),
    tile_info(32, 32, 0, 0, 868, 126, 900, 158),
    tile_info(32, 32, 0, 0, 900, 126, 932, 158),
    tile_info(32, 32, 0, 0, 932, 126, 964, 158),
    tile_info(32, 32, 0, 0, 964, 126, 996, 158),
    tile_info(32, 32, 0, 0, 0, 158, 32, 190),
    tile_info(32, 32, 0, 0, 32, 158, 64, 190),
    tile_info(32, 32, 0, 0, 64, 158, 96, 190),
    tile_info(32, 32, 0, 0, 96, 158, 128, 190),
    tile_info(32, 32, 0, 0, 128, 158, 160, 190),
    tile_info(32, 32, 0, 0, 160, 158, 192, 190),
    tile_info(32, 32, 0, 0, 192, 158, 224, 190),
    tile_info(32, 32, 0, 0, 224, 158, 256, 190),
    tile_info(32, 32, 0, 0, 256, 158, 288, 190),
    tile_info(32, 32, 0, 0, 288, 158, 320, 190),
    tile_info(32, 32, 0, 0, 320, 158, 352, 190),
    tile_info(32, 32, 0, 0, 352, 158, 384, 190),
    tile_info(32, 32, 0, 0, 384, 158, 416, 190),
    tile_info(32, 32, 0, 0, 416, 158, 448, 190),
    tile_info(32, 32, 0, 0, 448, 158, 480, 190),
    tile_info(32, 32, 0, 0, 480, 158, 512, 190),
    tile_info(32, 32, 0, 0, 512, 158, 544, 190),
    tile_info(32, 32, 0, 0, 544, 158, 576, 190),
    tile_info(32, 32, 0, 0, 576, 158, 608, 190),
    tile_info(32, 32, 0, 0, 608, 158, 640, 190),
    tile_info(32, 32, 0, 0, 640, 158, 672, 190),
    tile_info(32, 32, 0, 0, 672, 158, 704, 190),
    tile_info(32, 32, 0, 0, 704, 158, 736, 190),
    tile_info(32, 32, 0, 0, 736, 158, 768, 190),
    tile_info(32, 32, 0, 0, 768, 158, 800, 190),
    tile_info(32, 32, 0, 0, 800, 158, 832, 190),
    tile_info(32, 32, 0, 0, 832, 158, 864, 190),
    tile_info(32, 32, 0, 0, 864, 158, 896, 190),
    tile_info(32, 32, 0, 0, 896, 158, 928, 190),
    tile_info(32, 32, 0, 0, 928, 158, 960, 190),
    tile_info(32, 32, 0, 0, 960, 158, 992, 190),
    tile_info(32, 32, 0, 0, 992, 158, 1024, 190),
    tile_info(32, 32, 0, 0, 0, 190, 32, 222),
    tile_info(32, 32, 0, 0, 32, 190, 64, 222),
    tile_info(32, 32, 0, 0, 64, 190, 96, 222),
    tile_info(32, 32, 0, 0, 96, 190, 128, 222),
    tile_info(32, 32, 0, 0, 128, 190, 160, 222),
    tile_info(32, 32, 0, 0, 160, 190, 192, 222),
    tile_info(32, 32, 0, 0, 192, 190, 224, 222),
    tile_info(32, 32, 0, 0, 224, 190, 256, 222),
    tile_info(32, 32, 0, 0, 256, 190, 288, 222),
    tile_info(32, 32, 0, 0, 288, 190, 320, 222),
    tile_info(32, 32, 0, 0, 320, 190, 352, 222),
    tile_info(32, 32, 0, 0, 352, 190, 384, 222),
    tile_info(32, 32, 2, 1, 384, 190, 414, 220),
    tile_info(32, 32, 4, 3, 414, 190, 442, 219),
    tile_info(32, 32, 5, 3, 442, 190, 463, 217),
    tile_info(32, 32, 0, 3, 463, 190, 494, 217),
    tile_info(32, 32, 3, 2, 494, 190, 522, 219),
    tile_info(32, 32, 4, 3, 522, 190, 548, 218),
    tile_info(32, 32, 1, 8, 548, 190, 578, 212),
    tile_info(32, 32, 3, 2, 578, 190, 605, 218),
    tile_info(32, 32, 0, 0, 605, 190, 636, 221),
    tile_info(32, 32, 1, 2, 636, 190, 666, 217),
    tile_info(32, 32, 4, 3, 666, 190, 690, 217),
    tile_info(32, 32, 2, 4, 690, 190, 718, 217),
    tile_info(32, 32, 4, 0, 718, 190, 744, 221),
    tile_info(32, 32, 0, 1, 744, 190, 776, 221),
    tile_info(32, 32, 2, 2, 776, 190, 803, 218),
    tile_info(32, 32, 0, 0, 803, 190, 835, 222),
    tile_info(32, 32, 0, 0, 835, 190, 867, 222),
    tile_info(32, 32, 0, 0, 867, 190, 899, 222),
    tile_info(32, 32, 3, 3, 899, 190, 925, 216),
    tile_info(32, 32, 1, 2, 925, 190, 955, 218),
    tile_info(32, 32, 4, 1, 955, 190, 981, 216),
    tile_info(32, 32, 4, 4, 981, 190, 1006, 216),
    tile_info(32, 32, 6, 6, 0, 222, 21, 245),
    tile_info(20, 20, 0, 0, 21, 222, 41, 242),
    tile_info(20, 20, 0, 0, 41, 222, 61, 242),
    tile_info(20, 20, 0, 0, 61, 222, 81, 242),
    tile_info(20, 20, 0, 0, 81, 222, 101, 242),
    tile_info(20, 20, 0, 0, 101, 222, 121, 242),
    tile_info(20, 20, 0, 0, 121, 222, 141, 242),
    tile_info(20, 20, 0, 0, 141, 222, 161, 242),
    tile_info(20, 20, 0, 0, 161, 222, 181, 242),
    tile_info(20, 20, 0, 0, 181, 222, 201, 242),
    tile_info(20, 20, 0, 0, 201, 222, 221, 242),
    tile_info(20, 20, 0, 0, 221, 222, 241, 242),
    tile_info(20, 20, 0, 0, 241, 222, 261, 242),
    tile_info(20, 20, 0, 0, 261, 222, 281, 242),
    tile_info(20, 20, 0, 0, 281, 222, 301, 242),
    tile_info(20, 20, 0, 0, 301, 222, 321, 242),
    tile_info(20, 20, 0, 0, 321, 222, 341, 242),
    tile_info(20, 20, 0, 0, 341, 222, 361, 242),
    tile_info(20, 20, 0, 0, 361, 222, 381, 242),
};

tile_info &tile_gui_info(tileidx_t idx)
{
    assert(idx >= 0 && idx < TILEG_GUI_MAX);
    return _tile_gui_info[idx - 0];
}


typedef std::pair<const char*, tileidx_t> _name_pair;

_name_pair gui_name_pairs[] =
{
    _name_pair("abjuration", 132 + 0),
    _name_pair("agony", 96 + 0),
    _name_pair("air_magic_off", 225 + 0),
    _name_pair("air_magic_on", 224 + 0),
    _name_pair("airstrike", 22 + 0),
    _name_pair("alistairs_intoxication", 119 + 0),
    _name_pair("alter_self", 163 + 0),
    _name_pair("animate_dead", 97 + 0),
    _name_pair("animate_skeleton", 98 + 0),
    _name_pair("apportation", 151 + 0),
    _name_pair("armour_off", 191 + 0),
    _name_pair("armour_on", 190 + 0),
    _name_pair("axes_off", 175 + 0),
    _name_pair("axes_on", 174 + 0),
    _name_pair("banishment", 152 + 0),
    _name_pair("berserker_rage", 51 + 0),
    _name_pair("blade_hands", 164 + 0),
    _name_pair("blink", 153 + 0),
    _name_pair("bolt_of_cold", 83 + 0),
    _name_pair("bolt_of_draining", 99 + 0),
    _name_pair("bolt_of_fire", 68 + 0),
    _name_pair("bolt_of_magma", 69 + 0),
    _name_pair("borgnjors_revivification", 100 + 0),
    _name_pair("bows_off", 185 + 0),
    _name_pair("bows_on", 184 + 0),
    _name_pair("call_canine_familiar", 133 + 0),
    _name_pair("call_imp", 134 + 0),
    _name_pair("cause_fear", 52 + 0),
    _name_pair("chain_lightning", 23 + 0),
    _name_pair("charms_off", 211 + 0),
    _name_pair("charms_on", 210 + 0),
    _name_pair("cigotuvis_degeneration", 101 + 0),
    _name_pair("cmd_autofight", 237 + 0),
    _name_pair("cmd_character_dump", 253 + 0),
    _name_pair("cmd_cycle_quiver_forward", 254 + 0),
    _name_pair("cmd_display_character_status", 248 + 0),
    _name_pair("cmd_display_commands", 252 + 0),
    _name_pair("cmd_display_mutations", 246 + 0),
    _name_pair("cmd_display_overmap", 244 + 0),
    _name_pair("cmd_display_religion", 245 + 0),
    _name_pair("cmd_display_skills", 247 + 0),
    _name_pair("cmd_edit_player_tile", 251 + 0),
    _name_pair("cmd_explore", 235 + 0),
    _name_pair("cmd_interlevel_travel", 236 + 0),
    _name_pair("cmd_known_items", 249 + 0),
    _name_pair("cmd_pray", 240 + 0),
    _name_pair("cmd_replay_messages", 242 + 0),
    _name_pair("cmd_resists_screen", 243 + 0),
    _name_pair("cmd_rest", 234 + 0),
    _name_pair("cmd_save_game_now", 250 + 0),
    _name_pair("cmd_search", 238 + 0),
    _name_pair("cmd_search_stashes", 241 + 0),
    _name_pair("cmd_use_ability", 239 + 0),
    _name_pair("condensation_shield", 84 + 0),
    _name_pair("confuse", 53 + 0),
    _name_pair("confusing_touch", 54 + 0),
    _name_pair("conjurations_off", 207 + 0),
    _name_pair("conjurations_on", 206 + 0),
    _name_pair("conjure_ball_lightning", 24 + 0),
    _name_pair("conjure_flame", 70 + 0),
    _name_pair("control_undead", 102 + 0),
    _name_pair("controlled_blink", 154 + 0),
    _name_pair("controlled_teleport", 155 + 0),
    _name_pair("corona", 55 + 0),
    _name_pair("corpse_rot", 103 + 0),
    _name_pair("crossbows_off", 187 + 0),
    _name_pair("crossbows_on", 186 + 0),
    _name_pair("cure_poison", 120 + 0),
    _name_pair("darkness", 56 + 0),
    _name_pair("death_channel", 104 + 0),
    _name_pair("deaths_door", 105 + 0),
    _name_pair("deflect_missiles", 25 + 0),
    _name_pair("delayed_fireball", 71 + 0),
    _name_pair("demonic_horde", 135 + 0),
    _name_pair("dig", 40 + 0),
    _name_pair("dispel_undead", 106 + 0),
    _name_pair("dispersal", 156 + 0),
    _name_pair("dodging_off", 193 + 0),
    _name_pair("dodging_on", 192 + 0),
    _name_pair("dragon_form", 72 + 0),
    _name_pair("earth_magic_off", 227 + 0),
    _name_pair("earth_magic_on", 226 + 0),
    _name_pair("enslavement", 57 + 0),
    _name_pair("ensorcelled_hibernation", 85 + 0),
    _name_pair("error", 256 + 0),
    _name_pair("evaporate", 73 + 0),
    _name_pair("evocations_off", 233 + 0),
    _name_pair("evocations_on", 232 + 0),
    _name_pair("excruciating_wounds", 107 + 0),
    _name_pair("extension", 58 + 0),
    _name_pair("fighting_off", 169 + 0),
    _name_pair("fighting_on", 168 + 0),
    _name_pair("fire_brand", 74 + 0),
    _name_pair("fire_magic_off", 221 + 0),
    _name_pair("fire_magic_on", 220 + 0),
    _name_pair("fire_storm", 75 + 0),
    _name_pair("fireball", 76 + 0),
    _name_pair("flame_tongue", 77 + 0),
    _name_pair("flight", 26 + 0),
    _name_pair("freeze", 86 + 0),
    _name_pair("freezing_aura", 87 + 0),
    _name_pair("freezing_cloud", 88 + 0),
    _name_pair("fulsome_distillation", 108 + 0),
    _name_pair("haste", 59 + 0),
    _name_pair("haunt", 109 + 0),
    _name_pair("hexes_off", 209 + 0),
    _name_pair("hexes_on", 208 + 0),
    _name_pair("homunculus", 166 + 0),
    _name_pair("ice_form", 90 + 0),
    _name_pair("ice_magic_off", 223 + 0),
    _name_pair("ice_magic_on", 222 + 0),
    _name_pair("ice_storm", 91 + 0),
    _name_pair("ignite_poison", 78 + 0),
    _name_pair("illude", 130 + 0),
    _name_pair("inner_flame", 82 + 0),
    _name_pair("insulation", 27 + 0),
    _name_pair("invisibility", 60 + 0),
    _name_pair("invocations_off", 231 + 0),
    _name_pair("invocations_on", 230 + 0),
    _name_pair("iood", 38 + 0),
    _name_pair("iron_shot", 39 + 0),
    _name_pair("iskenderuns_mystic_blast", 36 + 0),
    _name_pair("ledas_liquefaction", 43 + 0),
    _name_pair("lees_rapid_deconstruction", 41 + 0),
    _name_pair("lehudibs_crystal_spear", 42 + 0),
    _name_pair("lethal_infusion", 110 + 0),
    _name_pair("levitation", 28 + 0),
    _name_pair("lightning_bolt", 29 + 0),
    _name_pair("long_blades_off", 173 + 0),
    _name_pair("long_blades_on", 172 + 0),
    _name_pair("maces_flails_off", 177 + 0),
    _name_pair("maces_flails_on", 176 + 0),
    _name_pair("magic_dart", 37 + 0),
    _name_pair("malign_gateway", 136 + 0),
    _name_pair("mass_confusion", 61 + 0),
    _name_pair("memorise", 167 + 0),
    _name_pair("mephitic_cloud", 121 + 0),
    _name_pair("metabolic_englaciation", 92 + 0),
    _name_pair("necromancy_off", 215 + 0),
    _name_pair("necromancy_on", 214 + 0),
    _name_pair("necromutation", 111 + 0),
    _name_pair("olgrebs_toxic_radiance", 122 + 0),
    _name_pair("ozocubus_armour", 93 + 0),
    _name_pair("ozocubus_refrigeration", 94 + 0),
    _name_pair("pain", 112 + 0),
    _name_pair("passage_of_golubria", 157 + 0),
    _name_pair("passwall", 44 + 0),
    _name_pair("petrify", 45 + 0),
    _name_pair("phase_shift", 158 + 0),
    _name_pair("poison_arrow", 123 + 0),
    _name_pair("poison_brand", 124 + 0),
    _name_pair("poison_magic_off", 229 + 0),
    _name_pair("poison_magic_on", 228 + 0),
    _name_pair("poisonous_cloud", 125 + 0),
    _name_pair("polearms_off", 179 + 0),
    _name_pair("polearms_on", 178 + 0),
    _name_pair("polymorph_other", 165 + 0),
    _name_pair("portal_projectile", 159 + 0),
    _name_pair("projected_noise", 62 + 0),
    _name_pair("recall", 137 + 0),
    _name_pair("regeneration", 113 + 0),
    _name_pair("repel_missiles", 30 + 0),
    _name_pair("resist_poison", 126 + 0),
    _name_pair("ring_of_flames", 79 + 0),
    _name_pair("sandblast", 46 + 0),
    _name_pair("see_invisible", 63 + 0),
    _name_pair("selective_amnesia", 67 + 0),
    _name_pair("shatter", 47 + 0),
    _name_pair("shields_off", 199 + 0),
    _name_pair("shields_on", 198 + 0),
    _name_pair("shock", 31 + 0),
    _name_pair("short_blades_off", 171 + 0),
    _name_pair("short_blades_on", 170 + 0),
    _name_pair("silence", 32 + 0),
    _name_pair("simulacrum", 114 + 0),
    _name_pair("slings_off", 183 + 0),
    _name_pair("slings_on", 182 + 0),
    _name_pair("slow", 64 + 0),
    _name_pair("spell_air", 10 + 0),
    _name_pair("spell_conjuration", 11 + 0),
    _name_pair("spell_divination", 12 + 0),
    _name_pair("spell_earth", 13 + 0),
    _name_pair("spell_enchantment", 14 + 0),
    _name_pair("spell_fire", 15 + 0),
    _name_pair("spell_ice", 16 + 0),
    _name_pair("spell_necromancy", 17 + 0),
    _name_pair("spell_poison", 18 + 0),
    _name_pair("spell_summoning", 19 + 0),
    _name_pair("spell_translocation", 20 + 0),
    _name_pair("spell_transmutation", 21 + 0),
    _name_pair("spellcasting_off", 205 + 0),
    _name_pair("spellcasting_on", 204 + 0),
    _name_pair("spider_form", 127 + 0),
    _name_pair("stabbing_off", 197 + 0),
    _name_pair("stabbing_on", 196 + 0),
    _name_pair("startup_arena", 8 + 0),
    _name_pair("startup_challenge", 3 + 0),
    _name_pair("startup_hints", 4 + 0),
    _name_pair("startup_instructions", 7 + 0),
    _name_pair("startup_sprint", 5 + 0),
    _name_pair("startup_stonesoup", 1 + 0),
    _name_pair("startup_tutorial", 2 + 0),
    _name_pair("startup_zotdef", 6 + 0),
    _name_pair("static_discharge", 33 + 0),
    _name_pair("statue_form", 48 + 0),
    _name_pair("staves_off", 181 + 0),
    _name_pair("staves_on", 180 + 0),
    _name_pair("stealth_off", 195 + 0),
    _name_pair("stealth_on", 194 + 0),
    _name_pair("sticks_to_snakes", 138 + 0),
    _name_pair("sticky_flame", 80 + 0),
    _name_pair("sting", 128 + 0),
    _name_pair("stone_arrow", 49 + 0),
    _name_pair("stoneskin", 50 + 0),
    _name_pair("sublimation_of_blood", 115 + 0),
    _name_pair("summon_butterflies", 139 + 0),
    _name_pair("summon_demon", 140 + 0),
    _name_pair("summon_dragon", 141 + 0),
    _name_pair("summon_elemental", 142 + 0),
    _name_pair("summon_greater_demon", 143 + 0),
    _name_pair("summon_horrible_things", 144 + 0),
    _name_pair("summon_hydra", 145 + 0),
    _name_pair("summon_ice_beast", 146 + 0),
    _name_pair("summon_scorpions", 147 + 0),
    _name_pair("summon_shadow_creatures", 148 + 0),
    _name_pair("summon_small_mammals", 149 + 0),
    _name_pair("summon_ugly_thing", 150 + 0),
    _name_pair("summonings_off", 213 + 0),
    _name_pair("summonings_on", 212 + 0),
    _name_pair("sure_blade", 65 + 0),
    _name_pair("swiftness", 34 + 0),
    _name_pair("symbol_of_torment", 116 + 0),
    _name_pair("tab_command", 272 + 0),
    _name_pair("tab_item", 257 + 0),
    _name_pair("tab_memorise", 263 + 0),
    _name_pair("tab_monster", 266 + 0),
    _name_pair("tab_skill", 269 + 0),
    _name_pair("tab_spell", 260 + 0),
    _name_pair("tavern", 9 + 0),
    _name_pair("teleport", 160 + 0),
    _name_pair("teleport_other", 161 + 0),
    _name_pair("throw_flame", 81 + 0),
    _name_pair("throw_frost", 95 + 0),
    _name_pair("throw_icicle", 89 + 0),
    _name_pair("throwing_off", 189 + 0),
    _name_pair("throwing_on", 188 + 0),
    _name_pair("todo", 255 + 0),
    _name_pair("tornado", 35 + 0),
    _name_pair("toxic_shield", 131 + 0),
    _name_pair("translocations_off", 217 + 0),
    _name_pair("translocations_on", 216 + 0),
    _name_pair("transmutations_off", 219 + 0),
    _name_pair("transmutations_on", 218 + 0),
    _name_pair("traps_doors_off", 201 + 0),
    _name_pair("traps_doors_on", 200 + 0),
    _name_pair("tukimas_dance", 66 + 0),
    _name_pair("twisted_resurrection", 117 + 0),
    _name_pair("unarmed_combat_off", 203 + 0),
    _name_pair("unarmed_combat_on", 202 + 0),
    _name_pair("vampiric_draining", 118 + 0),
    _name_pair("venom_bolt", 129 + 0),
    _name_pair("warp_weapon", 162 + 0),
};

bool tile_gui_index(const char *str, tileidx_t *idx)
{
    assert(str);
    if (!str)
        return false;

    string lc = str;
    for (unsigned int i = 0; i < lc.size(); i++)
        lc[i] = tolower(lc[i]);

    int num_pairs = sizeof(gui_name_pairs) / sizeof(gui_name_pairs[0]);
    bool result = binary_search<const char *, tileidx_t>(
       lc.c_str(), &gui_name_pairs[0], num_pairs, &strcmp, idx);
    return (result);
}

bool tile_gui_equal(tileidx_t tile, tileidx_t idx)
{
    assert(tile >= 0 && tile < TILEG_GUI_MAX);
    return (idx >= tile && idx < tile + tile_gui_count(tile));
}


typedef std::pair<tile_variation, tileidx_t> _colour_pair;

_colour_pair gui_colour_pairs[] =
{
    _colour_pair(tile_variation(0, 0), 0),
};

tileidx_t tile_gui_coloured(tileidx_t idx, int col)
{
    int num_pairs = sizeof(gui_colour_pairs) / sizeof(gui_colour_pairs[0]);
    tile_variation key(idx, col);
    tileidx_t found;
    bool result = binary_search<tile_variation, tileidx_t>(
       key, &gui_colour_pairs[0], num_pairs,
       &tile_variation::cmp, &found);
    return (result ? found : idx);
}

