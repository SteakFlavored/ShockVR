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
#define MINIPISTOL_TRIPLE             0x0        //  (0)
#define DARTPISTOL_TRIPLE             0x1        //  (1)
#define MAGNUM_TRIPLE                  0x2        //  (2)
#define ASSAULTRFL_TRIPLE             0x3        //  (3)
#define RIOTGUN_TRIPLE                 0x4        //  (4)
#define FLECHETTE_TRIPLE              0x100        //  (256)
#define SKORPION_TRIPLE                0x101        //  (257)
#define MAGPULSE_TRIPLE                0x200        //  (512)
#define RAILGUN_TRIPLE                 0x201        //  (513)
#define BATON_TRIPLE                    0x300        //  (768)
#define LASERAPIER_TRIPLE             0x301        //  (769)
#define PHASER_TRIPLE                  0x400        //  (1024)
#define BLASTER_TRIPLE                 0x401        //  (1025)
#define IONBEAM_TRIPLE                 0x402        //  (1026)
#define STUNGUN_TRIPLE                 0x500        //  (1280)
#define PLASMABEAM_TRIPLE             0x501        //  (1281)
#define SPAMMO_TRIPLE                  0x10000        //  (65536)
#define TEFAMMO_TRIPLE                 0x10001        //  (65537)
#define NNAMMO_TRIPLE                  0x10100        //  (65792)
#define TNAMMO_TRIPLE                  0x10101        //  (65793)
#define HTAMMO_TRIPLE                  0x10200        //  (66048)
#define HSAMMO_TRIPLE                  0x10201        //  (66049)
#define RBAMMO_TRIPLE                  0x10202        //  (66050)
#define MRAMMO_TRIPLE                  0x10300        //  (66304)
#define PRAMMO_TRIPLE                  0x10301        //  (66305)
#define HNAMMO_TRIPLE                  0x10400        //  (66560)
#define SPLAMMO_TRIPLE                 0x10401        //  (66561)
#define SLGAMMO_TRIPLE                 0x10500        //  (66816)
#define BGAMMO_TRIPLE                  0x10501        //  (66817)
#define MAGAMMO_TRIPLE                 0x10600        //  (67072)
#define RGAMMO_TRIPLE                  0x10601        //  (67073)
#define BULLTRACE_TRIPLE              0x20000        //  (131072)
#define ENERTRACE_TRIPLE              0x20001        //  (131073)
#define AUTOTRACE_TRIPLE              0x20002        //  (131074)
#define NEEDTRACE_TRIPLE              0x20003        //  (131075)
#define GRENTRACE_TRIPLE              0x20004        //  (131076)
#define RUBBTRACE_TRIPLE              0x20005        //  (131077)
#define VIRUSSLOW_TRIPLE              0x20100        //  (131328)
#define ELITESLOW_TRIPLE              0x20101        //  (131329)
#define ASSASINSLOW_TRIPLE            0x20102        //  (131330)
#define MUTANTSLOW_TRIPLE             0x20103        //  (131331)
#define ZEROSLOW_TRIPLE                0x20104        //  (131332)
#define MAGBURST_TRIPLE                0x20105        //  (131333)
#define RAILSLOW_TRIPLE                0x20106        //  (131334)
#define STUNSLOW_TRIPLE                0x20107        //  (131335)
#define PLASMABLST_TRIPLE             0x20108        //  (131336)
#define CYBERBOLT_TRIPLE              0x20109        //  (131337)
#define CYBERSLOW_TRIPLE              0x2010a        //  (131338)
#define DRILLSLOW_TRIPLE              0x2010b        //  (131339)
#define DISCSLOW_TRIPLE                0x2010c        //  (131340)
#define SPEWSLOW_TRIPLE                0x2010d        //  (131341)
#define PLANTSLOW_TRIPLE              0x2010e        //  (131342)
#define INVISOSLOW_TRIPLE             0x2010f        //  (131343)
#define DRONECAM_TRIPLE                0x20200        //  (131584)
#define EXPLCAM_TRIPLE                 0x20201        //  (131585)
#define FRAG_G_TRIPLE                  0x30000        //  (196608)
#define EMP_G_TRIPLE                    0x30001        //  (196609)
#define GAS_G_TRIPLE                    0x30002        //  (196610)
#define CONC_G_TRIPLE                  0x30003        //  (196611)
#define L_MINE_TRIPLE                  0x30004        //  (196612)
#define NITRO_G_TRIPLE                 0x30100        //  (196864)
#define EARTH_G_TRIPLE                 0x30101        //  (196865)
#define OBJ_G_TRIPLE                    0x30102        //  (196866)
#define STAMINA_DRUG_TRIPLE          0x40000        //  (262144)
#define SIGHT_DRUG_TRIPLE             0x40001        //  (262145)
#define LSD_DRUG_TRIPLE                0x40002        //  (262146)
#define MEDI_DRUG_TRIPLE              0x40003        //  (262147)
#define NINJA_DRUG_TRIPLE             0x40004        //  (262148)
#define GENIUS_DRUG_TRIPLE            0x40005        //  (262149)
#define DETOX_DRUG_TRIPLE             0x40006        //  (262150)
#define INFRA_GOG_TRIPLE              0x50000        //  (327680)
#define TARG_GOG_TRIPLE                0x50001        //  (327681)
#define SENS_HARD_TRIPLE              0x50002        //  (327682)
#define AIM_GOG_TRIPLE                 0x50003        //  (327683)
#define HUD_GOG_TRIPLE                 0x50004        //  (327684)
#define BIOSCAN_HARD_TRIPLE          0x50100        //  (327936)
#define NAV_HARD_TRIPLE                0x50101        //  (327937)
#define SHIELD_HARD_TRIPLE            0x50102        //  (327938)
#define VIDTEX_HARD_TRIPLE            0x50103        //  (327939)
#define LANTERN_HARD_TRIPLE          0x50104        //  (327940)
#define FULLSCR_HARD_TRIPLE          0x50105        //  (327941)
#define ENV_HARD_TRIPLE                0x50106        //  (327942)
#define MOTION_HARD_TRIPLE            0x50107        //  (327943)
#define JET_HARD_TRIPLE                0x50108        //  (327944)
#define STATUS_HARD_TRIPLE            0x50109        //  (327945)
#define DRILL_TRIPLE                    0x60000        //  (393216)
#define SPEW_TRIPLE                     0x60001        //  (393217)
#define MINE_TRIPLE                     0x60002        //  (393218)
#define DISC_TRIPLE                     0x60003        //  (393219)
#define PULSER_TRIPLE                  0x60004        //  (393220)
#define SCRAMBLER_TRIPLE              0x60005        //  (393221)
#define VIRUS_TRIPLE                    0x60006        //  (393222)
#define SHIELD_TRIPLE                  0x60100        //  (393472)
#define OLD_FAKEID_TRIPLE             0x60101        //  (393473)
#define ICE_TRIPLE                      0x60102        //  (393474)
#define TURBO_TRIPLE                    0x60200        //  (393728)
#define FAKEID_TRIPLE                  0x60201        //  (393729)
#define DECOY_TRIPLE                    0x60202        //  (393730)
#define RECALL_TRIPLE                  0x60203        //  (393731)
#define GAMES_TRIPLE                    0x60300        //  (393984)
#define MONITOR1_TRIPLE                0x60301        //  (393985)
#define IDENTIFY_TRIPLE                0x60302        //  (393986)
#define TRACE_TRIPLE                    0x60303        //  (393987)
#define TOGGLE_TRIPLE                  0x60304        //  (393988)
#define TEXT1_TRIPLE                    0x60400        //  (394240)
#define EMAIL1_TRIPLE                  0x60401        //  (394241)
#define MAP1_TRIPLE                     0x60402        //  (394242)
#define PHONE_TRIPLE                    0x70000        //  (458752)
#define VCR_TRIPLE                      0x70001        //  (458753)
#define MICROWAVE_OVN_TRIPLE         0x70002        //  (458754)
#define STEREO_TRIPLE                  0x70003        //  (458755)
#define KEYBOARD_TRIPLE                0x70004        //  (458756)
#define SMALL_CPU_TRIPLE              0x70005        //  (458757)
#define TV_TRIPLE                        0x70006        //  (458758)
#define MONITOR2_TRIPLE                0x70007        //  (458759)
#define LARGCPU_TRIPLE                 0x70008        //  (458760)
#define LDESK_TRIPLE                    0x70100        //  (459008)
#define FDESK_TRIPLE                    0x70101        //  (459009)
#define CABINET_TRIPLE                 0x70102        //  (459010)
#define SHELF_TRIPLE                    0x70103        //  (459011)
#define HIDEAWAY_TRIPLE                0x70104        //  (459012)
#define CHAIR_TRIPLE                    0x70105        //  (459013)
#define ENDTABLE_TRIPLE                0x70106        //  (459014)
#define COUCH_TRIPLE                    0x70107        //  (459015)
#define EXECCHR_TRIPLE                 0x70108        //  (459016)
#define COATTREE_TRIPLE                0x70109        //  (459017)
#define SIGN_TRIPLE                     0x70200        //  (459264)
#define ICON_TRIPLE                     0x70201        //  (459265)
#define GRAF_TRIPLE                     0x70202        //  (459266)
#define WORDS_TRIPLE                    0x70203        //  (459267)
#define PAINTING_TRIPLE                0x70204        //  (459268)
#define POSTER_TRIPLE                  0x70205        //  (459269)
#define SCREEN_TRIPLE                  0x70206        //  (459270)
#define TMAP_TRIPLE                     0x70207        //  (459271)
#define SUPERSCREEN_TRIPLE            0x70208        //  (459272)
#define BIGSCREEN_TRIPLE              0x70209        //  (459273)
#define REPULSWALL_TRIPLE             0x7020a        //  (459274)
#define DESKLAMP_TRIPLE                0x70300        //  (459520)
#define FLOORLAMP_TRIPLE              0x70301        //  (459521)
#define GLOWBULB_TRIPLE                0x70302        //  (459522)
#define CHAND_TRIPLE                    0x70303        //  (459523)
#define GENE_SPLICER_TRIPLE          0x70400        //  (459776)
#define TUBING_TRIPLE                  0x70401        //  (459777)
#define MED_CART_TRIPLE                0x70402        //  (459778)
#define SURG_MACH_TRIPLE              0x70403        //  (459779)
#define TTUBE_RACK_TRIPLE             0x70404        //  (459780)
#define RSRCH_CHAIR_TRIPLE            0x70405        //  (459781)
#define HOSP_BED_TRIPLE                0x70406        //  (459782)
#define BROKLAB1_TRIPLE                0x70407        //  (459783)
#define BROKLAB2_TRIPLE                0x70408        //  (459784)
#define MICROSCOPE_TRIPLE             0x70500        //  (460032)
#define SCOPE_TRIPLE                    0x70501        //  (460033)
#define LAB_PROBE_TRIPLE              0x70502        //  (460034)
#define XRAY_MACHINE_TRIPLE          0x70503        //  (460035)
#define CAMERA_TRIPLE                  0x70504        //  (460036)
#define CONTPAN_TRIPLE                 0x70505        //  (460037)
#define CONTPED_TRIPLE                 0x70506        //  (460038)
#define ENERGY_MINE_TRIPLE            0x70507        //  (460039)
#define STATUE1_TRIPLE                 0x70600        //  (460288)
#define SHRUB1_TRIPLE                  0x70601        //  (460289)
#define GRASS_TRIPLE                    0x70602        //  (460290)
#define PLANT1_TRIPLE                  0x70603        //  (460291)
#define FUNG1_TRIPLE                    0x70604        //  (460292)
#define FUNG2_TRIPLE                    0x70605        //  (460293)
#define PLANT2_TRIPLE                  0x70606        //  (460294)
#define VINE1_TRIPLE                    0x70607        //  (460295)
#define VINE2_TRIPLE                    0x70608        //  (460296)
#define PLANT3_TRIPLE                  0x70609        //  (460297)
#define PLANT4_TRIPLE                  0x7060a        //  (460298)
#define LBOULDER_TRIPLE                0x7060b        //  (460299)
#define BBOULDER_TRIPLE                0x7060c        //  (460300)
#define SHRUB2_TRIPLE                  0x7060d        //  (460301)
#define VSHRUB1_TRIPLE                 0x7060e        //  (460302)
#define VSHRUB2_TRIPLE                 0x7060f        //  (460303)
#define BRIDGE_TRIPLE                  0x70700        //  (460544)
#define CATWALK_TRIPLE                 0x70701        //  (460545)
#define WALL_TRIPLE                     0x70702        //  (460546)
#define FPILLAR_TRIPLE                 0x70703        //  (460547)
#define RAILING1_TRIPLE                0x70704        //  (460548)
#define RAILING2_TRIPLE                0x70705        //  (460549)
#define PILLAR_TRIPLE                  0x70706        //  (460550)
#define FORCE_BRIJ_TRIPLE             0x70707        //  (460551)
#define NON_BRIDGE_TRIPLE             0x70708        //  (460552)
#define FORCE_BRIJ2_TRIPLE            0x70709        //  (460553)
#define BEV_CONT_TRIPLE                0x80000        //  (524288)
#define WRAPPER_TRIPLE                 0x80001        //  (524289)
#define PAPERS_TRIPLE                  0x80002        //  (524290)
#define WARECASING_TRIPLE             0x80003        //  (524291)
#define EXTING_TRIPLE                  0x80004        //  (524292)
#define HELMET_TRIPLE                  0x80005        //  (524293)
#define CLOTHES_TRIPLE                 0x80006        //  (524294)
#define BRIEFCASE_TRIPLE              0x80007        //  (524295)
#define BROKEN_GUN_TRIPLE             0x80100        //  (524544)
#define MCHUNK1_TRIPLE                 0x80101        //  (524545)
#define MCHUNK2_TRIPLE                 0x80102        //  (524546)
#define MCHUNK3_TRIPLE                 0x80103        //  (524547)
#define CRATE_FRAG_TRIPLE             0x80104        //  (524548)
#define BROKEN_PAN_TRIPLE             0x80105        //  (524549)
#define BROKEN_CLK_TRIPLE             0x80106        //  (524550)
#define MSCRAP_TRIPLE                  0x80107        //  (524551)
#define BROKEN_LEV1_TRIPLE            0x80108        //  (524552)
#define BROKEN_LEV2_TRIPLE            0x80109        //  (524553)
#define CORPSE1_TRIPLE                 0x80200        //  (524800)
#define CORPSE2_TRIPLE                 0x80201        //  (524801)
#define CORPSE3_TRIPLE                 0x80202        //  (524802)
#define CORPSE4_TRIPLE                 0x80203        //  (524803)
#define CORPSE5_TRIPLE                 0x80204        //  (524804)
#define CORPSE6_TRIPLE                 0x80205        //  (524805)
#define CORPSE7_TRIPLE                 0x80206        //  (524806)
#define CORPSE8_TRIPLE                 0x80207        //  (524807)
#define SKEL_RAGS_TRIPLE              0x80208        //  (524808)
#define BONES1_TRIPLE                  0x80209        //  (524809)
#define BONES2_TRIPLE                  0x8020a        //  (524810)
#define SKULL_TRIPLE                    0x8020b        //  (524811)
#define LIMB_TRIPLE                     0x8020c        //  (524812)
#define HEAD_TRIPLE                     0x8020d        //  (524813)
#define HEAD2_TRIPLE                    0x8020e        //  (524814)
#define EPICK_TRIPLE                    0x80300        //  (525056)
#define BATTERY2_TRIPLE                0x80301        //  (525057)
#define ROD_TRIPLE                      0x80302        //  (525058)
#define AIDKIT_TRIPLE                  0x80303        //  (525059)
#define TRACBEAM_TRIPLE                0x80304        //  (525060)
#define BATTERY_TRIPLE                 0x80305        //  (525061)
#define GENCARDS_TRIPLE                0x80400        //  (525312)
#define STDCARD_TRIPLE                 0x80401        //  (525313)
#define SCICARD_TRIPLE                 0x80402        //  (525314)
#define STORECARD_TRIPLE              0x80403        //  (525315)
#define ENGCARD_TRIPLE                 0x80404        //  (525316)
#define MEDCARD_TRIPLE                 0x80405        //  (525317)
#define MAINTCARD_TRIPLE              0x80406        //  (525318)
#define ADMINCARD_TRIPLE              0x80407        //  (525319)
#define SECCARD_TRIPLE                 0x80408        //  (525320)
#define COMCARD_TRIPLE                 0x80409        //  (525321)
#define GROUPCARD_TRIPLE              0x8040a        //  (525322)
#define PERSCARD_TRIPLE                0x8040b        //  (525323)
#define MULTIPLEXR_TRIPLE             0x80500        //  (525568)
#define CYBERHEAL_TRIPLE              0x80501        //  (525569)
#define CYBERMINE_TRIPLE              0x80502        //  (525570)
#define CYBERCARD_TRIPLE              0x80503        //  (525571)
#define SHODO_SHRINE_TRIPLE          0x80504        //  (525572)
#define ICEWALL_TRIPLE                 0x80505        //  (525573)
#define INFONODE_TRIPLE                0x80506        //  (525574)
#define CSPACE_EXIT_TRIPLE            0x80507        //  (525575)
#define DATALET_TRIPLE                 0x80508        //  (525576)
#define BARRICADE_TRIPLE              0x80509        //  (525577)
#define TARGET_TRIPLE                  0x8050a        //  (525578)
#define ARROW_TRIPLE                    0x8050b        //  (525579)
#define BEAMBLST_TRIPLE                0x80600        //  (525824)
#define ACIDCORR_TRIPLE                0x80601        //  (525825)
#define BULLETHOLE_TRIPLE             0x80602        //  (525826)
#define EXBLAST_TRIPLE                 0x80603        //  (525827)
#define BURNRES_TRIPLE                 0x80604        //  (525828)
#define BLOODSTN_TRIPLE                0x80605        //  (525829)
#define CHEMSPLAT_TRIPLE              0x80606        //  (525830)
#define OILPUDDLE_TRIPLE              0x80607        //  (525831)
#define WASTESPILL_TRIPLE             0x80608        //  (525832)
#define ISOTOPE_X_TRIPLE              0x80700        //  (526080)
#define CIRCBOARD1_TRIPLE             0x80701        //  (526081)
#define PLASTIQUE_TRIPLE              0x80702        //  (526082)
#define FAUX_X_TRIPLE                  0x80703        //  (526083)
#define CIRCBOARD4_TRIPLE             0x80704        //  (526084)
#define CIRCBOARD5_TRIPLE             0x80705        //  (526085)
#define CIRCBOARD6_TRIPLE             0x80706        //  (526086)
#define CIRCBOARD7_TRIPLE             0x80707        //  (526087)
#define SWITCH1_TRIPLE                 0x90000        //  (589824)
#define SWITCH2_TRIPLE                 0x90001        //  (589825)
#define BUTTON1_TRIPLE                 0x90002        //  (589826)
#define BUTTON2_TRIPLE                 0x90003        //  (589827)
#define LEVER1_TRIPLE                  0x90004        //  (589828)
#define LEVER2_TRIPLE                  0x90005        //  (589829)
#define BIGRED_TRIPLE                  0x90006        //  (589830)
#define BIGLEVER_TRIPLE                0x90007        //  (589831)
#define DIAL_TRIPLE                     0x90008        //  (589832)
#define ACCESS_SLOT_TRIPLE            0x90100        //  (590080)
#define CRCT_BD_SLOT_TRIPLE          0x90101        //  (590081)
#define CHEM_RECEPT_TRIPLE            0x90102        //  (590082)
#define ANTENNA_PAN_TRIPLE            0x90103        //  (590083)
#define PLAS_ANTENNA_TRIPLE          0x90104        //  (590084)
#define DEST_ANTENNA_TRIPLE          0x90105        //  (590085)
#define RETSCANNER_TRIPLE             0x90106        //  (590086)
#define CYB_TERM_TRIPLE                0x90200        //  (590336)
#define ENRG_CHARGE_TRIPLE            0x90201        //  (590337)
#define FIXUP_STATION_TRIPLE         0x90202        //  (590338)
#define ACCPANEL1_TRIPLE              0x90300        //  (590592)
#define ACCPANEL2_TRIPLE              0x90301        //  (590593)
#define ACCPANEL3_TRIPLE              0x90302        //  (590594)
#define ACCPANEL4_TRIPLE              0x90303        //  (590595)
#define ELEPANEL1_TRIPLE              0x90304        //  (590596)
#define ELEPANEL2_TRIPLE              0x90305        //  (590597)
#define ELEPANEL3_TRIPLE              0x90306        //  (590598)
#define KEYPAD1_TRIPLE                 0x90307        //  (590599)
#define KEYPAD2_TRIPLE                 0x90308        //  (590600)
#define ACCPANEL5_TRIPLE              0x90309        //  (590601)
#define ACCPANEL6_TRIPLE              0x9030a        //  (590602)
#define AMMOVEND_TRIPLE                0x90400        //  (590848)
#define HEALVEND_TRIPLE                0x90401        //  (590849)
#define CYBERTOG1_TRIPLE              0x90500        //  (591104)
#define CYBERTOG2_TRIPLE              0x90501        //  (591105)
#define CYBERTOG3_TRIPLE              0x90502        //  (591106)
#define BLAST_DOOR_TRIPLE             0xa0000        //  (655360)
#define ACCESS_DOOR_TRIPLE            0xa0001        //  (655361)
#define RESID_DOOR_TRIPLE             0xa0002        //  (655362)
#define MAINT_DOOR_TRIPLE             0xa0003        //  (655363)
#define HOSP_DOOR_TRIPLE              0xa0004        //  (655364)
#define LAB_DOOR_TRIPLE                0xa0005        //  (655365)
#define STOR_DOOR_TRIPLE              0xa0006        //  (655366)
#define REACTR_DOOR_TRIPLE            0xa0007        //  (655367)
#define EXEC_DOOR_TRIPLE              0xa0008        //  (655368)
#define NO_DOOR_TRIPLE                 0xa0009        //  (655369)
#define LAB_DOORWAY_TRIPLE            0xa0100        //  (655616)
#define RES_DOORWAY_TRIPLE            0xa0101        //  (655617)
#define BRJ_DOORWAY_TRIPLE            0xa0102        //  (655618)
#define RCT_DOORWAY_TRIPLE            0xa0103        //  (655619)
#define GRATING1_TRIPLE                0xa0104        //  (655620)
#define GRATING2_TRIPLE                0xa0105        //  (655621)
#define GRATING3_TRIPLE                0xa0106        //  (655622)
#define GRATING4_TRIPLE                0xa0107        //  (655623)
#define NO_DOOR2_TRIPLE                0xa0108        //  (655624)
#define LABFORCE_TRIPLE                0xa0200        //  (655872)
#define BROKLABFORCE_TRIPLE          0xa0201        //  (655873)
#define RESFORCE_TRIPLE                0xa0202        //  (655874)
#define BROKRESFORCE_TRIPLE          0xa0203        //  (655875)
#define GENFORCE_TRIPLE                0xa0204        //  (655876)
#define CYBGENFORCE_TRIPLE            0xa0205        //  (655877)
#define NO_DOOR3_TRIPLE                0xa0206        //  (655878)
#define EXEC_ELEV_TRIPLE              0xa0300        //  (656128)
#define REG_ELEV1_TRIPLE              0xa0301        //  (656129)
#define REG_ELEV2_TRIPLE              0xa0302        //  (656130)
#define FREIGHT_ELEV_TRIPLE          0xa0303        //  (656131)
#define NO_DOOR4_TRIPLE                0xa0304        //  (656132)
#define DOUB_LEFTDOOR_TRIPLE         0xa0400        //  (656384)
#define DOUB_RITEDOOR_TRIPLE         0xa0401        //  (656385)
#define IRIS_TRIPLE                     0xa0402        //  (656386)
#define VERT_OPEN_TRIPLE              0xa0403        //  (656387)
#define VERT_SPLIT_TRIPLE             0xa0404        //  (656388)
#define NO_DOOR5_TRIPLE                0xa0405        //  (656389)
#define SECRET_DOOR1_TRIPLE          0xa0406        //  (656390)
#define SECRET_DOOR2_TRIPLE          0xa0407        //  (656391)
#define SECRET_DOOR3_TRIPLE          0xa0408        //  (656392)
#define INVISO_DOOR_TRIPLE            0xa0409        //  (656393)
#define ALERT_PANEL_OFF_TRIPLE      0xb0000        //  (720896)
#define ALERT_PANEL_ON_TRIPLE        0xb0001        //  (720897)
#define HORZ_KLAXOFF_TRIPLE          0xb0002        //  (720898)
#define HORZ_KLAXON_TRIPLE            0xb0003        //  (720899)
#define SPARK_CABLE_TRIPLE            0xb0004        //  (720900)
#define TWITCH_MUT2_TRIPLE            0xb0005        //  (720901)
#define MACHINE_TRIPLE                 0xb0006        //  (720902)
#define HOLOG_ANIM_TRIPLE             0xb0007        //  (720903)
#define TWITCH_MUT_TRIPLE             0xb0008        //  (720904)
#define BLOOD1_TRIPLE                  0xb0100        //  (721152)
#define CAMEXPL_TRIPLE                 0xb0101        //  (721153)
#define TVEXPL_TRIPLE                  0xb0102        //  (721154)
#define SIMPLSMOKE_TRIPLE             0xb0103        //  (721155)
#define PLANTEXPL_TRIPLE              0xb0104        //  (721156)
#define BULLETWALLHIT_TRIPLE         0xb0105        //  (721157)
#define BEAMWALLHIT_TRIPLE            0xb0106        //  (721158)
#define IMPACT_ANIM_TRIPLE            0xb0107        //  (721159)
#define BULL_ROBOT_TRIPLE             0xb0108        //  (721160)
#define BEAM_ROBOT1_TRIPLE            0xb0109        //  (721161)
#define BEAM_ROBOT2_TRIPLE            0xb010a        //  (721162)
#define EXPLOSION1_TRIPLE             0xb0200        //  (721408)
#define EXPLOSION2_TRIPLE             0xb0201        //  (721409)
#define EXPLOSION3_TRIPLE             0xb0202        //  (721410)
#define LG_EXPLOSION_TRIPLE          0xb0203        //  (721411)
#define MAGPULSEHIT_TRIPLE            0xb0204        //  (721412)
#define STUNHIT_TRIPLE                 0xb0205        //  (721413)
#define PLASMAHIT_TRIPLE              0xb0206        //  (721414)
#define SMOKEEXPL_TRIPLE              0xb0207        //  (721415)
#define CRATEEXPL_TRIPLE              0xb0208        //  (721416)
#define MNTR2EXPL_TRIPLE              0xb0209        //  (721417)
#define GASEXPL_TRIPLE                 0xb020a        //  (721418)
#define EMPEXPL_TRIPLE                 0xb020b        //  (721419)
#define CORP_HUM_EXPL_TRIPLE         0xb020c        //  (721420)
#define CORP_ROB_EXPL_TRIPLE         0xb020d        //  (721421)
#define ENTRY_TRIG_TRIPLE             0xc0000        //  (786432)
#define NULL_TRIG_TRIPLE              0xc0001        //  (786433)
#define FLOOR_TRIG_TRIPLE             0xc0002        //  (786434)
#define PLRDETH_TRIG_TRIPLE          0xc0003        //  (786435)
#define DETHWATCH_TRIG_TRIPLE        0xc0004        //  (786436)
#define AOE_ENT_TRIG_TRIPLE          0xc0005        //  (786437)
#define AOE_CON_TRIG_TRIPLE          0xc0006        //  (786438)
#define AI_HINT_TRIPLE                 0xc0007        //  (786439)
#define LEVEL_TRIG_TRIPLE             0xc0008        //  (786440)
#define CONTIN_TRIG_TRIPLE            0xc0009        //  (786441)
#define REPULSOR_TRIPLE                0xc000a        //  (786442)
#define ECOLOGY_TRIG_TRIPLE          0xc000b        //  (786443)
#define SHODO_TRIG_TRIPLE             0xc000c        //  (786444)
#define TRIPBEAM_TRIPLE                0xc0100        //  (786688)
#define BIOHAZARD_TRIPLE              0xc0200        //  (786944)
#define RADHAZARD_TRIPLE              0xc0201        //  (786945)
#define CHEMHAZARD_TRIPLE             0xc0202        //  (786946)
#define MAPNOTE_TRIPLE                 0xc0203        //  (786947)
#define MUSIC_MARK_TRIPLE             0xc0204        //  (786948)
#define SML_CRT_TRIPLE                 0xd0000        //  (851968)
#define LG_CRT_TRIPLE                  0xd0001        //  (851969)
#define SECURE_CONTR_TRIPLE          0xd0002        //  (851970)
#define RAD_BARREL_TRIPLE             0xd0100        //  (852224)
#define TOXIC_BARREL_TRIPLE          0xd0101        //  (852225)
#define CHEM_TANK_TRIPLE              0xd0102        //  (852226)
#define THERMOS_TRIPLE                 0xd0200        //  (852480)
#define VIAL_CONT_TRIPLE              0xd0201        //  (852481)
#define FLASK_CONT_TRIPLE             0xd0202        //  (852482)
#define BEAKER_CONT_TRIPLE            0xd0203        //  (852483)
#define MUT_CORPSE1_TRIPLE            0xd0300        //  (852736)
#define MUT_CORPSE2_TRIPLE            0xd0301        //  (852737)
#define MUT_CORPSE3_TRIPLE            0xd0302        //  (852738)
#define MUT_CORPSE4_TRIPLE            0xd0303        //  (852739)
#define MUT_CORPSE5_TRIPLE            0xd0304        //  (852740)
#define MUT_CORPSE6_TRIPLE            0xd0305        //  (852741)
#define MUT_CORPSE7_TRIPLE            0xd0306        //  (852742)
#define MUT_CORPSE8_TRIPLE            0xd0307        //  (852743)
#define ROB_CORPSE1_TRIPLE            0xd0400        //  (852992)
#define ROB_CORPSE2_TRIPLE            0xd0401        //  (852993)
#define ROB_CORPSE3_TRIPLE            0xd0402        //  (852994)
#define ROB_CORPSE4_TRIPLE            0xd0403        //  (852995)
#define ROB_CORPSE5_TRIPLE            0xd0404        //  (852996)
#define ROB_CORPSE6_TRIPLE            0xd0405        //  (852997)
#define ROB_CORPSE7_TRIPLE            0xd0406        //  (852998)
#define ROB_CORPSE8_TRIPLE            0xd0407        //  (852999)
#define ROB_CORPSE9_TRIPLE            0xd0408        //  (853000)
#define ROB_CORPSE10_TRIPLE          0xd0409        //  (853001)
#define ROB_CORPSE11_TRIPLE          0xd040a        //  (853002)
#define ROB_CORPSE12_TRIPLE          0xd040b        //  (853003)
#define ROB_CORPSE13_TRIPLE          0xd040c        //  (853004)
#define CYB_CORPSE1_TRIPLE            0xd0500        //  (853248)
#define CYB_CORPSE2_TRIPLE            0xd0501        //  (853249)
#define CYB_CORPSE3_TRIPLE            0xd0502        //  (853250)
#define CYB_CORPSE4_TRIPLE            0xd0503        //  (853251)
#define CYB_CORPSE5_TRIPLE            0xd0504        //  (853252)
#define CYB_CORPSE6_TRIPLE            0xd0505        //  (853253)
#define CYB_CORPSE7_TRIPLE            0xd0506        //  (853254)
#define OTH_CORPSE1_TRIPLE            0xd0600        //  (853504)
#define OTH_CORPSE2_TRIPLE            0xd0601        //  (853505)
#define OTH_CORPSE3_TRIPLE            0xd0602        //  (853506)
#define OTH_CORPSE4_TRIPLE            0xd0603        //  (853507)
#define OTH_CORPSE5_TRIPLE            0xd0604        //  (853508)
#define OTH_CORPSE6_TRIPLE            0xd0605        //  (853509)
#define OTH_CORPSE7_TRIPLE            0xd0606        //  (853510)
#define OTH_CORPSE8_TRIPLE            0xd0607        //  (853511)
#define HUMAN_CRIT_TRIPLE             0xe0000        //  (917504)
#define GOR_TIGER_TRIPLE              0xe0001        //  (917505)
#define INSECT_CRIT_TRIPLE            0xe0002        //  (917506)
#define AVIAN_CRIT_TRIPLE             0xe0003        //  (917507)
#define PLANT_CRIT_TRIPLE             0xe0004        //  (917508)
#define ZERO_CRIT_TRIPLE              0xe0005        //  (917509)
#define PLAYER_CRIT_TRIPLE            0xe0006        //  (917510)
#define INVISO_CRIT_TRIPLE            0xe0007        //  (917511)
#define VIRUS_CRIT_TRIPLE             0xe0008        //  (917512)
#define LIFT_BOT_TRIPLE                0xe0100        //  (917760)
#define REPAIRBOT_TRIPLE              0xe0101        //  (917761)
#define SERVBOT_TRIPLE                 0xe0102        //  (917762)
#define EXECBOT_TRIPLE                 0xe0103        //  (917763)
#define LGTURRET_TRIPLE                0xe0104        //  (917764)
#define HOPPER_TRIPLE                  0xe0105        //  (917765)
#define SECURITY_BOT1_TRIPLE         0xe0106        //  (917766)
#define SECURITY_BOT2_TRIPLE         0xe0107        //  (917767)
#define AUTOBOMB_TRIPLE                0xe0108        //  (917768)
#define REPAIRBOT2_TRIPLE             0xe0109        //  (917769)
#define FLIER_TRIPLE                    0xe010a        //  (917770)
#define SECURITY_BOT3_TRIPLE         0xe010b        //  (917771)
#define CYBORG_DRONE_TRIPLE          0xe0200        //  (918016)
#define WARRIOR_TRIPLE                 0xe0201        //  (918017)
#define ASSASSIN_TRIPLE                0xe0202        //  (918018)
#define CYBERBABE_TRIPLE              0xe0203        //  (918019)
#define ELITE_GUARD_TRIPLE            0xe0204        //  (918020)
#define CORTEX_REAVER_TRIPLE         0xe0205        //  (918021)
#define MUTANT_BORG_TRIPLE            0xe0206        //  (918022)
#define CYBERDOG_TRIPLE                0xe0300        //  (918272)
#define CYBERGUARD_TRIPLE             0xe0301        //  (918273)
#define CYBER_CORTEX_TRIPLE          0xe0302        //  (918274)
#define CYBER_DYN_ICE_TRIPLE         0xe0303        //  (918275)
#define CYBER_HNT_KIL_TRIPLE         0xe0304        //  (918276)
#define CYBER_SHODAN_TRIPLE          0xe0305        //  (918277)
#define CYBERGUARD2_TRIPLE            0xe0306        //  (918278)
#define ROBOBABE_TRIPLE                0xe0400        //  (918528)
#define DIEGO_TRIPLE                    0xe0401        //  (918529)
