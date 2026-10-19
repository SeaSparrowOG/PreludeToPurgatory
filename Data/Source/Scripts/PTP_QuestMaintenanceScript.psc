Scriptname PTP_QuestMaintenanceScript Extends Quest
{Handles maintenance and new functions for Prelude to Purgatory}

Actor Property PlayerREF Auto
GlobalVariable Property PlayerIsVampire Auto
Perk Property PTP_Entry_PRK_Transcendence Auto
Quest Property PlayerVampireQuest Auto
Quest Property C00 Auto
Quest Property NecroLichRitualQuest Auto
Race Property NecroLichRace Auto
Spell Property WerewolfChange Auto
Spell Property DLC1VampireChange Auto
Spell Property NecroDeathGrip auto
Spell Property NecroRevert auto
Spell Property NecroUClDarkConduit auto
SoulGem Property SoulGemBlackFilled Auto

GlobalVariable Property PTP_Framework_GLB_SkillLevelModifier Auto 
GlobalVariable Property PTP_Framework_GLB_PerkPoints Auto 
GlobalVariable Property PTP_Framework_GLB_SkillLevel Auto 
GlobalVariable Property PTP_Framework_GLB_SkillLevelRatio Auto 
Perk Property PTP_Necromancer_PRK_FrozenCadavers Auto
Perk Property PTP_Powers_PRK_EnemyDefied Auto
Perk Property PTP_Powers_PRK_Wraithform Auto
Perk Property PTP_Powers_PRK_RiteOfTheDragolich Auto
Perk Property PTP_Powers_PRK_NecroticRejuvination Auto
Perk Property PTP_Entry_PRK_HardenedWeave Auto
Perk Property PTP_Entry_PRK_MirrorWeave Auto
Quest Property PTP_Powers_QST_DragolichFollowerQuest Auto
Message Property PTP_MSG_MissingCustomSkillsFramework Auto
Message Property PTP_MSG_IncompatibleUpdate Auto
Spell Property PTP_Necromancer_SPL_abFrozenCadavers Auto
Spell Property PTP_Powers_SPL_ToggleLichSight Auto
Spell Property PTP_Powers_SPL_WraithForm Auto
Spell Property PTP_Powers_SPL_RiteOfTheDragolich Auto
Spell Property PTP_Entry_SPL_abHardenedWeave Auto
Spell Property PTP_Entry_SPL_abMirrorWeave Auto

necrolichtrackingquest Property LichTrackingQuest Auto
Int iLevel
Float fExperienceToNextLevel
Float fCurrentExperience

Int[] iVersion 

Int Function CheckVersion()

    If iVersion[0] < 2

        Return -1
    EndIf

    If iVersion[1] < 0

        Return 1
    EndIf

    Return 0
EndFunction

Function PlayerLoad()

    Int iResponse = CheckVersion() 
    If (iResponse == -1)

        PTP_MSG_IncompatibleUpdate.Show()
        Return
    ElseIf (iResponse == 1)

        Self.Stop()
    EndIf
EndFunction

Float Function GetExperienceToNextLevel()

    Return Math.Sqrt(iLevel) * 0.37 + 0.1
EndFunction

Function CalculateExperience(Float fExperience)

    If (fExperience <= 0.0 || iLevel >= 14)

        Return
    EndIf
    
    fCurrentExperience += fExperience * PTP_Framework_GLB_SkillLevelModifier.GetValue()
    If (fCurrentExperience < fExperienceToNextLevel)

        Float fRatio = fCurrentExperience / fExperienceToNextLevel
        PTP_Framework_GLB_SkillLevelRatio.SetValue(fRatio)
        Return
    EndIf

    iLevel += 1
    fCurrentExperience -= fExperienceToNextLevel
    Float fExcess = fCurrentExperience
    fExperienceToNextLevel = GetExperienceToNextLevel()
    PTP_Framework_GLB_PerkPoints.Mod(1.0)

    While fExcess >= fExperienceToNextLevel && iLevel <= 12

        iLevel += 1
        fExcess -= fExperienceToNextLevel
        fCurrentExperience -= fExperienceToNextLevel
        fExperienceToNextLevel = GetExperienceToNextLevel()
        PTP_Framework_GLB_PerkPoints.Mod(1.0)
    EndWhile

    Float fRatio = fCurrentExperience / fExperienceToNextLevel
    PTP_Framework_GLB_SkillLevelRatio.SetValue(fRatio)
    PTP_Framework_GLB_SkillLevel.SetValueInt(iLevel)
    CustomSkills.ShowSkillIncreaseMessage("Lichdom", iLevel)
EndFunction

;/
The following functions are called externally. This is done to 
avoid congesting the original scripts with new functions so that
the eventual Ascension patch is easier. 
/;

;Function for adding the necessary spells to the Lich on transformation
;and ensuring that million faces works.
Bool Function PreTransform(Actor TheLich)

    If (!LichTrackingQuest.PlayerOriginalRace || TheLich.HasPerk(PTP_Powers_PRK_NecroticRejuvination))

        LichTrackingQuest.PlayerOriginalRace = TheLich.GetRace()
    EndIf

    If (TheLich.HasPerk(PTP_Necromancer_PRK_FrozenCadavers))

        TheLich.AddSpell(PTP_Necromancer_SPL_abFrozenCadavers)
    EndIf

    If (TheLich.HasPerk(PTP_Powers_PRK_EnemyDefied))

        TheLich.AddSpell(PTP_Powers_SPL_ToggleLichSight)
    EndIf

    If (TheLich.HasPerk(PTP_Powers_PRK_WraithForm))

        TheLich.AddSpell(PTP_Powers_SPL_Wraithform)
    EndIf

    If (TheLich.HasPerk(PTP_Powers_PRK_RiteOfTheDragolich))

        TheLich.AddSpell(PTP_Powers_SPL_RiteOfTheDragolich)
    EndIf
    
    If (TheLich.HasPerk(PTP_Entry_PRK_HardenedWeave))

        TheLich.AddSpell(PTP_Entry_SPL_abHardenedWeave)
    EndIf
    
    If (TheLich.HasPerk(PTP_Entry_PRK_MirrorWeave))

        TheLich.AddSpell(PTP_Entry_SPL_abMirrorWeave)
    EndIf
    Return True
EndFunction

;Revert removes the spells added by PreTransform. 
Bool Function PostRevert(Actor TheLich)

    If (PTP_Powers_QST_DragolichFollowerQuest.IsRunning())

        PTP_Powers_QST_DragolichFollowerQuest.Stop()
    EndIf

        TheLich.RemoveSpell(PTP_Necromancer_SPL_abFrozenCadavers)
        TheLich.RemoveSpell(PTP_Powers_SPL_ToggleLichSight)
        TheLich.RemoveSpell(PTP_Powers_SPL_Wraithform)
        TheLich.RemoveSpell(PTP_Powers_SPL_RiteOfTheDragolich)
        TheLich.RemoveSpell(PTP_Entry_SPL_abMirrorWeave)
        TheLich.RemoveSpell(PTP_Entry_SPL_abHardenedWeave)
    Return True
EndFunction

;/
Events
/;

Event OnKeyDown(Int a_iKeyCode)

    If (PlayerREF.GetRace() == NecroLichRace && a_iKeyCode == 15 && !Utility.IsInMenuMode())

        CustomSkills.OpenCustomSkillMenu("Lichdom")
    EndIf
EndEvent

Event OnInit()

    If !(CustomSkills.GetAPIVersion() > 0)

        PTP_MSG_MissingCustomSkillsFramework.Show()
        Return
    EndIf

    iVersion = New Int[2]
    iVersion[0] = 2
    iVersion[1] = 0

    iLevel = PTP_Framework_GLB_SkillLevel.GetValue() As Int
    fExperienceToNextLevel = GetExperienceToNextLevel() 
    fCurrentExperience = PTP_Framework_GLB_SkillLevelRatio.GetValue() * fExperienceToNextLevel

    RegisterForKey(15)
EndEvent