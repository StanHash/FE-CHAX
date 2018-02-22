
.macro SET_ABS_FUNC name, value
	.global \name
	.type   \name, %function
	.set    \name, \value
.endm

.macro SET_ABS_DATA name, value
	.global \name
	.type   \name, %object
	.set    \name, \value
.endm

SET_ABS_DATA action_data,  (0x0203A958)

SET_ABS_DATA map_size,     (0x0202E4D4)

SET_ABS_DATA unit_map,     (0x0202E4D8)
SET_ABS_DATA terrain_map,  (0x0202E4DC)
SET_ABS_DATA movement_map, (0x0202E4E0)
SET_ABS_DATA range_map,    (0x0202E4E4)
SET_ABS_DATA fog_map,      (0x0202E4E8)
SET_ABS_DATA hidden_map,   (0x0202E4EC)

SET_ABS_DATA tile_index_map, (0x0859A9D4)

SET_ABS_DATA trap_array,   (0x0203A614)

SET_ABS_DATA active_unit,  (0x03004E50)

SET_ABS_DATA sin_table,    (0x080D751C)
SET_ABS_DATA cos_table,    (0x080D759C)

@ <RNG Systems>

SET_ABS_FUNC NextRN,          (0x08000B88+1)
SET_ABS_FUNC LoadRNState,     (0x08000C34+1)
SET_ABS_FUNC SaveRNState,     (0x08000C4C+1)
SET_ABS_FUNC NextRN_100,      (0x08000C64+1)
SET_ABS_FUNC NextRN_N,        (0x08000C80+1)
SET_ABS_FUNC Roll1RN,         (0x08000CA0+1)
SET_ABS_FUNC Roll2RN,         (0x08000CB8+1)

@ </RNG Systems>

@ <6C/Proc System>

SET_ABS_FUNC New6C,               (0x08002C7C+1)
SET_ABS_FUNC New6CBlocking,       (0x08002CE0+1)
SET_ABS_FUNC Delete6C,            (0x08002D6C+1)
SET_ABS_FUNC Exec6C,              (0x08002E84+1)
SET_ABS_FUNC Break6CLoop,         (0x08002E94+1)
SET_ABS_FUNC Find6C,              (0x08002E9C+1)
SET_ABS_FUNC Goto6CLabel,         (0x08002F24+1)
SET_ABS_FUNC Goto6CCode,          (0x08002F5C+1)
SET_ABS_FUNC Set6CMark,           (0x08002F64+1)
SET_ABS_FUNC Set6CDestructor,     (0x08002F6C+1)
SET_ABS_FUNC ForAll6C,            (0x08002F70+1)
SET_ABS_FUNC ForEach6C,           (0x08002F98+1)
SET_ABS_FUNC BlockEach6CMarked,   (0x08002FEC+1)
SET_ABS_FUNC UnblockEach6CMarked, (0x08003014+1)
SET_ABS_FUNC DeleteEach6CMarked,  (0x08003040+1)
SET_ABS_FUNC DeleteEach6C,        (0x08003078+1)
SET_ABS_FUNC BreakEach6CLoop,     (0x08003094+1)
SET_ABS_FUNC Set6CLoop,           (0x08003450+1)

@ </6C/Proc System>

@ <Game Lock>

SET_ABS_FUNC LockGameLogic,    (0x08015360+1)
SET_ABS_FUNC UnlockGameLogic,  (0x08015370+1)
SET_ABS_FUNC GetGameLogicLock, (0x08015380+1)

@ </Game Lock>

SET_ABS_FUNC CanUnitCrossTerrain, (0x0801949C+1)

@ <Maps>

SET_ABS_FUNC FlushTileUpdates,        (0x08019A64+1)
SET_ABS_FUNC UpdateGameTilesGraphics, (0x08019C3C+1)
SET_ABS_FUNC PrepareMapChangeGfx,     (0x08019CBC+1)
SET_ABS_FUNC RefreshEntityMaps,       (0x0801A1F4+1)

@ </Maps>

@ <BMXFADE>

SET_ABS_FUNC NewBMXFADE,         (0x0801DDC4+1)
SET_ABS_FUNC NewBlockingBMXFADE, (0x0801DDF0+1)

SET_ABS_FUNC BMXFADEExists,      (0x0801DE18+1)

@ </BMXFADE>

@ <SMS>

SET_ABS_FUNC HideUnitSMS, (0x0802810C+1)
SET_ABS_FUNC ShowUnitSMS, (0x08028130+1)

@ </SMS>

@ <Traps>

SET_ABS_FUNC GetTrapAt,           (0x0802E1F0+1)
SET_ABS_FUNC GetSpecificTrapAt,   (0x0802E24C+1)

SET_ABS_FUNC AddTrap,             (0x0802E2B8+1)
SET_ABS_FUNC AddTrapExt,          (0x0802E2E0+1)
SET_ABS_FUNC RemoveTrap,          (0x0802E2FC+1)
SET_ABS_FUNC AddFireTrap,         (0x0802E314+1)
SET_ABS_FUNC AddGasTrap,          (0x0802E330+1)
SET_ABS_FUNC AddArrowTrap,        (0x0802E350+1)
SET_ABS_FUNC AddTrap8,            (0x0802E388+1)
SET_ABS_FUNC AddTrap9,            (0x0802E398+1)

SET_ABS_FUNC ApplyTrapMapChanges, (0x0802E430+1)

SET_ABS_FUNC ApplyMapChangesById, (0x0802E58C+1)
SET_ABS_FUNC AddMapChange,        (0x0802E5F8+1)
SET_ABS_FUNC RemoveMapChange,     (0x0802E60C+1)

SET_ABS_FUNC AddGorgonEggTrap,    (0x08037928+1)
SET_ABS_FUNC AddBallista,         (0x08037A04+1)

@ </Traps>

SET_ABS_FUNC GetStatIncrease, (0x0802B9A0+1)

@ <Roof>

SET_ABS_FUNC HideIfUnderRoof,      (0x0802E660+1)
SET_ABS_FUNC UpdateUnitsUnderRoof, (0x0802E690+1)

@ </Roof>

@ <MoveUnit>

SET_ABS_FUNC ResetAllMoveUnitExtraData,      (0x0807840C+1)
SET_ABS_FUNC NewMoveUnitForUnit_Specialized, (0x08078428+1)
SET_ABS_FUNC NewMoveUnitForUnit,             (0x08078464+1)
SET_ABS_FUNC EnableMoveUnitCameraFollow,     (0x080784E4+1)
SET_ABS_FUNC DisableMoveUnitCameraFollow,    (0x080784EC+1)
SET_ABS_FUNC NewMoveUnitForUI,               (0x080784F4+1)
SET_ABS_FUNC NewMoveUnit,                    (0x08078540+1)
SET_ABS_FUNC SetMoveUnitDirection,           (0x08078694+1)
SET_ABS_FUNC ResetMoveUnitDirection,         (0x080786BC+1)
SET_ABS_FUNC ResetMoveUnitDirection_Unique,  (0x080786E8+1)
SET_ABS_FUNC SetMoveUnitMoveManual_Unique,   (0x08078700+1)
SET_ABS_FUNC DoesMoveUnitExist,              (0x08078720+1)
SET_ABS_FUNC DoesMovingMoveUnitExist,        (0x08078738+1)
SET_ABS_FUNC SetMoveUnitMoveManual,          (0x08078790+1)
SET_ABS_FUNC EndAllMoveUnits,                (0x080790A4+1)
SET_ABS_FUNC EndMoveUnit,                    (0x080790B4+1)

SET_ABS_FUNC SetMoveUnitDisplayPosition,     (0x080797E4+1)

@ </MoveUnit>

@ <C Standard Library>

SET_ABS_FUNC div,             (0x080D1AD4+1)
SET_ABS_FUNC mod,             (0x080D1B4C+1)

SET_ABS_FUNC memcpy,          (0x080D1C0C+1)
SET_ABS_FUNC memset,          (0x080D1C6C+1)

@ </C Standard Library>
