/**
 * @file
 * @brief Code related to travel exclusions.
**/

#include "AppHdr.h"

#include "exclude.h"

#include <algorithm>
#include <sstream>

#include "cloud.h"
#include "coord.h"
#include "coordit.h"
#include "dgn-overview.h"
#include "libutil.h"
#include "map_knowledge.h"
#include "mon-util.h"
#include "options.h"
#include "env.h"
#include "tags.h"
#include "terrain.h"
#include "travel.h"
#include "hints.h"
#include "view.h"

static bool _mon_needs_auto_exclude(const monster* mon, bool sleepy = false)
{
    if (mons_is_stationary(mon))
    {
        if (sleepy)
            return (false);

        // I cannot conceive a reason why these would ever be excluded. -dtsund
        if (mon->type == MONS_SCREAMING_STATUE)
            return false;

        // Don't give away mimics unless already known.
        return (!mons_is_mimic(mon->type)
                || testbits(mon->flags, MF_KNOWN_MIMIC));
    }
    // Auto exclusion only makes sense if the monster is still asleep.
    return (mon->asleep());
}

// Check whether a given monster is listed in the auto_exclude option.
bool need_auto_exclude(const monster* mon, bool sleepy)
{
    // This only works if the name is lowercased.
    std::string name = mon->name(DESC_BASENAME,
                                 mons_is_stationary(mon)
                                     && testbits(mon->flags, MF_SEEN));
    lowercase(name);

    for (unsigned i = 0; i < Options.auto_exclude.size(); ++i)
        if (Options.auto_exclude[i].matches(name)
            && _mon_needs_auto_exclude(mon, sleepy)
            && (mon->attitude == ATT_HOSTILE
                || mon->type == MONS_HYPERACTIVE_BALLISTOMYCETE))
        {
            return (true);
        }

    return (false);
}

// Nightstalker reduces LOS, so reducing the maximum exclusion radius
// only makes sense. This is only possible because it's a permanent
// mutation; the lantern of Shadows should not have this effect.
int _get_full_exclusion_radius()
{
    return (LOS_RADIUS - player_mutation_level(MUT_NIGHTSTALKER));
}

// If the monster is in the auto_exclude list, automatically set an
// exclusion.
void set_auto_exclude(const monster* mon)
{
    if (!player_in_mappable_area())
        return;

    if (need_auto_exclude(mon) && !is_exclude_root(mon->pos()))
    {
        int rad = _get_full_exclusion_radius();
        if (mon->type == MONS_HYPERACTIVE_BALLISTOMYCETE)
            rad = 2;
        set_exclude(mon->pos(), rad, true);
        // FIXME: If this happens for several monsters in the same turn
        //        (as is possible for some vaults), this could be really
        //        annoying. (jpeg)
        mprf(MSGCH_WARN,
             "Marking area around %s as unsafe for travelling.",
             mon->name(DESC_NOCAP_THE).c_str());

#ifdef USE_TILE
        viewwindow();
#endif
        learned_something_new(HINT_AUTO_EXCLUSION, mon->pos());
    }
}

// Clear auto exclusion if the monster is killed or wakes up with the
// player in sight. If sleepy is true, stationary monsters are ignored.
void remove_auto_exclude(const monster* mon, bool sleepy)
{
    if (need_auto_exclude(mon, sleepy))
    {
        del_exclude(mon->pos());
#ifdef USE_TILE
        viewwindow();
#endif
    }
}

static opacity_type _feat_opacity(dungeon_feature_type feat)
{
    return (feat_is_opaque(feat) ? OPC_OPAQUE : OPC_CLEAR);
}

// A cell is considered clear unless the player knows it's
// opaque.
class opacity_excl : public opacity_func
{
public:
    CLONE(opacity_excl)

    opacity_type operator()(const coord_def& p) const
    {
        map_cell& cell = env.map_knowledge(p);
        if (!cell.seen())
            return OPC_CLEAR;
        else if (!cell.changed())
            return _feat_opacity(env.grid(p));
        else if (cell.feat() != DNGN_UNSEEN)
            return _feat_opacity(cell.feat());
        else
            return OPC_CLEAR;
    }
};
static opacity_excl opc_excl;

// Note: circle_def(r, C_ROUND) gives a circle with square radius r*r+1;
// this doesn't work well for radius 0, but then we want to
// skip LOS calculation in that case anyway since it doesn't
// currently short-cut for small bounds. So radius 0, 1 are special-cased.

// C_ROUND being bad with radius 0 isn't relevant any more. -- elliptic
travel_exclude::travel_exclude(const coord_def &p, int r,
                               bool autoexcl, std::string dsc, bool vaultexcl)
    : pos(p), radius(r),
      los(los_def(p, opc_excl, circle_def(r, C_SQUARE))),
      uptodate(false), autoex(autoexcl), desc(dsc), vault(vaultexcl)
{
    set_los();
}

// For exclude_map[p] = foo;
travel_exclude::travel_exclude()
    : pos(-1, -1), radius(-1),
      los(coord_def(-1, -1), opc_excl, circle_def(-1, C_SQUARE)),
      uptodate(false), autoex(false), desc(""), vault(false)
{
}

extern exclude_set curr_excludes; // in travel.cc

void travel_exclude::set_los()
{
    uptodate = true;
    if (radius > 1)
    {
        // Radius might have been changed, and this is cheap.
        los.set_bounds(circle_def(radius, C_SQUARE));
        los.update();
    }
}

bool travel_exclude::affects(const coord_def& p) const
{
    if (!uptodate)
    {
        mprf(MSGCH_ERROR, "exclusion not up-to-date: e (%d,%d) p (%d,%d)",
             pos.x, pos.y, p.x, p.y);
    }
    if (radius == 0)
        return (p == pos);
    else if (radius == 1)
        return ((p - pos).rdist() <= 1);
    else
        return (los.see_cell(p));
}

bool travel_exclude::in_bounds(const coord_def &p) const
{
    return (radius == 0 && p == pos
            || radius == 1 && (p - pos).rdist() <= 1
            || los.in_bounds(p));
}

/////////////////////////////////////////////////////////////////////////

exclude_set::exclude_set()
{
}

void exclude_set::clear()
{
    exclude_roots.clear();
    exclude_points.clear();
}

void exclude_set::erase(const coord_def &p)
{
    exclude_set::iterator it = exclude_roots.find(p);

    if (it == exclude_roots.end())
        return;

    travel_exclude old_ex = it->second;
    exclude_roots.erase(it);

    recompute_excluded_points();
}

void exclude_set::add_exclude(travel_exclude &ex)
{
    add_exclude_points(ex);
    exclude_roots[ex.pos] = ex;
}

void exclude_set::add_exclude(const coord_def &p, int radius,
                              bool autoexcl, std::string desc,
                              bool vaultexcl)
{
    travel_exclude ex(p, radius, autoexcl, desc, vaultexcl);
    add_exclude(ex);
}

void exclude_set::add_exclude_points(travel_exclude& ex)
{
    if (ex.radius == 0)
    {
        exclude_points.insert(ex.pos);
        return;
    }

    if (!ex.uptodate)
        ex.set_los();
    else
        ex.los.update();

    for (radius_iterator ri(ex.pos, ex.radius, C_SQUARE); ri; ++ri)
        if (ex.affects(*ri))
            exclude_points.insert(*ri);
}

void exclude_set::update_excluded_points(bool recompute_los)
{
    for (iterator it = exclude_roots.begin(); it != exclude_roots.end(); ++it)
    {
        travel_exclude &ex = it->second;
        if (!ex.uptodate)
        {
            recompute_excluded_points(recompute_los);
            return;
        }
    }
}

void exclude_set::recompute_excluded_points(bool recompute_los)
{
    exclude_points.clear();
    for (iterator it = exclude_roots.begin(); it != exclude_roots.end(); ++it)
    {
        travel_exclude &ex = it->second;
        if (recompute_los)
            ex.set_los();
        add_exclude_points(ex);
    }
}

bool exclude_set::is_excluded(const coord_def &p) const
{
    return (exclude_points.find(p) != exclude_points.end());
}

bool exclude_set::is_exclude_root(const coord_def &p) const
{
    return (exclude_roots.find(p) != exclude_roots.end());
}

travel_exclude* exclude_set::get_exclude_root(const coord_def &p)
{
    exclude_set::iterator it = exclude_roots.find(p);

    if (it != exclude_roots.end())
        return (&(it->second));

    return (NULL);
}

size_t exclude_set::size() const
{
    return exclude_roots.size();
}

bool exclude_set::empty() const
{
    return exclude_roots.empty();
}

exclude_set::const_iterator exclude_set::begin() const
{
    return exclude_roots.begin();
}

exclude_set::const_iterator exclude_set::end() const
{
    return exclude_roots.end();
}

exclude_set::iterator exclude_set::begin()
{
    return exclude_roots.begin();
}

exclude_set::iterator exclude_set::end()
{
    return exclude_roots.end();
}

/////////////////////////////////////////////////////////////////////////

static void _mark_excludes_non_updated(const coord_def &p)
{
    for (exclude_set::iterator it = curr_excludes.begin();
         it != curr_excludes.end(); ++it)
    {
        travel_exclude &ex = it->second;
        ex.uptodate = ex.uptodate && !ex.in_bounds(p);
    }
}

void init_exclusion_los()
{
    curr_excludes.recompute_excluded_points(true);
}

/*
 * Update exclusions' LOS to reflect changes within their range.
 * "changed" is a list of coordinates that have been changed.
 * Only exclusions that might have one of the changed points
 * in view are updated.
 */
void update_exclusion_los(std::vector<coord_def> changed)
{
    if (changed.empty())
        return;

    for (unsigned int i = 0; i < changed.size(); ++i)
        _mark_excludes_non_updated(changed[i]);

    curr_excludes.update_excluded_points(true);
}

bool is_excluded(const coord_def &p, const exclude_set &exc)
{
    return exc.is_excluded(p);
}

bool is_exclude_root(const coord_def &p)
{
    return (curr_excludes.get_exclude_root(p));
}

int get_exclusion_radius(const coord_def &p)
{
    if (travel_exclude *exc = curr_excludes.get_exclude_root(p))
    {
        if (!exc->radius)
            return 1;
        else
            return exc->radius;
    }
    return 0;
}

std::string get_exclusion_desc(const coord_def &p)
{
    if (travel_exclude *exc = curr_excludes.get_exclude_root(p))
        return exc->desc;
    return "";
}

#ifdef USE_TILE
// update Gmap for squares surrounding exclude centre
static void _tile_exclude_gmap_update(const coord_def &p)
{
    for (int x = -8; x <= 8; x++)
        for (int y = -8; y <= 8; y++)
        {
            const coord_def pc(p.x + x, p.y + y);
            if (in_bounds(pc))
                tiles.update_minimap(pc);
        }
}
#endif

static void _exclude_update()
{
    set_level_exclusion_annotation(curr_excludes.get_exclusion_desc());
    travel_cache.update_excludes();
}

static void _exclude_update(const coord_def &p)
{
#ifdef USE_TILE
    _tile_exclude_gmap_update(p);
#endif
    _exclude_update();
}

// Catch up exclude updates from set_exclude with defer_updates=true.
//
// Warning: For tiles, this assumes all changed exclude centres
//          are still there, so this won't work as is for
//          del_exclude.
void deferred_exclude_update()
{
    _exclude_update();
#ifdef USE_TILE
    exclude_set::iterator it;
    for (it = curr_excludes.begin(); it != curr_excludes.end(); ++it)
        _tile_exclude_gmap_update(it->second.pos);
#endif
}

void clear_excludes()
{
    // Sanity checks
    if (!player_in_mappable_area())
        return;

#ifdef USE_TILE
    // Tiles needs to update the minimap for each exclusion that is removed,
    // but the exclusions need to be removed first.  Therefore, make a copy
    // of the current set of exclusions and iterate through the copy below.
    exclude_set excludes = curr_excludes;
#endif

    curr_excludes.clear();
    clear_level_exclusion_annotation();

#ifdef USE_TILE
    for (exclude_set::iterator it = excludes.begin(); it != excludes.end(); ++it)
        _tile_exclude_gmap_update(it->second.pos);
#endif

    _exclude_update();
}

static void _exclude_gate(const coord_def &p, bool del = false)
{
    std::set<coord_def> all_doors;
    find_connected_identical(p, grd(p), all_doors);
    for (std::set<coord_def>::const_iterator dc = all_doors.begin();
         dc != all_doors.end(); ++dc)
    {
        if (del)
            del_exclude(*dc);
        else
            set_exclude(*dc, 0);
    }
}

// Cycles the radius of an exclusion, including "off" state;
// may start at 0 < radius < LOS_RADIUS, but won't cycle there.
void cycle_exclude_radius(const coord_def &p)
{
    if (travel_exclude *exc = curr_excludes.get_exclude_root(p))
    {
        if (feat_is_door(grd(p)))
        {
            _exclude_gate(p, exc->radius == 0);
            return;
        }
        if (exc->radius > 0)
            set_exclude(p, 0);
        else
            del_exclude(p);
    }
    else
        set_exclude(p, _get_full_exclusion_radius());
}

// Remove a possible exclude.
void del_exclude(const coord_def &p)
{
    curr_excludes.erase(p);
    _exclude_update(p);
}

// Set or update an exclude.
void set_exclude(const coord_def &p, int radius, bool autoexcl, bool vaultexcl,
                 bool defer_updates)
{
    if (!player_in_mappable_area()) // currently only Abyss
        return;

    if (!in_bounds(p))
        return;

    if (travel_exclude *exc = curr_excludes.get_exclude_root(p))
    {
        if (exc->desc.empty() && defer_updates)
        {
            const int cl = env.cgrid(p);
            if (env.cgrid(p) != EMPTY_CLOUD)
                exc->desc = cloud_name_at_index(cl) + " cloud";
        }
        else if (exc->radius == radius)
            return;

        exc->radius   = radius;
        exc->uptodate = false;
        curr_excludes.recompute_excluded_points();
    }
    else
    {
        std::string desc = "";
        if (!defer_updates)
        {
            // Don't list a monster in the exclusion annotation if the
            // exclusion was triggered by e.g. the flamethrowers' lua check.
            const map_cell& cell = env.map_knowledge(p);
            if (cell.monster() != MONS_NO_MONSTER)
            {
                desc = mons_type_name(cell.monster(), DESC_PLAIN);
                if (cell.detected_monster())
                    desc += " (detected)";
            }
            else
            {
                // Maybe it's a door or staircase?
                const dungeon_feature_type feat = cell.feat();
                if (feat_is_door(feat))
                    desc = "door";
                else
                {
                    const command_type dir = feat_stair_direction(feat);
                    if (dir == CMD_GO_UPSTAIRS)
                        desc = "upstairs";
                    else if (dir == CMD_GO_DOWNSTAIRS)
                        desc = "downstairs";
                }
            }
        }
        else
        {
            const int cl = env.cgrid(p);
            if (env.cgrid(p) != EMPTY_CLOUD)
                desc = cloud_name_at_index(cl) + " cloud";
        }

        curr_excludes.add_exclude(p, radius, autoexcl, desc, vaultexcl);
    }

    if (!defer_updates)
        _exclude_update(p);
}

// If a cell that was placed automatically no longer contains the original
// monster (or it is invisible), remove the exclusion.
void maybe_remove_autoexclusion(const coord_def &p)
{
    if (travel_exclude *exc = curr_excludes.get_exclude_root(p))
    {
        if (!exc->autoex)
            return;

        const monster* m = monster_at(p);
        if (!m || !you.can_see(m)
            || m->attitude != ATT_HOSTILE
                && m->type != MONS_HYPERACTIVE_BALLISTOMYCETE
            || strcmp(mons_type_name(m->type, DESC_PLAIN).c_str(),
                      exc->desc.c_str()) != 0)
        {
            del_exclude(p);
        }
    }
}

// Lists all exclusions on the current level.
std::string exclude_set::get_exclusion_desc()
{
    std::vector<std::string> desc;
    int count_other = 0;
    for (exclmap::iterator it = exclude_roots.begin();
         it != exclude_roots.end(); ++it)
    {
        travel_exclude &ex = it->second;

        // Don't count cloud exclusions.
        if (strstr(ex.desc.c_str(), "cloud"))
            continue;

        if (ex.desc != "")
            desc.push_back(ex.desc);
        else
            count_other++;
    }

    if (desc.size() > 1)
    {
        // Combine identical descriptions.
        std::sort(desc.begin(), desc.end());
        std::vector<std::string> help = desc;
        desc.clear();
        std::string old_desc = "";
        int count = 1;
        for (unsigned int i = 0; i < help.size(); ++i)
        {
            std::string tmp = help[i];
            if (i == 0)
                old_desc = tmp;
            else
            {
                if (strcmp(tmp.c_str(), old_desc.c_str()) == 0)
                    count++;
                else
                {
                    if (count == 1)
                        desc.push_back(old_desc);
                    else
                    {
                        snprintf(info, INFO_SIZE, "%d %s",
                                 count, pluralise(old_desc).c_str());
                        desc.push_back(info);
                        count = 1;
                    }
                    old_desc = tmp;
                }
            }
        }
        if (count == 1)
            desc.push_back(old_desc);
        else
        {
            snprintf(info, INFO_SIZE, "%d %s",
                     count, pluralise(old_desc).c_str());
            desc.push_back(info);
        }
    }

    if (count_other > 0)
    {
        snprintf(info, INFO_SIZE, "%d %sexclusion%s",
                 count_other, desc.empty() ? "" : "more ",
                 count_other > 1 ? "s" : "");
        desc.push_back(info);
    }
    else if (desc.empty())
        return "";

    std::string desc_str = "";
    if (desc.size() > 1 || count_other == 0)
    {
        snprintf(info, INFO_SIZE, "exclusion%s: ",
                 desc.size() > 1 ? "s" : "");
        desc_str += info;
    }
    return (desc_str + comma_separated_line(desc.begin(), desc.end(),
                                            " and ", ", "));
}


void marshallExcludes(writer& outf, const exclude_set& excludes)
{
    marshallShort(outf, excludes.size());
    exclude_set::const_iterator it;
    if (excludes.size())
    {
        for (it = excludes.begin(); it != excludes.end(); ++it)
        {
            const travel_exclude &ex = it->second;
            marshallCoord(outf, ex.pos);
            marshallShort(outf, ex.radius);
            marshallBoolean(outf, ex.autoex);
            marshallString(outf, ex.desc);
            // XXX: marshall travel_exclude::vault?
        }
    }
}

void unmarshallExcludes(reader& inf, int minorVersion, exclude_set &excludes)
{
    excludes.clear();
    int nexcludes = unmarshallShort(inf);
    if (nexcludes)
    {
        for (int i = 0; i < nexcludes; ++i)
        {
            const coord_def c      = unmarshallCoord(inf);
            const int radius       = unmarshallShort(inf);
            const bool autoexcl    = unmarshallBoolean(inf);
            const std::string desc = unmarshallString(inf);
            excludes.add_exclude(c, radius, autoexcl, desc);
        }
    }
}
