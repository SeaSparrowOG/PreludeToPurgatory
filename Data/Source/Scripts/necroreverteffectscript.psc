Scriptname NecroRevertEffectSCRIPT extends ActiveMagicEffect  

Race Property PlayerOriginalRace Auto
;Race Property BretonRace Auto
Race Property LichRace Auto

FormList Property CrimeFactions auto

Quest Property NecroLichRitualQuest auto
Quest Property NecroTrackingQuest auto

Faction Property HunterFaction  Auto  
Faction Property LichFaction auto
Faction Property DLC1PlayerVampireLordFaction Auto

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
Armor Property NecroLichSkinUnderking auto
Armor Property NecroLichSkinWhiteFlesh auto
Armor Property NecroLichSkinWhiteSkel auto
Armor Property NecroLichSkinWhiteSkel_Elf auto
Armor Property NecroLichSkinWhiteSkel_Orc auto
Armor Property NecroLichSkinWhiteSkel_Argonian auto
Armor Property NecroLichSkinWhiteSkel_Khajiit auto
Armor Property NecroLichSkinRedSkel_Elf auto
Armor Property NecroLichSkinRedSkel_Orc auto
Armor Property NecroLichSkinRedSkel_Argonian auto
Armor Property NecroLichSkinRedSkel_Khajiit auto
Armor Property NecroLichSkinGreenSkel_Elf auto
Armor Property NecroLichSkinGreenSkel_Orc auto
Armor Property NecroLichSkinGreenSkel_Argonian auto
Armor Property NecroLichSkinGreenSkel_Khajiit auto
Armor Property NecroLichSkinBlackSkel_Elf auto
Armor Property NecroLichSkinBlackSkel_Orc auto
Armor Property NecroLichSkinBlackSkel_Argonian auto
Armor Property NecroLichSkinBlackSkel_Khajiit auto
Armor Property NecroLichSkinWhiteFlesh_Elf auto
Armor Property NecroLichSkinWhiteFlesh_Orc auto
Armor Property NecroLichSkinWhiteFlesh_Argonian auto
Armor Property NecroLichSkinWhiteFlesh_Khajiit auto
Armor Property NecroLichSkinRedFlesh_Elf auto
Armor Property NecroLichSkinRedFlesh_Orc auto
Armor Property NecroLichSkinRedFlesh_Argonian auto
Armor Property NecroLichSkinRedFlesh_Khajiit auto
Armor Property NecroLichSkinGreenFlesh_Elf auto
Armor Property NecroLichSkinGreenFlesh_Orc auto
Armor Property NecroLichSkinGreenFlesh_Argonian auto
Armor Property NecroLichSkinGreenFlesh_Khajiit auto
Armor Property NecroLichSkinBlackFlesh_Elf auto
Armor Property NecroLichSkinBlackFlesh_Orc auto
Armor Property NecroLichSkinBlackFlesh_Argonian auto
Armor Property NecroLichSkinBlackFlesh_Khajiit auto

float PlayerHP
float PlayerMG
float PlayerST

Int Property HP  Auto
Int Property Magicka  Auto  
Int Property Stamina  Auto 

Explosion Property TransformFXExplosion auto
Explosion Property TransformFXParticles auto
Explosion Property FXDragonPriestLargeExplosion auto

VisualEffect Property DragonPriestParticlesFX Auto
{Floor based stream of upward particles}

VisualEffect Property DragonPriestEyeGlowFX Auto
{eye glow fx}

VisualEffect Property FXfallingSandDragonPriestEffect Auto
{falling sand from holes fx}

Bool Property Transforming auto
Perk Property LightFoot  Auto  

Spell property DLC1FXCastVampireBleedSpell Auto

Spell Property AbLichNecroPassive auto
Spell Property DragonPriestAbility auto
Spell Property LichAbility auto
Spell Property LichTransform auto
Spell Property NecroRevert auto
Spell Property AbLichNecroCooldown auto
Spell Property NecroDeathGrip auto


Spell Property NecroUClDarkConduit Auto

EffectShader Property NecroLichTransformPlayerFireFXShort auto

MagicEffect Property NecroRevertEffect auto
MagicEffect Property LichTransformEffect auto

actor TheLich

Event OnEffectStart(Actor akTarget, Actor akCaster)

TheLich = akCaster

if (TheLich.GetRace() == LichRace && Transforming == false)
	Debug.Trace("LICH: Casting transformation spell on " + akTarget)

	; set up tracking
	if (akTarget == TheLich)
		;DLC1FXCastVampireBleedSpell.Cast(TheLich, TheLich)
		NecroLichTransformPlayerFireFXShort.Play(TheLich,0.5)
		TheLich.placeAtMe(TransformFXExplosion, 1)
		TheLich.placeAtMe(TransformFXParticles, 1)
		TheLich.placeAtMe(FXDragonPriestLargeExplosion, 1)
		TheLich.addspell(AbLichNecroPassive, false)
		PlayerHP = TheLich.GetBaseActorValue("health")
		PlayerMG = TheLich.GetBaseActorValue("magicka")
		PlayerST = TheLich.GetBaseActorValue("stamina")
		Transforming = true
		Revert()
	endif
elseif (TheLich.GetRace() != LichRace)
	NecroTransformationAlreadyRevertedMsg.Show()
endif

EndEvent

Function Revert()

	TheLich.SetGhost(true)
;     Debug.Trace("LICH: Revert effect detected")
;	TheLich.RemoveFromFaction(LichFaction)
	
	(NecroTrackingQuest as NecroTrackingQuestSCRIPT).LichfootCheck()
	
    if (NecroTrackingQuest as NecroTrackingQuestSCRIPT).DLC1HasLightfoot == false
        TheLich.RemovePerk(Lightfoot)
    endif

    ; make sure your health is reasonable before turning you back
    float currHealth = TheLich.GetAV("health")
    if (currHealth <= 70)
        Debug.Trace("VAMPIRE: Player's health is only " + currHealth + "; restoring.")
        TheLich.RestoreAV("health", 70 - currHealth)
    endif

    	; change you back
    	;TheLich.RemoveItem(NecroLichForm, 2, true)


	if NecroLichModelValue.GetValueInt() == 3
		;give player dragon priest skin
		TheLich.RemoveItem(NecroLichSkinDragonPriest,  1, True)				
	elseif NecroLichModelValue.GetValueInt() == 4
		;give player ayleid skin
		TheLich.RemoveItem(NecroLichSkinAyleid,  1, True)			
	elseif NecroLichModelValue.GetValueInt() == 5
		;give player underking skin
		TheLich.RemoveItem(NecroLichSkinUnderking,  1, True)					
	elseif NecroLichModelValue.GetValueInt() == 1
	; embalmed essence	
		;Hubris Of Man
		if NecroLichRaceValue.GetValueInt() == 0
			if NecroLichColorValue.GetValueInt() == 1
				;transform the player into the red flesh lich
				TheLich.RemoveItem(NecroLichSkin01,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 2
				;transform the player into the green flesh lich
				TheLich.RemoveItem(NecroLichSkin02,  1, True)	
			elseif NecroLichColorValue.GetValueInt() == 3
				;transform the player into the black flesh lich
				TheLich.RemoveItem(NecroLichSkin09,  1, True)	
			elseif NecroLichColorValue.GetValueInt() == 4		
				;transform the player into the white flesh lich
				TheLich.RemoveItem(NecroLichSkinWhiteFlesh,  1, True)
			else 
				;lich has no color, likely ethereal. equip black
				TheLich.RemoveItem(NecroLichSkin09,  1, True)	
			endif			
		;Elven Worm
		elseif NecroLichRaceValue.GetValueInt() == 1
			if NecroLichColorValue.GetValueInt() == 1
				;transform the player into the red flesh lich (elf)
				TheLich.RemoveItem(NecroLichSkinRedFlesh_Elf,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 2
				;transform the player into the green flesh lich (elf)
				TheLich.RemoveItem(NecroLichSkinGreenFlesh_Elf,  1, True)	
			elseif NecroLichColorValue.GetValueInt() == 3
				;transform the player into the black flesh lich (elf)
				TheLich.RemoveItem(NecroLichSkinBlackFlesh_Elf,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 4
				;transform the player into the white flesh lich (elf)
				TheLich.RemoveItem(NecroLichSkinWhiteFlesh_Elf,  1, True)		
			else 
				;player has no color, likely ethereal. pick black
				TheLich.RemoveItem(NecroLichSkinBlackFlesh_Elf,  1, True)
			endif		
		;Orc Warlock
		elseif NecroLichRaceValue.GetValueInt() == 2
			if NecroLichColorValue.GetValueInt() == 1
				;transform the player into the red flesh lich (orc)
				TheLich.RemoveItem(NecroLichSkinRedFlesh_Orc,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 2
				;transform the player into the green flesh lich (orc)
				TheLich.RemoveItem(NecroLichSkinGreenFlesh_Orc,  1, True)	
			elseif NecroLichColorValue.GetValueInt() == 3
				;transform the player into the black flesh lich (orc)
				TheLich.RemoveItem(NecroLichSkinBlackFlesh_Orc,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 4
				;transform the player into the white flesh lich (orc)
				TheLich.RemoveItem(NecroLichSkinWhiteFlesh_Orc,  1, True)	
			else 
				;player has no color, likely ethereal. pick black
				TheLich.RemoveItem(NecroLichSkinBlackFlesh_Orc,  1, True)				
			endif		
		;Reptilian Fossil	
		elseif NecroLichRaceValue.GetValueInt() == 3
			if NecroLichColorValue.GetValueInt() == 1
				;transform the player into the red flesh lich (lizard)
				TheLich.RemoveItem(NecroLichSkinRedFlesh_Argonian,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 2
				;transform the player into the green flesh lich (lizard)
				TheLich.RemoveItem(NecroLichSkinGreenFlesh_Argonian,  1, True)	
			elseif NecroLichColorValue.GetValueInt() == 3
				;transform the player into the black flesh lich (lizard)
				TheLich.RemoveItem(NecroLichSkinBlackFlesh_Argonian,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 4
				;transform the player into the white flesh lich (lizard)
				TheLich.RemoveItem(NecroLichSkinWhiteFlesh_Argonian,  1, True)	
			else 
				;player has no color, likely ethereal. pick black
				TheLich.RemoveItem(NecroLichSkinBlackFlesh_Argonian,  1, True)						
			endif	
		;Arum'Khal's Empire
		elseif NecroLichRaceValue.GetValueInt() == 4
			if NecroLichColorValue.GetValueInt() == 1
				;transform the player into the red flesh lich (cat)
				TheLich.RemoveItem(NecroLichSkinRedFlesh_Khajiit,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 2
				;transform the player into the green flesh lich (cat)
				TheLich.RemoveItem(NecroLichSkinGreenFlesh_Khajiit,  1, True)	
			elseif NecroLichColorValue.GetValueInt() == 3
				;transform the player into the black flesh lich (cat)
				TheLich.RemoveItem(NecroLichSkinBlackFlesh_Khajiit,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 4
				;transform the player into the white flesh lich (cat)
				TheLich.RemoveItem(NecroLichSkinWhiteFlesh_Khajiit,  1, True)	
			else 
				;player has no color, likely ethereal. pick black
				TheLich.RemoveItem(NecroLichSkinBlackFlesh_Khajiit,  1, True)				
			endif	
		endif
			
	elseif NecroLichModelValue.GetValueInt() == 2
	; corroded bone 
	
		;Hubris Of Man
		if NecroLichRaceValue.GetValueInt() == 0
			if NecroLichColorValue.GetValueInt() == 1
				;transform the player into the red skeletal lich
				TheLich.RemoveItem(NecroLichSkin05,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 2
				;transform the player into the green skeletal lich
				TheLich.RemoveItem(NecroLichSkin06,  1, True)	
			elseif NecroLichColorValue.GetValueInt() == 3
				;transform the player into the black skeletal lich
				TheLich.RemoveItem(NecroLichSkin11,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 4
				;transform the player into the white skeletal lich
				TheLich.RemoveItem(NecroLichSkinWhiteSkel,  1, True)	
			else 
				;player has no color, likely ethereal. pick black
				TheLich.RemoveItem(NecroLichSkin11,  1, True)
			endif
		;Elven Worm
		elseif NecroLichRaceValue.GetValueInt() == 1
			if NecroLichColorValue.GetValueInt() == 1
				;transform the player into the red skeletal lich (elf)
				TheLich.RemoveItem(NecroLichSkinRedSkel_Elf,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 2
				;transform the player into the green skeletal lich (elf)
				TheLich.RemoveItem(NecroLichSkinGreenSkel_Elf,  1, True)	
			elseif NecroLichColorValue.GetValueInt() == 3
				;transform the player into the black skeletal lich (elf)
				TheLich.RemoveItem(NecroLichSkinBlackSkel_Elf,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 4
				;transform the player into the white skeletal lich (elf)
				TheLich.RemoveItem(NecroLichSkinWhiteSkel_Elf,  1, True)		
			else 
				;player has no color, likely ethereal. pick black
				TheLich.RemoveItem(NecroLichSkinBlackSkel_Elf,  1, True)
			endif		
		;Orc Warlock
		elseif NecroLichRaceValue.GetValueInt() == 2
			if NecroLichColorValue.GetValueInt() == 1
				;transform the player into the red skeletal lich (orc)
				TheLich.RemoveItem(NecroLichSkinRedSkel_Orc,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 2
				;transform the player into the green skeletal lich (orc)
				TheLich.RemoveItem(NecroLichSkinGreenSkel_Orc,  1, True)	
			elseif NecroLichColorValue.GetValueInt() == 3
				;transform the player into the black skeletal lich (orc)
				TheLich.RemoveItem(NecroLichSkinBlackSkel_Orc,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 4
				;transform the player into the white skeletal lich (orc)
				TheLich.RemoveItem(NecroLichSkinWhiteSkel_Orc,  1, True)	
			else 
				;player has no color, likely ethereal. pick black
				TheLich.RemoveItem(NecroLichSkinBlackSkel_Orc,  1, True)				
			endif		
		;Reptilian Fossil	
		elseif NecroLichRaceValue.GetValueInt() == 3
			if NecroLichColorValue.GetValueInt() == 1
				;transform the player into the red skeletal lich (lizard)
				TheLich.RemoveItem(NecroLichSkinRedSkel_Argonian,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 2
				;transform the player into the green skeletal lich (lizard)
				TheLich.RemoveItem(NecroLichSkinGreenSkel_Argonian,  1, True)	
			elseif NecroLichColorValue.GetValueInt() == 3
				;transform the player into the black skeletal lich (lizard)
				TheLich.RemoveItem(NecroLichSkinBlackSkel_Argonian,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 4
				;transform the player into the white skeletal lich (lizard)
				TheLich.RemoveItem(NecroLichSkinWhiteSkel_Argonian,  1, True)	
			else 
				;player has no color, likely ethereal. pick black
				TheLich.RemoveItem(NecroLichSkinBlackSkel_Argonian,  1, True)						
			endif	
		;Arum'Khal's Empire
		elseif NecroLichRaceValue.GetValueInt() == 4
			if NecroLichColorValue.GetValueInt() == 1
				;transform the player into the red skeletal lich (cat)
				TheLich.RemoveItem(NecroLichSkinRedSkel_Khajiit,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 2
				;transform the player into the green skeletal lich (cat)
				TheLich.RemoveItem(NecroLichSkinGreenSkel_Khajiit,  1, True)	
			elseif NecroLichColorValue.GetValueInt() == 3
				;transform the player into the black skeletal lich (cat)
				TheLich.RemoveItem(NecroLichSkinBlackSkel_Khajiit,  1, True)
			elseif NecroLichColorValue.GetValueInt() == 4
				;transform the player into the white skeletal lich (cat)
				TheLich.RemoveItem(NecroLichSkinWhiteSkel_Khajiit,  1, True)	
			else 
				;player has no color, likely ethereal. pick black
				TheLich.RemoveItem(NecroLichSkinBlackSkel_Khajiit,  1, True)				
			endif	
		endif
	else
		;lich has no skin, likely due to adding transform by console. Give naked skin until player changes skin at phylactery.
		TheLich.RemoveItem(NecroLichSkin11,  1, True)
	endif
		
	if NecroLichModelValue.GetValueInt() == 3
		;give player dragon priest skin
		TheLich.RemoveItem(NecroLichSkinDragonPriest,  1, True)	
	elseif NecroLichModelValue.GetValueInt() == 4
		;give player ayleid skin
		TheLich.RemoveItem(NecroLichSkinAyleid,  1, True)	
	elseif NecroLichColorValue.GetValueInt() == 1 && NecroLichModelValue.GetValueInt() == 1
		;transform the player into the red flesh lich
		TheLich.RemoveItem(NecroLichSkin01,  1, True)		
	elseif NecroLichColorValue.GetValueInt() == 1 && NecroLichModelValue.GetValueInt() == 2
		;transform the player into the red skeletal lich
		TheLich.RemoveItem(NecroLichSkin05,  1, True)	
	elseif NecroLichColorValue.GetValueInt() == 2 && NecroLichModelValue.GetValueInt() == 1
		;transform the player into the green flesh lich
		TheLich.RemoveItem(NecroLichSkin02,  1, True)
	elseif NecroLichColorValue.GetValueInt() == 2 && NecroLichModelValue.GetValueInt() == 2
		;transform the player into the green skeletal lich
		TheLich.RemoveItem(NecroLichSkin06,  1, True)	
	elseif NecroLichColorValue.GetValueInt() == 3 && NecroLichModelValue.GetValueInt() == 1
		;transform the player into the black flesh lich
		TheLich.RemoveItem(NecroLichSkin09,  1, True)	
	elseif NecroLichColorValue.GetValueInt() == 3 && NecroLichModelValue.GetValueInt() == 2
		;transform the player into the black skeletal lich
		TheLich.RemoveItem(NecroLichSkin11,  1, True)
	elseif NecroLichColorValue.GetValueInt() == 4 && NecroLichModelValue.GetValueInt() == 1
		;transform the player into the white flesh lich
		TheLich.RemoveItem(NecroLichSkinWhiteFlesh,  1, True)	
	elseif NecroLichColorValue.GetValueInt() == 4 && NecroLichModelValue.GetValueInt() == 2
		;transform the player into the white skeletal lich
		TheLich.RemoveItem(NecroLichSkinWhiteSkel,  1, True)				
	elseif NecroLichModelValue.GetValueInt() == 1
		TheLich.RemoveItem(NecroLichSkin09,  1, True)	
	else
		TheLich.RemoveItem(NecroLichSkin11,  1, True)
	endif
	
	DragonPriestEyeGlowFX.Stop(TheLich)
	DragonPriestParticlesFX.Stop(TheLich)
	FXfallingSandDragonPriestEffect.Stop(TheLich)

   	TheLich.SetRace((NecroLichRitualQuest as NecroLichTrackingQuest).playerOriginalRace)
	TheLich.RemoveSpell(NecroUCLEtherealAbility)

	Spell right = TheLich.GetEquippedSpell(1)
	if (right == NecroDeathGrip)
        	TheLich.UnequipSpell(right, 1)
	endif
	
	TheLich.removespell(LichAbility)
	TheLich.removespell(NecroUClDarkConduit)
	TheLich.removespell(NecroDeathGrip)

	if !TheLich.HasSpell(LichTransform)
		TheLich.Addspell(LichTransform, false)
	endif
	TheLich.equipspell(LichTransform, 2)
	;SeaSparrow - One liner for our checks.
	PreludeManager.PostRevert(TheLich)	
   	Game.EnablePlayerControls(abMovement = false, abFighting = false, abCamSwitch = true, abLooking = false, abSneaking = false, abMenu = false, abActivate = false, abJournalTabs = false, aiDisablePOVType = 1)
    Game.ShowFirstPersonGeometry(true)

    TheLich.SetAttackActorOnSight(false)
    HunterFaction.SetPlayerEnemy(false)
    TheLich.RemoveFromFaction(LichFaction)

	;beth slop: DLC1PlayerVampireLordFaction has hardcoded interactions to prevent volkihar castle aggro
	;so this is needed for Guest of Volkihar to fix aggro in castle volkihar. thanks bethesda!	
	TheLich.RemoveFromFaction(DLC1PlayerVampireLordFaction)
	
; 	UCL: if no masquerade perk, add bounty to player when people witness transform
	If !TheLich.HasPerk(NecroUCLMasqueradePerk)
		Game.SendWereWolfTransformation()
	endif
	
	
	;Utility.Wait(0.5)

	TheLich.SetActorValue("health", (PlayerHP))
	;Debug.notification("LICH: Player health changed to " + PlayerHP)
	TheLich.SetActorValue("magicka", (PlayerMG))
	;Debug.notification("LICH: Player magicka changed to " + PlayerMG)
	TheLich.SetActorValue("stamina", (PlayerST))

;	TheLich.RestoreActorValue("health", 70)
;	TheLich.RestoreActorValue("stamina", 50)
;	TheLich.SetScale (1.0)

    ; give the set race event a chance to come back, otherwise shut us down
    	;Utility.Wait(5)
;    	Debug.Trace("OnRaceSwitchComplete event took more than 5 seconds, shutting down ourselves.")
;    	Shutdown()

	Transforming = false
	Game.SetBeastForm(false)
	Game.EnableFastTravel()
	TheLich.SetGhost(false)
	AbLichNecroCooldown.Cast(TheLich)
	
	;UCL: dont remove revert and transform spells so that they can stay in favourite menu
	;TheLich.removespell(NecroRevert)
	
	;this function is slow as fuck, therefore its done last and only if masquerade perk isnt active. 
	;the phylactery script will handle scenarios where player obtains the perk in lich form
	
	If !TheLich.HasPerk(NecroUCLMasqueradePerk)
        int cfIndex = 0
        while (cfIndex < CrimeFactions.GetSize())
            ;Debug.Trace("LICH: Removing enemy flag from " + CrimeFactions.GetAt(cfIndex))
            (CrimeFactions.GetAt(cfIndex) as Faction).SetPlayerEnemy(false)
            cfIndex += 1
        endwhile
	endif
	
    ;and you're now recognized
    Game.SetPlayerReportCrime(true)
		
EndFunction

Perk Property NecroUCLMasqueradePerk Auto
GlobalVariable Property NecroLichColorValue Auto
GlobalVariable Property NecroLichModelValue Auto
GlobalVariable Property NecroLichRaceValue Auto
Spell Property NecroUCLEtherealAbility Auto
Message Property NecroTransformationAlreadyRevertedMsg Auto

;SeaSparrow - New Property
PTP_QuestMaintenanceScript Property PreludeManager Auto

