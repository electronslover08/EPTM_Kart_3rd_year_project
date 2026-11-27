"""Roues à chevrons (herringbone) pour la direction du kart - script Fusion.

Crée deux corps dans le composant racine du document actif (fonctionne dans une pièce,
qui n'accepte qu'un seul composant, comme dans un assemblage) :
  RoueA : centrée en (0, 0)
  RoueB : centrée en (CENTER_DISTANCE, 0), image miroir de A, décalée d'un demi-pas
Chaque roue est faite d'une esquisse du profil de denture (développante, splines
ajustées) puis de deux balayages avec torsion, un vers le haut et un vers le bas,
à partir du plan de symétrie z = 0 (la roue va donc de z = -WIDTH/2 à z = +WIDTH/2).
Aucun alésage : à percer ensuite dans Fusion.

Après création, tout reste modifiable dans la chronologie : l'esquisse du profil,
les chemins de balayage et l'angle de torsion de chaque balayage.

Installation : Utilitaires > Scripts et compléments > « + » > Créer un script,
nom « HerringboneGears », puis remplacer le contenu du .py par ce fichier.

Unités : le script est écrit en mm et en degrés, converti en cm et radians pour l'API.
"""
import math
import traceback

import adsk.core
import adsk.fusion

# ---------------------------------------------------------------- paramètres
TEETH = 46               # nombre de dents (identique sur les deux roues, rapport 1:1)
CENTER_DISTANCE = 92.0   # entraxe [mm] ; diamètre primitif = entraxe (rapport 1:1)
HELIX_ANGLE = 30.0       # angle d'hélice au primitif [deg]
PRESSURE_ANGLE_N = 20.0  # angle de pression normal [deg]
WIDTH = 10.0             # hauteur totale de la roue [mm]
BACKLASH = 0.10          # jeu circonférentiel total entre les deux roues [mm]
FLIP_HAND = False        # True : inverse le sens des chevrons (A et B échangent leur rôle)
FLANK_POINTS = 10        # points d'interpolation par flanc de dent
SHOW_START_BOX = True    # True : boîte « script démarré » dès le lancement (diagnostic)

MM = 0.1                 # 1 mm = 0,1 cm (unité interne de Fusion)


# ------------------------------------------------- géométrie (sans API Fusion)
def inv(alpha):
    return math.tan(alpha) - alpha


def gear_params(teeth, center_distance, helix_deg, pressure_n_deg, backlash):
    beta = math.radians(helix_deg)
    alpha_n = math.radians(pressure_n_deg)
    m_t = center_distance / teeth                 # module transversal
    m_n = m_t * math.cos(beta)                    # module normal
    alpha_t = math.atan(math.tan(alpha_n) / math.cos(beta))
    r_p = teeth * m_t / 2.0
    r_b = r_p * math.cos(alpha_t)
    r_a = r_p + 1.00 * m_n
    r_f = r_p - 1.25 * m_n
    if r_f < r_b:
        raise ValueError(
            "Pas assez de dents : le pied de dent passe sous le cercle de base "
            "(r_f = %.2f mm < r_b = %.2f mm)." % (r_f, r_b))
    s_p = math.pi * m_t / 2.0 - backlash / 2.0
    psi_p = s_p / (2.0 * r_p)
    return {"beta": beta, "alpha_t": alpha_t, "m_t": m_t, "m_n": m_n, "r_p": r_p,
            "r_b": r_b, "r_a": r_a, "r_f": r_f, "psi_p": psi_p}


def half_angle(r, p):
    alpha_r = math.acos(min(1.0, p["r_b"] / r))
    return p["psi_p"] + inv(p["alpha_t"]) - inv(alpha_r)


def polar(r, ang):
    return (r * math.cos(ang), r * math.sin(ang))


def outline_curves(teeth, p, n_flank=FLANK_POINTS):
    """Contour fermé de la roue : liste de courbes (listes de points (x, y) en mm).

    Pour chaque dent, dans le sens trigonométrique : flanc droit, sommet, flanc gauche,
    pied. Les extrémités de deux courbes consécutives sont le même tuple Python, donc
    exactement les mêmes coordonnées (contour fermé sans trou numérique).
    """
    r_start = p["r_f"]
    radii = [r_start + (p["r_a"] - r_start) * i / (n_flank - 1) for i in range(n_flank)]
    psi_a = half_angle(p["r_a"], p)
    if psi_a <= 0:
        raise ValueError("Pointe de dent d'épaisseur négative.")
    rights, lefts = [], []
    for k in range(teeth):
        c = 2.0 * math.pi * k / teeth
        rights.append([polar(r, c - half_angle(r, p)) for r in radii])
        lefts.append([polar(r, c + half_angle(r, p)) for r in reversed(radii)])
    psi_f = half_angle(r_start, p)
    if 2.0 * psi_f >= 2.0 * math.pi / teeth:
        raise ValueError("Dents qui se chevauchent au pied.")
    curves = []
    for k in range(teeth):
        c = 2.0 * math.pi * k / teeth
        right, left = rights[k], lefts[k]
        nxt = rights[(k + 1) % teeth]
        tip_mid = polar(p["r_a"], c)
        root_mid = polar(p["r_f"], c + math.pi / teeth)
        curves.append(right)
        curves.append([right[-1], tip_mid, left[0]])
        curves.append(left)
        curves.append([left[-1], root_mid, nxt[0]])
    return curves


def transform_curves(curves, mirror, rot, dx):
    """Miroir (x -> -x), rotation autour de l'origine, puis translation en x."""
    cr, sr = math.cos(rot), math.sin(rot)
    memo = {}

    def tf(pt):
        key = id(pt)
        if key not in memo:
            x, y = pt
            if mirror:
                x = -x
            memo[key] = (x * cr - y * sr + dx, x * sr + y * cr)
        return memo[key]

    return [[tf(pt) for pt in crv] for crv in curves]


# ------------------------------------------------------------- API Fusion
def point_in_sketch(sketch, x_mm, y_mm):
    return sketch.modelToSketchSpace(adsk.core.Point3D.create(x_mm * MM, y_mm * MM, 0.0))


def make_path(comp, name, cx_mm, z_end_mm):
    """Chemin de balayage : droite verticale de (cx, 0, 0) à (cx, 0, z_end)."""
    sk = comp.sketches.add(comp.xZConstructionPlane)
    sk.name = name
    p0 = sk.modelToSketchSpace(adsk.core.Point3D.create(cx_mm * MM, 0.0, 0.0))
    p1 = sk.modelToSketchSpace(adsk.core.Point3D.create(cx_mm * MM, 0.0, z_end_mm * MM))
    line = sk.sketchCurves.sketchLines.addByTwoPoints(p0, p1)
    return comp.features.createPath(line)


def build_gear(comp, name, curves_mm, cx_mm, twist_up_rad):
    # esquisse du profil dans le plan de symétrie (plan XY, z = 0)
    sketch = comp.sketches.add(comp.xYConstructionPlane)
    sketch.name = name + "_profil"
    splines = sketch.sketchCurves.sketchFittedSplines
    for pts in curves_mm:
        coll = adsk.core.ObjectCollection.create()
        for (x, y) in pts:
            coll.add(point_in_sketch(sketch, x, y))
        splines.add(coll)

    log("%s : %d courbes dans l'esquisse, %d profil(s)" % (name, len(curves_mm), sketch.profiles.count))
    if sketch.profiles.count == 0:
        raise RuntimeError("Aucun profil fermé détecté dans l'esquisse " + sketch.name)
    profile = sketch.profiles.item(0)
    for i in range(1, sketch.profiles.count):
        cand = sketch.profiles.item(i)
        if cand.areaProperties().area > profile.areaProperties().area:
            profile = cand
    profile_area_cm2 = profile.areaProperties().area

    half = WIDTH / 2.0
    path_up = make_path(comp, name + "_chemin_haut", cx_mm, +half)
    path_down = make_path(comp, name + "_chemin_bas", cx_mm, -half)

    log(name + " : profil et chemins prêts, création des balayages")
    sweeps = comp.features.sweepFeatures
    # moitié haute : torsion +t sur +z ; moitié basse : chemin dirigé vers -z, donc torsion
    # -t pour obtenir la même rotation au sommet et au pied (symétrie miroir en z = 0 : chevron)
    inp_up = sweeps.createInput(
        profile, path_up, adsk.fusion.FeatureOperations.NewBodyFeatureOperation)
    inp_up.twistAngle = adsk.core.ValueInput.createByReal(twist_up_rad)
    sw_up = sweeps.add(inp_up)
    log(name + " : balayage haut créé")

    inp_dn = sweeps.createInput(
        profile, path_down, adsk.fusion.FeatureOperations.NewBodyFeatureOperation)
    inp_dn.twistAngle = adsk.core.ValueInput.createByReal(-twist_up_rad)
    sw_dn = sweeps.add(inp_dn)
    log(name + " : balayage bas créé")

    # les deux moitiés sont fusionnées explicitement (corps haut = cible, corps bas = outil)
    body = sw_up.bodies.item(0)
    tools = adsk.core.ObjectCollection.create()
    tools.add(sw_dn.bodies.item(0))
    combines = comp.features.combineFeatures
    cin = combines.createInput(body, tools)
    cin.operation = adsk.fusion.FeatureOperations.JoinFeatureOperation
    combines.add(cin)

    body.name = name
    log("%s : volume %.3f cm3" % (name, body.volume))
    expected_cm3 = profile_area_cm2 * WIDTH * MM
    return body.volume, expected_cm3


_APP = None


def log(msg):
    """Écrit dans la fenêtre « Commandes de texte » de Fusion (si disponible)."""
    try:
        if _APP is not None:
            _APP.log("[HerringboneGears] " + msg)
    except Exception:
        pass


def run(context):
    global _APP
    ui = None
    try:
        app = adsk.core.Application.get()
        _APP = app
        ui = app.userInterface
        log("démarrage")
        if SHOW_START_BOX:
            ui.messageBox("HerringboneGears : le script démarre.\nClique OK pour continuer.")
        design = adsk.fusion.Design.cast(app.activeProduct)
        if design is None:
            ui.messageBox("Ouvre (ou crée) un document de conception Fusion, puis relance le script.")
            return
        if design.designType != adsk.fusion.DesignTypes.ParametricDesignType:
            ui.messageBox("Active l'historique de conception (mode paramétrique) puis relance le script : "
                          "sans chronologie, les esquisses et balayages ne resteraient pas modifiables.")
            return

        log("document : " + str(design.parentDocument.name))
        p = gear_params(TEETH, CENTER_DISTANCE, HELIX_ANGLE, PRESSURE_ANGLE_N, BACKLASH)
        base = outline_curves(TEETH, p)
        log("profil calculé : %d courbes" % len(base))
        theta = (WIDTH / 2.0) * math.tan(p["beta"]) / p["r_p"]   # torsion d'une demi-hauteur [rad]
        sign = -1.0 if FLIP_HAND else 1.0

        root = design.rootComponent
        # roue A : profil tel quel en (0, 0)
        curves_a = transform_curves(base, mirror=False, rot=0.0, dx=0.0)
        va, ea = build_gear(root, "RoueA", curves_a, 0.0, +sign * theta)
        # roue B : miroir de A (hélices de sens opposé), demi-pas de décalage, en (entraxe, 0)
        curves_b = transform_curves(base, mirror=True, rot=math.pi / TEETH, dx=CENTER_DISTANCE)
        vb, eb = build_gear(root, "RoueB", curves_b, CENTER_DISTANCE, -sign * theta)

        warn = ""
        for label, v, e in (("RoueA", va, ea), ("RoueB", vb, eb)):
            if abs(v - e) / e > 0.005:
                warn += "\n%s : volume %.2f cm3 (attendu %.2f cm3)" % (label, v, e)
        if root.bRepBodies.count != 2:
            warn += "\nNombre de corps = %d (attendu 2)" % root.bRepBodies.count
        ui.messageBox(
            "Roues créées.\n"
            "m_t = %.4f mm, m_n = %.4f mm, d primitif = %.2f mm\n"
            "d tête = %.2f mm, d pied = %.2f mm\n"
            "Torsion par demi-hauteur : %.3f deg\n"
            "Volume RoueA : %.2f cm3 (attendu %.2f)\n"
            "Contrôle : Inspecter > Interférence sur les deux corps (doit être nul)."
            % (p["m_t"], p["m_n"], 2 * p["r_p"], 2 * p["r_a"], 2 * p["r_f"],
               math.degrees(theta), va, ea)
            + (("\n\nÀ VÉRIFIER :" + warn) if warn else ""))
    except Exception:
        if ui:
            ui.messageBox("Échec du script :\n" + traceback.format_exc())
