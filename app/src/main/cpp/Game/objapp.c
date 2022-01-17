/*

Copyright (C) 2015-2018 Night Dive Studios, LLC.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
/*
** $Header: n:/project/cit/src/RCS/objapp.c 1.17 1994/05/14 03:30:03 xemu Exp $
 *
*/

#include "objects.h"
#include "objsim.h"
#include "objwpn.h"
#include "objwarez.h"
#include "objstuff.h"
#include "objgame.h"
#include "objcrit.h"
#include "objprop.h"
#include "map.h"

////////////////////////////// APPLICATION-SPECIFIC DATA
//
// Here we define the arrays for all the application-specific
// classes defined in objapp.h.  Follow this example, and there
// won't be any trouble.
//
// ## INSERT NEW CLASS HERE

/*const*/ ObjSpecHeader objSpecHeaders[NUM_CLASSES] =
{
   {NUM_OBJECTS_GUN, sizeof(ObjGun), (int8_t *)&objGuns},
   {NUM_OBJECTS_AMMO, sizeof(ObjAmmo), (int8_t *)&objAmmos},
   {NUM_OBJECTS_PHYSICS, sizeof(ObjPhysics), (int8_t *)&objPhysicss},
   {NUM_OBJECTS_GRENADE, sizeof(ObjGrenade), (int8_t *)&objGrenades},
   {NUM_OBJECTS_DRUG, sizeof(ObjDrug), (int8_t *)&objDrugs},
   {NUM_OBJECTS_HARDWARE, sizeof(ObjHardware), (int8_t *)&objHardwares},
   {NUM_OBJECTS_SOFTWARE, sizeof(ObjSoftware), (int8_t *)&objSoftwares},
   {NUM_OBJECTS_BIGSTUFF, sizeof(ObjBigstuff), (int8_t *)&objBigstuffs},
   {NUM_OBJECTS_SMALLSTUFF, sizeof(ObjSmallstuff), (int8_t *)&objSmallstuffs},
   {NUM_OBJECTS_FIXTURE, sizeof(ObjFixture), (int8_t *)&objFixtures},
   {NUM_OBJECTS_DOOR, sizeof(ObjDoor), (int8_t *)&objDoors},
   {NUM_OBJECTS_ANIMATING, sizeof(ObjAnimating), (int8_t *)&objAnimatings},
   {NUM_OBJECTS_TRAP, sizeof(ObjTrap), (int8_t *)&objTraps},
   {NUM_OBJECTS_CONTAINER, sizeof(ObjContainer), (int8_t *)&objContainers},
   {NUM_OBJECTS_CRITTER, sizeof(ObjCritter), (int8_t *)&objCritters},
};

//const ObjSpecHeader ObjPropHeader = {NUM_OBJECT, sizeof(ObjProp), &ObjProps}

const ObjSpecHeader ClassPropHeaders[NUM_CLASSES] =
{
   {NUM_GUN, sizeof(GunProp), (int8_t *)&GunProps},
   {NUM_AMMO, sizeof(AmmoProp), (int8_t *)&AmmoProps},
   {NUM_PHYSICS, sizeof(PhysicsProp), (int8_t *)&PhysicsProps},
   {NUM_GRENADE, sizeof(GrenadeProp), (int8_t *)&GrenadeProps},
   {NUM_DRUG, sizeof(DrugProp), (int8_t *)&DrugProps},
   {NUM_HARDWARE, sizeof(HardwareProp), (int8_t *)&HardwareProps},
   {NUM_SOFTWARE, sizeof(SoftwareProp), (int8_t *)&SoftwareProps},
   {NUM_BIGSTUFF, sizeof(BigstuffProp), (int8_t *)&BigstuffProps},
   {NUM_SMALLSTUFF, sizeof(SmallstuffProp), (int8_t *)&SmallstuffProps},
   {NUM_FIXTURE, sizeof(FixtureProp), (int8_t *)&FixtureProps},
   {NUM_DOOR, sizeof(DoorProp), (int8_t *)&DoorProps},
   {NUM_CONTAINER, sizeof(ContainerProp), (int8_t *)&ContainerProps},
   {NUM_CRITTER, sizeof(CritterProp), (int8_t *)&CritterProps},
};

const ObjSpecHeader SubclassPropHeaders[NUM_SUBCLASSES] =
{
   {NUM_PISTOL_GUN, sizeof(PistolGunProp), (int8_t *)&PistolGunProps},
   {NUM_AUTO_GUN, sizeof(AutoGunProp), (int8_t *)&AutoGunProps},
   {NUM_SPECIAL_GUN	      , sizeof(SpecialGunProp), (int8_t *)&SpecialGunProps},
   {NUM_HANDTOHAND_GUN    , sizeof(HandtohandGunProp), (int8_t *)&HandtohandGunProps},
   {NUM_BEAM_GUN	         , sizeof(BeamGunProp), (int8_t *)&BeamGunProps},
   {NUM_BEAMPROJ_GUN	   , sizeof(BeamprojGunProp), (int8_t *)&BeamprojGunProps},
   {NUM_PISTOL_AMMO	      , sizeof(PistolAmmoProp), (int8_t *)&PistolAmmoProps},
   {NUM_NEEDLE_AMMO	      , sizeof(NeedleAmmoProp), (int8_t *)&NeedleAmmoProps},
   {NUM_MAGNUM_AMMO	      , sizeof(MagnumAmmoProp), (int8_t *)&MagnumAmmoProps},
   {NUM_RIFLE_AMMO	      , sizeof(RifleAmmoProp), (int8_t *)&RifleAmmoProps},
   {NUM_FLECHETTE_AMMO	   , sizeof(FlechetteAmmoProp), (int8_t *)&FlechetteAmmoProps},
   {NUM_AUTO_AMMO	      , sizeof(AutoAmmoProp), (int8_t *)&AutoAmmoProps},
   {NUM_PROJ_AMMO	      , sizeof(ProjAmmoProp), (int8_t *)&ProjAmmoProps},
   {NUM_TRACER_PHYSICS    , sizeof(TracerPhysicsProp), (int8_t *)&TracerPhysicsProps},
   {NUM_SLOW_PHYSICS      , sizeof(SlowPhysicsProp), (int8_t *)&SlowPhysicsProps},
   {NUM_CAMERA_PHYSICS    , sizeof(CameraPhysicsProp), (int8_t *)&CameraPhysicsProps},
   {NUM_DIRECT_GRENADE    , sizeof(DirectGrenadeProp), (int8_t *)&DirectGrenadeProps},
   {NUM_TIMED_GRENADE     , sizeof(TimedGrenadeProp), (int8_t *)&TimedGrenadeProps},
   {NUM_STATS_DRUG	        , sizeof(StatsDrugProp), (int8_t *)&StatsDrugProps},
   {NUM_GOGGLE_HARDWARE	, sizeof(GoggleHardwareProp), (int8_t *)&GoggleHardwareProps},
   {NUM_HARDWARE_HARDWARE	, sizeof(HardwareHardwareProp), (int8_t *)&HardwareHardwareProps},
   {NUM_OFFENSE_SOFTWARE	, sizeof(OffenseSoftwareProp), (int8_t *)&OffenseSoftwareProps},
   {NUM_DEFENSE_SOFTWARE	, sizeof(DefenseSoftwareProp), (int8_t *)&DefenseSoftwareProps},
   {NUM_ONESHOT_SOFTWARE	, sizeof(OneshotSoftwareProp), (int8_t *)&OneshotSoftwareProps},
   {NUM_MISC_SOFTWARE	   , sizeof(MiscSoftwareProp), (int8_t *)&MiscSoftwareProps},
   {NUM_DATA_SOFTWARE     , sizeof(DataSoftwareProp), (int8_t *)&DataSoftwareProps},
   {NUM_ELECTRONIC_BIGSTUFF , sizeof(ElectronicBigstuffProp), (int8_t *)&ElectronicBigstuffProps},
   {NUM_FURNISHING_BIGSTUFF , sizeof(FurnishingBigstuffProp), (int8_t *)&FurnishingBigstuffProps},
   {NUM_ONTHEWALL_BIGSTUFF , sizeof(OnthewallBigstuffProp), (int8_t *)&OnthewallBigstuffProps},
   {NUM_LIGHT_BIGSTUFF , sizeof(LightBigstuffProp), (int8_t *)&LightBigstuffProps},
   {NUM_LABGEAR_BIGSTUFF , sizeof(LabgearBigstuffProp), (int8_t *)&LabgearBigstuffProps},
   {NUM_TECHNO_BIGSTUFF , sizeof(TechnoBigstuffProp), (int8_t *)&TechnoBigstuffProps},
   {NUM_DECOR_BIGSTUFF , sizeof(DecorBigstuffProp), (int8_t *)&DecorBigstuffProps},
   {NUM_TERRAIN_BIGSTUFF , sizeof(TerrainBigstuffProp), (int8_t *)&TerrainBigstuffProps},
   {NUM_USELESS_SMALLSTUFF , sizeof(UselessSmallstuffProp), (int8_t *)&UselessSmallstuffProps},
   {NUM_BROKEN_SMALLSTUFF , sizeof(BrokenSmallstuffProp), (int8_t *)&BrokenSmallstuffProps},
   {NUM_CORPSELIKE_SMALLSTUFF , sizeof(CorpselikeSmallstuffProp), (int8_t *)&CorpselikeSmallstuffProps},
   {NUM_GEAR_SMALLSTUFF , sizeof(GearSmallstuffProp), (int8_t *)&GearSmallstuffProps},
   {NUM_CARDS_SMALLSTUFF , sizeof(CardsSmallstuffProp), (int8_t *)&CardsSmallstuffProps},
   {NUM_CYBER_SMALLSTUFF , sizeof(CyberSmallstuffProp), (int8_t *)&CyberSmallstuffProps},
   {NUM_ONTHEWALL_SMALLSTUFF , sizeof(OnthewallSmallstuffProp), (int8_t *)&OnthewallSmallstuffProps},
   {NUM_PLOT_SMALLSTUFF , sizeof(PlotSmallstuffProp), (int8_t *)&PlotSmallstuffProps},
   {NUM_CONTROL_FIXTURE , sizeof(ControlFixtureProp), (int8_t *)&ControlFixtureProps},
   {NUM_RECEPTACLE_FIXTURE , sizeof(ReceptacleFixtureProp), (int8_t *)&ReceptacleFixtureProps},
   {NUM_TERMINAL_FIXTURE , sizeof(TerminalFixtureProp), (int8_t *)&TerminalFixtureProps},
   {NUM_PANEL_FIXTURE , sizeof(PanelFixtureProp), (int8_t *)&PanelFixtureProps},
   {NUM_VENDING_FIXTURE , sizeof(VendingFixtureProp), (int8_t *)&VendingFixtureProps},
   {NUM_CYBER_FIXTURE , sizeof(CyberFixtureProp), (int8_t *)&CyberFixtureProps},
   {NUM_NORMAL_DOOR , sizeof(NormalDoorProp), (int8_t *)&NormalDoorProps},
   {NUM_DOORWAYS_DOOR , sizeof(DoorwaysDoorProp), (int8_t *)&DoorwaysDoorProps},
   {NUM_FORCE_DOOR , sizeof(ForceDoorProp), (int8_t *)&ForceDoorProps},
   {NUM_ELEVATOR_DOOR , sizeof(ElevatorDoorProp), (int8_t *)&ElevatorDoorProps},
   {NUM_SPECIAL_DOOR , sizeof(SpecialDoorProp), (int8_t *)&SpecialDoorProps},
   {NUM_OBJECT_ANIMATING , sizeof(ObjectsAnimatingProp), (int8_t *)&ObjectsAnimatingProps},
   {NUM_TRANSITORY_ANIMATING , sizeof(TransitoryAnimatingProp), (int8_t *)&TransitoryAnimatingProps},
   {NUM_EXPLOSION_ANIMATING , sizeof(ExplosionAnimatingProp), (int8_t *)&ExplosionAnimatingProps},
   {NUM_TRIGGER_TRAP , sizeof(TriggerTrapProp), (int8_t *)&TriggerTrapProps},
   {NUM_FEEDBACKS_TRAP , sizeof(FeedbacksTrapProp), (int8_t *)&FeedbacksTrapProps},
   {NUM_SECRET_TRAP , sizeof(SecretTrapProp), (int8_t *)&SecretTrapProps},
   {NUM_ACTUAL_CONTAINER , sizeof(ActualContainerProp), (int8_t *)&ActualContainerProps},
   {NUM_WASTE_CONTAINER , sizeof(WasteContainerProp), (int8_t *)&WasteContainerProps},
   {NUM_LIQUID_CONTAINER , sizeof(LiquidContainerProp), (int8_t *)&LiquidContainerProps},
   {NUM_MUTANT_CORPSE_CONTAINER , sizeof(MutantCorpseContainerProp), (int8_t *)&MutantCorpseContainerProps},
   {NUM_ROBOT_CORPSE_CONTAINER , sizeof(RobotCorpseContainerProp), (int8_t *)&RobotCorpseContainerProps},
   {NUM_CYBORG_CORPSE_CONTAINER , sizeof(CyborgCorpseContainerProp), (int8_t *)&CyborgCorpseContainerProps},
   {NUM_OTHER_CORPSE_CONTAINER , sizeof(OtherCorpseContainerProp), (int8_t *)&OtherCorpseContainerProps},
   {NUM_MUTANT_CRITTER         , sizeof(MutantCritterProp), (int8_t *)&MutantCritterProps},
   {NUM_ROBOT_CRITTER          , sizeof(RobotCritterProp), (int8_t *)&RobotCritterProps},
   {NUM_CYBORG_CRITTER         , sizeof(CyborgCritterProp), (int8_t *)&CyborgCritterProps},
   {NUM_CYBER_CRITTER          , sizeof(CyberCritterProp), (int8_t *)&CyberCritterProps},
   {NUM_ROBOBABE_CRITTER       , sizeof(RobobabeCritterProp), (int8_t *)&RobobabeCritterProps},
};


////////////////////////////// APPLICATION-SPECIFIC FUNCTIONS

static int32_t map_x, map_y;

void ObjInfoInit (ObjInfo *info)
{
   info->type = 0;
	info->ph = -1;
}

void ObjRefStateBinIteratorInit (void)
{
	map_x = map_y = 0;
}

bool ObjRefStateBinIterator (ObjRefStateBin *bin)
{
	if (map_y == MAP_YSIZE) return FALSE;
	bin->sq.x = map_x; bin->sq.y = map_y;
	if (++map_x == MAP_XSIZE) { map_x = 0; map_y++; }
	return TRUE;
}
