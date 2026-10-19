Scriptname NecroLichTransformSCRIPT extends ActiveMagicEffect  

Message Property NecroLeftMagicSelectMessage auto

Quest Property NecroLichRitualQuest auto
Quest Property NecroTrackingQuest auto
Quest Property PlayerVampireQuest auto
Quest Property C00 auto

GlobalVariable Property pDLC1VQ04SafeToEnter Auto

Race Property PlayerRace Auto
Race Property LichRace Auto

FormList Property CrimeFactions auto

Quest Property LichTrackingQuest auto

Faction Property HunterFaction  Auto  
Faction Property LichFaction auto

Ammo Property NecroLichClearArrow auto

Armor Property NecroLichSkin01 auto
Armor Property NecroLichSkin02 auto
Armor Property NecroLichSkin03 auto
Armor Property NecroLichSkin04 auto
Armor Property NecroLichSkin05 auto
Armor Property NecroLichSkin06 auto
Armor Property NecroLichSkin07 auto
Armor Property NecroLichSkin08 auto
Armor Property NecroLichSkin09 auto
Armor Property NecroLichSkin10 auto
Armor Property NecroLichSkin11 auto
Armor Property NecroLichSkinDragonPriest auto
Armor Property NecroLichSkinAyleid auto
Armor Property NecroLichSkinWhiteFlesh auto
Armor Property NecroLichSkinWhiteSkel auto

float PlayerHP
float PlayerMG
float PlayerST

;Float Property Health  Auto
;Float Property Magicka  Auto  
;Float Property Stamina Auto 

Bool Property Transforming auto
Perk Property LightFoot  Auto  

Explosion Property TransformFXExplosion auto
Explosion Property TransformFXParticles auto
Explosion property FXDragonPriestLargeExplosion auto
Sound Property TransformSoundFX auto
Sound Property NPCNecroLichCloak auto
Sound Property MAGFail auto

MagicEffect Property AbLichNecroCooldown auto

Spell Property NecroDeathGrip auto
Spell Property AbLichNecroPassive auto
Spell Property LichAbility auto
Spell Property LichTransform auto
Spell Property NecroRevert auto
Spell Property WerewolfChange Auto
Spell Property DLC1VampireChange Auto

ImageSpaceModifier property IllusionLightMassiveImod auto

MagicEffect Property NecroRevertEffect auto
MagicEffect Property LichTransformEffect auto

EffectShader Property NecroLichTransformPlayerFireFXShort auto

GlobalVariable Property PlayerIsVampire Auto
actor TheLich

Event OnEffectStart(Actor AkTarget, Actor AkCaster)
TheLich = AkCaster

if (TheLich.GetRace() != LichRace)
	if (akTarget == TheLich && Transforming == false && TheLich.HasMagicEffect(AbLichNecroCooldown) == 0)
			Game.DisablePlayerControls(abMovement = false, abFighting = false, abCamSwitch = true, abMenu = false, abActivate = false, abJournalTabs = false, aiDisablePOVType = 1)
			
			IllusionLightMassiveImod.apply()                                  ; apply isMod at full strength
			NecroLichTransformPlayerFireFXShort.Play(Game.GetPlayer(),0.5)
			TheLich.placeAtMe(TransformFXExplosion, 1)
			TheLich.placeAtMe(TransformFXParticles, 1)
			TheLich.placeAtMe(FXDragonPriestLargeExplosion, 1)
			TheLich.removespell(AbLichNecroPassive)
			PlayerHP = Game.GetPlayer().GetBaseActorValue("health")
			PlayerMG = Game.GetPlayer().GetBaseActorValue("magicka")
			PlayerST = Game.GetPlayer().GetBaseActorValue("stamina")
			Transforming = true		
			NecroLichTransform()
		else
			MAGFail.Play(TheLich)
			debug.notification("Lich Transformation is not yet ready.")
			Game.SetBeastForm(false)
		endif
endif

EndEvent


Function NecroLichTransform()

    	; Preload the spells the player can equip.
    	PreloadSpells()
 

;	Classical Lichdom : These cures should only be run IF the player has lycanthropy or vampirism, not all the time!
;   SeaSparrow - Yes but actually no. We run them in our script instead.
;/
	If PlayerIsVampire.GetValue() != 0
	(PlayerVampireQuest as PlayerVampireQuestScript).VampireCure(TheLich)
	EndIf
	If TheLich.HasSpell(WerewolfChange)
	(C00 as CompanionsHousekeepingScript).CurePlayer()
    EndIf
/;
	
	; unequip weapons
	TheLich.equipitem(NecroLichClearArrow, false, true)
    TheLich.removeitem(NecroLichClearArrow, 1, true)
	Weapon wleft = TheLich.GetEquippedWeapon(true)
	Weapon wright = TheLich.GetEquippedWeapon(false)


;Check for staff. if staff is in left hand, reequip to right.

	if (TheLich.GetEquippedItemType(1) == 8)
		(NecroTrackingQuest as NecroTrackingQuestSCRIPT).Staff = wright
;		debug.notification("player has staff")

	elseif (TheLich.GetEquippedItemType(0) == 8)
		(NecroTrackingQuest as NecroTrackingQuestSCRIPT).Staff = wleft
		TheLich.UnequipItem(wleft, false, true)
;		TheLich.EquipItemEx(wleft, wleft.GetFormID(), 1)
	else
		(NecroTrackingQuest as NecroTrackingQuestSCRIPT).Staff = None
;		debug.notification("player doesnt have staff")

	endif

	;SeaSparrow - One liner, runs all our checks.
	PreludeManager.PreTransform(TheLich)
 
	TheLich.SetGhost(true)
	Game.ForceThirdPerson()
	Game.SetBeastForm(True)
;	Game.ShowFirstPersonGeometry(false)
	Game.EnableFastTravel(False)
	TheLich.SetRace(LichRace)
	TransformSoundFX.Play(TheLich)
	NPCNecroLichCloak.Play(TheLich)
	
	If NecroLichColorValue.GetValueInt() == 5
		TheLich.AddSpell(NecroUCLEtherealAbility, false)
	EndIf
	
	if NecroLichModelValue.GetValueInt() == 3
		;give player dragon priest skin
		TheLich.EquipItem(NecroLichSkinDragonPriest,  False, True)		
	elseif NecroLichModelValue.GetValueInt() == 4
		;give player ayleid skin
		TheLich.EquipItem(NecroLichSkinAyleid,  False, True)		
	elseif NecroLichColorValue.GetValueInt() == 1 && NecroLichModelValue.GetValueInt() == 1
		;transform the player into the red flesh lich
		TheLich.EquipItem(NecroLichSkin01,  False, True)		
	elseif NecroLichColorValue.GetValueInt() == 1 && NecroLichModelValue.GetValueInt() == 2
		;transform the player into the red skeletal lich
		TheLich.EquipItem(NecroLichSkin05,  False, True)		
	elseif NecroLichColorValue.GetValueInt() == 2 && NecroLichModelValue.GetValueInt() == 1
		;transform the player into the green flesh lich
		TheLich.EquipItem(NecroLichSkin02,  False, True)	
	elseif NecroLichColorValue.GetValueInt() == 2 && NecroLichModelValue.GetValueInt() == 2
		;transform the player into the green skeletal lich
		TheLich.EquipItem(NecroLichSkin06,  False, True)		
	elseif NecroLichColorValue.GetValueInt() == 3 && NecroLichModelValue.GetValueInt() == 1
		;transform the player into the black flesh lich
		TheLich.EquipItem(NecroLichSkin09,  False, True)		
	elseif NecroLichColorValue.GetValueInt() == 3 && NecroLichModelValue.GetValueInt() == 2
		;transform the player into the black skeletal lich
		TheLich.EquipItem(NecroLichSkin11,  False, True)		
	elseif NecroLichColorValue.GetValueInt() == 4 && NecroLichModelValue.GetValueInt() == 1
		;transform the player into the white flesh lich
		TheLich.EquipItem(NecroLichSkinWhiteFlesh,  False, True)		
	elseif NecroLichColorValue.GetValueInt() == 4 && NecroLichModelValue.GetValueInt() == 2
		;transform the player into the white skeletal lich
		TheLich.EquipItem(NecroLichSkinWhiteSkel,  False, True)			
	elseif NecroLichModelValue.GetValueInt() == 1
		TheLich.EquipItem(NecroLichSkin09,  False, True)	
	else
		TheLich.EquipItem(NecroLichSkin11,  False, True)
	endif	
	
	
	TheLich.SetActorValue("health", (PlayerHP))
	;Debug.notification("LICH: Player health changed to" + PlayerHP)
	TheLich.SetActorValue("magicka", (PlayerMG))
	;Debug.notification("LICH: Player magicka changed to" + PlayerMG)
	TheLich.SetActorValue("stamina", (PlayerST))
	TheLich.RestoreActorValue("magicka", 100)
	;SeaSparrow - Original value of -999999999999 breaks the compiler
	TheLich.RestoreActorValue("stamina", -9999)
	TheLich.RestoreActorValue("health", 10)
	;TheLich.SetActorValue("healrate", 1)
   
	 ; set up perks/abilities
	 
    TheLich.SetActorValue("GrabActorOffset", 70)
	(NecroTrackingQuest as NecroTrackingQuestSCRIPT).LichfootCheck()
	if (NecroTrackingQuest as NecroTrackingQuestSCRIPT).DLC1HasLightfoot == false
		TheLich.AddPerk(Lightfoot)
	endif
	
	
	TheLich.addspell(NecroRevert, false)
	TheLich.equipspell(NecroRevert, 2)
	
	TheLich.addspell(NecroUClDarkConduit, false)
	TheLich.addspell(NecroDeathGrip, false)
	TheLich.equipspell(NecroUCLIceCoffin, 0)
	TheLich.equipspell(NecroDeathGrip, 1)


	If ((NecroTrackingQuest as NecroTrackingQuestSCRIPT).Staff == wright) ; Give the player their right hand staff back, if there was one
		;debug.notification("equipping players staff")
		TheLich.EquipItem(wright, false, True) ;equip the players staff they had in their right hand from earlier
	    TheLich.DrawWeapon()
		Debug.SendAnimationEvent(TheLich, "Magic_Equip")
	elseif ((NecroTrackingQuest as NecroTrackingQuestSCRIPT).Staff == wleft) ; Give the player left hand staff on right hand
		TheLich.EquipItemEx(wleft, wleft.GetFormID(), 1)
	    TheLich.DrawWeapon()
		Debug.SendAnimationEvent(TheLich, "Magic_Equip")		
	elseif ((NecroTrackingQuest as NecroTrackingQuestSCRIPT).Staff == None) ; If no staff was stored earlier, give the player the default right hand spell
		;debug.notification("not equipping staff")
		TheLich.equipspell(NecroDeathGrip, 1)
		TheLich.DrawWeapon()
		Debug.SendAnimationEvent(TheLich, "Magic_Equip")
	endif

   	; make everyone hate you and alert anyone nearby that they should now know the player is a lich
	; UCL: lich form is not hated once u get the masquerade perk
	
	If !TheLich.HasPerk(NecroUCLMasqueradePerk)
		TheLich.SetAttackActorOnSight(true)
		Game.SendWereWolfTransformation()
		TheLich.AddToFaction(LichFaction)
		HunterFaction.SetPlayerEnemy()
		Game.SetPlayerReportCrime(false)
		
		 int cfIndex = 0
			while (cfIndex < CrimeFactions.GetSize())
				Debug.Trace("VAMPIRE: Setting enemy flag on " + CrimeFactions.GetAt(cfIndex))
				(CrimeFactions.GetAt(cfIndex) as Faction).SetPlayerEnemy()
				cfIndex += 1
			endwhile	
	endif

		
	pDLC1VQ04SafeToEnter.SetValue(1)
	
	TheLich.SetGhost(false)
	TheLich.removespell(LichTransform)

EndFunction

; Preload all of the spells that could be equipped to the left or right hand. This will
; prevent a delay after EquipSpell before the casting art is loaded.
; This should be called when we start the script.
Function PreloadSpells()
	NecroUCLIceCoffin.Preload()
EndFunction

; Unload all of the spells the player could equip.
; This should be called when we shut the script down.
Function UnloadSpells()
	NecroUCLIceCoffin.Unload()
EndFunction

Spell Property NecroUCLIceCoffin Auto
Spell Property NecroUClDarkConduit Auto
Spell Property NecroUCLEtherealAbility Auto

Perk Property NecroUCLMasqueradePerk Auto

GlobalVariable Property NecroLichColorValue Auto
GlobalVariable Property NecroLichModelValue Auto

;SeaSparrow - New Property
PTP_QuestMaintenanceScript Property PreludeManager Auto