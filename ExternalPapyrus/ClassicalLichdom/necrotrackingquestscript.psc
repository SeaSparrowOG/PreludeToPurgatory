Scriptname NecroTrackingQuestSCRIPT extends Quest  

int property GameDay auto
int property CurrentDay auto
int property Daytimer auto
bool property Halo auto

Event OnInit()

	;sets the first day of the shades apearance to the current day
	GameDay = Utility.GetCurrentGameTime() As Int
	CurrentDay = GameDay
	Halo = True
	RegisterForSingleUpdate(60.0)

	;Add the MassReanimate perk for the player OnInit
	if (PlayerRef.HasPerk(NecroMassReanimatePerk) == 0)
		PlayerRef.addperk(NecroMassReanimatePerk)
	else
	endif

EndEvent

Event OnUpdate()

GameDay = Utility.GetCurrentGameTime() As Int
;debug.notification("NOTE: The Gameday is " + GameDay + "current day is" + CurrentDay)

;Add the MassReanimate perk incase the player didn't get it OnInit
If (PlayerRef.HasPerk(NecroMassReanimatePerk) == 0)
	PlayerRef.addperk(NecroMassReanimatePerk)
else
endif

If (PlayerRef.HasPerk(NecroTranscendencePerk) == 0) && (PlayerRef.HasPerk(NecroPhylacteryRegenPerk) == 1)
	int DeathCount = NecroDeathCount.GetValueInt()
	If DeathCount >= 3
		PlayerRef.addperk(NecroTranscendencePerk)
		NecroTranscendenceMsg.Show()
		PlayerRef.addspell(NecroUCLTranscendence, true)
	Endif
Endif

;Disable the shade if the day is passed
If (Halo == True)
	if (CurrentDay != GameDay)
		;disable the shade and start a timer for the next 7 days
		Halo = False
;		debug.notification("UCL: Turning off the fucking beam")
		NecroUCLRevenantMasterBeam01.Disable()
		NecroUCLRevenantMasterBeam02.Disable()
		NecroUCLRevenantMasterBeam03.Disable()
		Daytimer = 0
		GameDay = Utility.GetCurrentGameTime() As Int
		CurrentDay = GameDay
	endif
endif

;If the shade is not up, start it up again in 8 ingame days
if (CurrentDay != GameDay)
	if (DayTimer < 7)
		Daytimer = Daytimer + 1
;		debug.notification("Daytimer =" + Daytimer + "days")
		GameDay = Utility.GetCurrentGameTime() As Int
		CurrentDay = GameDay
	else
		Halo = True
		NecroRevenant_BlackedCount.SetValue(0.0)
		NecroRevenant_ScrollCount.SetValue(0.0)
;		debug.notification("NecroTrackingQuest: Halo is true, daytimer reset")
		NecroUCLRevenantMasterBeam01.Enable()
		NecroUCLRevenantMasterBeam02.Enable()
		NecroUCLRevenantMasterBeam03.Enable()
		Daytimer = 0
		GameDay = Utility.GetCurrentGameTime() As Int
		CurrentDay = GameDay
		If NecroPhylacterySoulCount.GetValueInt() >= 35
			Debug.Notification("Mannimarco's Moon passes over Nirn today.")
		Endif
	endif
endif

RegisterForSingleUpdate(60.0)

EndEvent

function Phylactery01Effects()
	TransformExplosion02.Play(NecroLichRitualXMarkerHeading001)
	TargetVFX.Play(NecroLichTransformationRitualTarget01,8.7,PlayerRef)             
	CasterVFX.Play(PlayerRef,8.9,NecroLichTransformationRitualTarget01)
	TargetFXS.Play(PlayerRef,5)    						
	CasterFXS.Play(NecroLichTransformationRitualTarget01,4)
	IllusionLightMassiveImod.apply() 
endFunction

function Phylactery02Effects()
	TransformExplosion02.Play(NecroLichRitualXMarkerHeading002)
	TargetVFX.Play(NecroLichTransformationRitualTarget02,8.8,PlayerRef)             
	CasterVFX.Play(PlayerRef,8.8,NecroLichTransformationRitualTarget02)
	TargetFXS.Play(PlayerRef,15)    						
	CasterFXS.Play(NecroLichTransformationRitualTarget02,15)
	IllusionLightMassiveImod.apply() 
endFunction

function PhylacteryMovableEffects()
	TransformExplosion02.Play(NecroLichRitualXMarkerMovable01)
	TargetVFX.Play(NecroLichPhylacteryMovable02,8.8,PlayerRef)             
	CasterVFX.Play(PlayerRef,8.8,NecroLichPhylacteryMovable02)
	TargetFXS.Play(PlayerRef,15)    						
	CasterFXS.Play(NecroLichPhylacteryMovable02,15)
	IllusionLightMassiveImod.apply() 
endFunction

function PhylacteryBelethorEffects()
	TransformExplosion02.Play(NecroLichRitualXMarkerMovable01)
	TargetVFX.Play(BelethorRef,8.8,PlayerRef)             
	CasterVFX.Play(PlayerRef,8.8,BelethorRef)
	TargetFXS.Play(PlayerRef,15)    						
	CasterFXS.Play(BelethorRef,15)
	IllusionLightMassiveImod.apply() 
endFunction

function MovePlayerHome(bool bHealPlayer = false)
; 	debug.trace(self + "MovePlayerHome START: bHealPlayer=" + bHealPlayer)
	Actor player = Game.GetPlayer()
	player.GetActorBase().SetInvulnerable(true)
	player.removespell(NecroPerkPhylacteryRegenAbility)


	if bHealPlayer
		; restore health
		player.RestoreActorValue("Health", 9999)
	endif
	
	If (NecroLichPhylacteryMovable02.IsEnabled() == 1)
		player.moveto(NecroLichRitualXMarkerMovable01)
		NecroLichPhylacteryMovableFX.Enable()
		FadeToBlackHoldImod.PopTo(DLC2ApocryphaRewardBookExit)
		FadeToBlackHoldImod.Remove()		
		PhylacteryMovableEffects()
	elseif (PlayerRef.HasPerk(NecroUCLBelethorSlavePerk) == 1)
		player.moveto(NecroLichRitualXMarkerMovable01)
		FadeToBlackHoldImod.PopTo(DLC2ApocryphaRewardBookExit)
		FadeToBlackHoldImod.Remove()
		PhylacteryBelethorEffects()
	elseif (NecroLichTransformationRitualTarget01.IsEnabled() == 1)
		player.moveto(NecroLichRitualXMarkerHeading001)
		NecroLichTransformationPhylacteryFX001.Enable()
		FadeToBlackHoldImod.PopTo(DLC2ApocryphaRewardBookExit)
		FadeToBlackHoldImod.Remove()
		Phylactery01Effects()
	elseIf (NecroLichTransformationRitualTarget02.IsEnabled() == 1)
		player.moveto(NecroLichRitualXMarkerHeading002)
		NecroLichTransformationPhylacteryFX002.Enable()
		FadeToBlackHoldImod.PopTo(DLC2ApocryphaRewardBookExit)
		FadeToBlackHoldImod.Remove()
		Phylactery02Effects()
	endif
	
	If (player.GetRace() != LichRace)
	   player.UnequipAll()
	   player.RemoveSpell(NecroUCLEtherealAbility)
	Else
		If NecroLichColorValue.GetValueInt() == 5
			player.AddSpell(NecroUCLEtherealAbility, false)
		else
			player.RemoveSpell(NecroUCLEtherealAbility)
		EndIf	
		
		if NecroLichModelValue.GetValueInt() == 3
			;give player dragon priest skin
			player.EquipItem(NecroLichSkinDragonPriest,  False, True)		
		elseif NecroLichModelValue.GetValueInt() == 4
			;give player ayleid skin
			player.EquipItem(NecroLichSkinAyleid,  False, True)		
		elseif NecroLichModelValue.GetValueInt() == 5
			;give player underking skin
			player.EquipItem(NecroLichSkinUnderking,  False, True)			
		elseif NecroLichModelValue.GetValueInt() == 1
		; embalmed essence
			;Hubris Of Man
			if NecroLichRaceValue.GetValueInt() == 0
				if NecroLichColorValue.GetValueInt() == 1
					;transform the player into the red flesh lich
					player.EquipItem(NecroLichSkin01,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 2
					;transform the player into the green flesh lich
					player.EquipItem(NecroLichSkin02,  False, True)	
				elseif NecroLichColorValue.GetValueInt() == 3
					;transform the player into the black flesh lich
					player.EquipItem(NecroLichSkin09,  False, True)	
				elseif NecroLichColorValue.GetValueInt() == 4		
					;transform the player into the white flesh lich
					player.EquipItem(NecroLichSkinWhiteFlesh,  False, True)
				else 
					;lich has no color, likely ethereal. equip black
					player.EquipItem(NecroLichSkin09,  False, True)	
				endif			
			;Elven Worm
			elseif NecroLichRaceValue.GetValueInt() == 1
				if NecroLichColorValue.GetValueInt() == 1
					;transform the player into the red flesh lich (elf)
					player.EquipItem(NecroLichSkinRedFlesh_Elf,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 2
					;transform the player into the green flesh lich (elf)
					player.EquipItem(NecroLichSkinGreenFlesh_Elf,  False, True)	
				elseif NecroLichColorValue.GetValueInt() == 3
					;transform the player into the black flesh lich (elf)
					player.EquipItem(NecroLichSkinBlackFlesh_Elf,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 4
					;transform the player into the white flesh lich (elf)
					player.EquipItem(NecroLichSkinWhiteFlesh_Elf,  False, True)		
				else 
					;player has no color, likely ethereal. pick black
					player.EquipItem(NecroLichSkinBlackFlesh_Elf,  False, True)
				endif		
			;Orc Warlock
			elseif NecroLichRaceValue.GetValueInt() == 2
				if NecroLichColorValue.GetValueInt() == 1
					;transform the player into the red flesh lich (orc)
					player.EquipItem(NecroLichSkinRedFlesh_Orc,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 2
					;transform the player into the green flesh lich (orc)
					player.EquipItem(NecroLichSkinGreenFlesh_Orc,  False, True)	
				elseif NecroLichColorValue.GetValueInt() == 3
					;transform the player into the black flesh lich (orc)
					player.EquipItem(NecroLichSkinBlackFlesh_Orc,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 4
					;transform the player into the white flesh lich (orc)
					player.EquipItem(NecroLichSkinWhiteFlesh_Orc,  False, True)	
				else 
					;player has no color, likely ethereal. pick black
					player.EquipItem(NecroLichSkinBlackFlesh_Orc,  False, True)				
				endif		
			;Reptilian Fossil	
			elseif NecroLichRaceValue.GetValueInt() == 3
				if NecroLichColorValue.GetValueInt() == 1
					;transform the player into the red flesh lich (lizard)
					player.EquipItem(NecroLichSkinRedFlesh_Argonian,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 2
					;transform the player into the green flesh lich (lizard)
					player.EquipItem(NecroLichSkinGreenFlesh_Argonian,  False, True)	
				elseif NecroLichColorValue.GetValueInt() == 3
					;transform the player into the black flesh lich (lizard)
					player.EquipItem(NecroLichSkinBlackFlesh_Argonian,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 4
					;transform the player into the white flesh lich (lizard)
					player.EquipItem(NecroLichSkinWhiteFlesh_Argonian,  False, True)	
				else 
					;player has no color, likely ethereal. pick black
					player.EquipItem(NecroLichSkinBlackFlesh_Argonian,  False, True)						
				endif	
			;Arum'Khal's Empire
			elseif NecroLichRaceValue.GetValueInt() == 4
				if NecroLichColorValue.GetValueInt() == 1
					;transform the player into the red flesh lich (cat)
					player.EquipItem(NecroLichSkinRedFlesh_Khajiit,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 2
					;transform the player into the green flesh lich (cat)
					player.EquipItem(NecroLichSkinGreenFlesh_Khajiit,  False, True)	
				elseif NecroLichColorValue.GetValueInt() == 3
					;transform the player into the black flesh lich (cat)
					player.EquipItem(NecroLichSkinBlackFlesh_Khajiit,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 4
					;transform the player into the white flesh lich (cat)
					player.EquipItem(NecroLichSkinWhiteFlesh_Khajiit,  False, True)	
				else 
					;player has no color, likely ethereal. pick black
					player.EquipItem(NecroLichSkinBlackFlesh_Khajiit,  False, True)				
				endif	
			endif			
		elseif NecroLichModelValue.GetValueInt() == 2
		; corroded bone
			;Hubris Of Man
			if NecroLichRaceValue.GetValueInt() == 0
				if NecroLichColorValue.GetValueInt() == 1
					;transform the player into the red skeletal lich
					player.EquipItem(NecroLichSkin05,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 2
					;transform the player into the green skeletal lich
					player.EquipItem(NecroLichSkin06,  False, True)	
				elseif NecroLichColorValue.GetValueInt() == 3
					;transform the player into the black skeletal lich
					player.EquipItem(NecroLichSkin11,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 4
					;transform the player into the white skeletal lich
					player.EquipItem(NecroLichSkinWhiteSkel,  False, True)	
				else 
					;player has no color, likely ethereal. pick black
					player.EquipItem(NecroLichSkin11,  False, True)
				endif
			;Elven Worm
			elseif NecroLichRaceValue.GetValueInt() == 1
				if NecroLichColorValue.GetValueInt() == 1
					;transform the player into the red skeletal lich (elf)
					player.EquipItem(NecroLichSkinRedSkel_Elf,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 2
					;transform the player into the green skeletal lich (elf)
					player.EquipItem(NecroLichSkinGreenSkel_Elf,  False, True)	
				elseif NecroLichColorValue.GetValueInt() == 3
					;transform the player into the black skeletal lich (elf)
					player.EquipItem(NecroLichSkinBlackSkel_Elf,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 4
					;transform the player into the white skeletal lich (elf)
					player.EquipItem(NecroLichSkinWhiteSkel_Elf,  False, True)		
				else 
					;player has no color, likely ethereal. pick black
					player.EquipItem(NecroLichSkinBlackSkel_Elf,  False, True)
				endif		
			;Orc Warlock
			elseif NecroLichRaceValue.GetValueInt() == 2
				if NecroLichColorValue.GetValueInt() == 1
					;transform the player into the red skeletal lich (orc)
					player.EquipItem(NecroLichSkinRedSkel_Orc,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 2
					;transform the player into the green skeletal lich (orc)
					player.EquipItem(NecroLichSkinGreenSkel_Orc,  False, True)	
				elseif NecroLichColorValue.GetValueInt() == 3
					;transform the player into the black skeletal lich (orc)
					player.EquipItem(NecroLichSkinBlackSkel_Orc,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 4
					;transform the player into the white skeletal lich (orc)
					player.EquipItem(NecroLichSkinWhiteSkel_Orc,  False, True)	
				else 
					;player has no color, likely ethereal. pick black
					player.EquipItem(NecroLichSkinBlackSkel_Orc,  False, True)				
				endif		
			;Reptilian Fossil	
			elseif NecroLichRaceValue.GetValueInt() == 3
				if NecroLichColorValue.GetValueInt() == 1
					;transform the player into the red skeletal lich (lizard)
					player.EquipItem(NecroLichSkinRedSkel_Argonian,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 2
					;transform the player into the green skeletal lich (lizard)
					player.EquipItem(NecroLichSkinGreenSkel_Argonian,  False, True)	
				elseif NecroLichColorValue.GetValueInt() == 3
					;transform the player into the black skeletal lich (lizard)
					player.EquipItem(NecroLichSkinBlackSkel_Argonian,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 4
					;transform the player into the white skeletal lich (lizard)
					player.EquipItem(NecroLichSkinWhiteSkel_Argonian,  False, True)	
				else 
					;player has no color, likely ethereal. pick black
					player.EquipItem(NecroLichSkinBlackSkel_Argonian,  False, True)						
				endif	
			;Arum'Khal's Empire
			elseif NecroLichRaceValue.GetValueInt() == 4
				if NecroLichColorValue.GetValueInt() == 1
					;transform the player into the red skeletal lich (cat)
					player.EquipItem(NecroLichSkinRedSkel_Khajiit,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 2
					;transform the player into the green skeletal lich (cat)
					player.EquipItem(NecroLichSkinGreenSkel_Khajiit,  False, True)	
				elseif NecroLichColorValue.GetValueInt() == 3
					;transform the player into the black skeletal lich (cat)
					player.EquipItem(NecroLichSkinBlackSkel_Khajiit,  False, True)
				elseif NecroLichColorValue.GetValueInt() == 4
					;transform the player into the white skeletal lich (cat)
					player.EquipItem(NecroLichSkinWhiteSkel_Khajiit,  False, True)	
				else 
					;player has no color, likely ethereal. pick black
					player.EquipItem(NecroLichSkinBlackSkel_Khajiit,  False, True)				
				endif	
			endif	
		;lich has no skin, likely due to adding transform by console. Give naked skin until player changes skin at phylactery.
		else
			player.EquipItem(NecroLichSkin11,  False, True)
		endif		
		
    Endif
	

	FXfallingSandDragonPriestEffect.Stop(Game.GetPlayer())
	
	;player.MoveTo(TamrielMarker)
	; clear from alias - make sure healed first as a failsafe for water damage
	if player.GetActorValue("Health") < 0
		; restore health
		player.RestoreActorValue("Health", 9999)
	endif
	
	If player.hasperk(NecroUCLBelethorSlavePerk)
		NecroUCLBelethorReformMsg.Show()
	else
		NecroUCLPhylacteryReformMsg.Show()
	endif
	
	PlayerAlias.Clear()
	If (Game.GetPlayer().GetRace() == LichRace)
	  player.equipspell(NecroUCLIceCoffin, 0)
	  player.equipspell(NecroDeathGrip, 1)
	  player.equipspell(NecroRevert, 2)
	Endif
	player.GetActorBase().SetInvulnerable(false)
	player.AddSpell(NecroPerkPhylacteryRegenAbility, false)
	NecroDeathCount.Mod(1.0)
	Game.EnablePlayerControls()
	
	If (bDoomedWorld == true)
		NecroUCLSovngardeDeathMsg.Show()
	Endif
	
	utility.wait(3.0)
	player.RemoveSpell(NecroPhylacteryEtherealizeSpell)
	EndEtherealizeEffects()
	
endFunction

Function PlayerInBleedout()

; ;	enter bleedout 

	If (PlayerRef.GetWorldSpace() == Sovngarde)
		bDoomedWorld = true
	Else	
		bDoomedWorld = false
	Endif

	;UCL: player cannot revive while phylactery is not tethered; handle dying from bleedout.
	If (PlayerRef.HasPerk(NecroUCLDisableRespawnPerk) == 1)

		Game.DisablePlayerControls(abMovement = true, abFighting = true, abCamSwitch = true, abLooking = false, abSneaking = true, abMenu = true, abActivate = true, abJournalTabs = true)	
		PlayerRef.SetGhost(true)
		PlayerRef.UnequipAll()	
		TransformExplosion02.Play(PlayerRef)
		PlayerRef.placeAtMe(FXDragonPriestLargeExplosion, 1)
		NecroUCLPhylacteryShatterMsg.Show()
		Utility.Wait(1.0)
		PlayerAlias.Clear()
		PlayerRef.DamageAV("Health", 9999)
		return
	Endif
	If (PlayerRef.HasPerk(NecroPhylacteryRegenPerk) == 1) && (PlayerRef.GetRace() == LichRace)

		Game.DisablePlayerControls(abMovement = true, abFighting = true, abCamSwitch = true, abLooking = false, abSneaking = true, abMenu = true, abActivate = true, abJournalTabs = true)
		PlayerRef.GetActorBase().SetInvulnerable(true)
		PlayerRef.SetGhost(true)
		Game.SetPlayerAIDriven(true)

		PlayerRef.placeAtMe(FXDragonPriestLargeExplosion, 1)
		NecroPhylacteryEtherealizeSpell.Cast(PlayerRef, PlayerRef)
		
		PlayerRef.pushActorAway(PlayerRef, 50)
		BeginEtherealizeEffects()
		UCLStopEyeFX(PlayerRef)
		UCLHandleEyeFX(PlayerRef)
		

		utility.wait(1.0)
		TransitionFX.play(PlayerRef)
		DLC2ApocryphaRewardBookEnter.Apply()
		DLC2ApocryphaRewardBookEnter.PopTo(FadeToBlackHoldImod)
		; move player home
		(NecroTrackingQuest as NecroTrackingQuestSCRIPT).MovePlayerHome(true)
		TransitionFX.stop(PlayerRef)
		PlayerRef.GetActorBase().SetInvulnerable(false)
		PlayerRef.SetGhost(false)
		Game.SetPlayerAIDriven(false)	
		bInBleedout = false
	Endif
	If (PlayerRef.HasPerk(NecroPhylacteryRegenPerk) == 1) && (PlayerRef.GetRace() != LichRace)

        Game.DisablePlayerControls(abMovement = true, abFighting = true, abCamSwitch = true, abLooking = false, abSneaking = true, abMenu = true, abActivate = true, abJournalTabs = true)
		PlayerRef.GetActorBase().SetInvulnerable(true)
		PlayerRef.SetGhost(true)
		Game.SetPlayerAIDriven(true)
		
		PlayerRef.placeAtMe(FXDragonPriestLargeExplosion, 1)
        NecroPhylacteryEtherealizeSpell.Cast(PlayerRef, PlayerRef)

		BeginEtherealizeEffects()
		

		utility.wait(1.0)
		TransitionFX.play(PlayerRef)
		DLC2ApocryphaRewardBookEnter.Apply()
		utility.wait(1.0)
		DLC2ApocryphaRewardBookEnter.PopTo(FadeToBlackHoldImod)
		; move player home
		MovePlayerHome(true)

		TransitionFX.stop(PlayerRef)
		PlayerRef.GetActorBase().SetInvulnerable(false)
		PlayerRef.SetGhost(false)
		Game.SetPlayerAIDriven(false)	
		PlayerRef.equipspell(LichTransform, 2)
		bInBleedout = false
	endif	

endFunction

Function BeginEtherealizeEffects()

	introFX.Apply()
	Utility.Wait(fDelay)
	IntroFX.PopTo(LoopFX)
	;PSGD.apply(FadeInTime)
	EtherealizeShader01.play(Game.GetPlayer())
	EtherealizeShader02.play(Game.GetPlayer())
	EtherealizeVFX.play(Game.GetPlayer())

endFunction

Function EndEtherealizeEffects()

	EtherealizeShader01.Stop(Game.GetPlayer())
	EtherealizeShader02.Stop(Game.GetPlayer())
	EtherealizeVFX.Stop(Game.GetPlayer())
	LoopFX.PopTo(OutroFX,fImodStrength)
	;PSGD.Remove(1.0)

endFunction


Function LichfootCheck()
;	debug.notification("UCL checking lichfoot")
    if (Game.GetPlayer().HasPerk(Lightfoot) == true)
        DLC1HasLightfoot = true
    else
        DLC1HasLightfoot = false
    endif
endFunction

Function UCLHandleEyeFX(actor akActor)
	int EyeColor = NecroLichEyeFXValue.GetValueInt()
	int RaceValue = NecroLichRaceValue.GetValueInt()
	int ModelValue = NecroLichModelValue.GetValueInt()
	
	; beast liches require unique eye offset meshes
	
	if ModelValue != 1 && ModelValue != 2
		if EyeColor == 1
			FXNecroUCLParticlesPurple.play(akActor, -1)
			FXNecroUCLEyeGlowPurple.play(akActor, -1)
		elseif EyeColor == 2
			FXNecroUCLParticlesBlue.play(akActor, -1)
			FXNecroUCLEyeGlowBlue.play(akActor, -1)	
		elseif EyeColor == 3
			FXNecroUCLParticlesRed.play(akActor, -1)
			FXNecroUCLEyeGlowRed.play(akActor, -1)	
		elseif EyeColor == 4
			FXNecroUCLParticlesGreen.play(akActor, -1)
			FXNecroUCLEyeGlowGreen.play(akActor, -1)	
		endif		
	elseif RaceValue == 3 
		if EyeColor == 1
			FXNecroUCLParticlesPurple.play(akActor, -1)
			FXNecroUCLEyeGlowPurple_LizardFlesh.play(akActor, -1)
		elseif EyeColor == 2
			FXNecroUCLParticlesBlue.play(akActor, -1)
			FXNecroUCLEyeGlowBlue_LizardFlesh.play(akActor, -1)	
		elseif EyeColor == 3
			FXNecroUCLParticlesRed.play(akActor, -1)
			FXNecroUCLEyeGlowRed_LizardFlesh.play(akActor, -1)	
		elseif EyeColor == 4
			FXNecroUCLParticlesGreen.play(akActor, -1)
			FXNecroUCLEyeGlowGreen_LizardFlesh.play(akActor, -1)	
		endif
	elseif RaceValue == 4 
		if EyeColor == 1
			FXNecroUCLParticlesPurple.play(akActor, -1)
			FXNecroUCLEyeGlowPurple_CatFlesh.play(akActor, -1)
		elseif EyeColor == 2
			FXNecroUCLParticlesBlue.play(akActor, -1)
			FXNecroUCLEyeGlowBlue_CatFlesh.play(akActor, -1)	
		elseif EyeColor == 3
			FXNecroUCLParticlesRed.play(akActor, -1)
			FXNecroUCLEyeGlowRed_CatFlesh.play(akActor, -1)	
		elseif EyeColor == 4
			FXNecroUCLParticlesGreen.play(akActor, -1)
			FXNecroUCLEyeGlowGreen_CatFlesh.play(akActor, -1)	
		endif
	else 
		if EyeColor == 1
			FXNecroUCLParticlesPurple.play(akActor, -1)
			FXNecroUCLEyeGlowPurple.play(akActor, -1)
		elseif EyeColor == 2
			FXNecroUCLParticlesBlue.play(akActor, -1)
			FXNecroUCLEyeGlowBlue.play(akActor, -1)	
		elseif EyeColor == 3
			FXNecroUCLParticlesRed.play(akActor, -1)
			FXNecroUCLEyeGlowRed.play(akActor, -1)	
		elseif EyeColor == 4
			FXNecroUCLParticlesGreen.play(akActor, -1)
			FXNecroUCLEyeGlowGreen.play(akActor, -1)	
		endif		
	endif
EndFunction

Function UCLStopEyeFX(actor akActor)
		DragonPriestParticlesFX.stop(akActor)
		DragonPriestEyeGlowFX.stop(akActor)
		FXNecroUCLParticlesPurple.stop(akActor)
		FXNecroUCLEyeGlowPurple.stop(akActor)
		FXNecroUCLParticlesBlue.stop(akActor)
		FXNecroUCLEyeGlowBlue.stop(akActor)
		FXNecroUCLParticlesRed.stop(akActor)
		FXNecroUCLEyeGlowRed.stop(akActor)
		FXNecroUCLParticlesGreen.stop(akActor)
		FXNecroUCLEyeGlowGreen.stop(akActor)
		FXNecroUCLEyeGlowPurple_LizardFlesh.stop(akActor)
		FXNecroUCLEyeGlowPurple_CatFlesh.stop(akActor)
		FXNecroUCLEyeGlowBlue_LizardFlesh.stop(akActor)
		FXNecroUCLEyeGlowBlue_CatFlesh.stop(akActor)
		FXNecroUCLEyeGlowRed_LizardFlesh.stop(akActor)
		FXNecroUCLEyeGlowRed_CatFlesh.stop(akActor)
		FXNecroUCLEyeGlowGreen_LizardFlesh.stop(akActor)
		FXNecroUCLEyeGlowGreen_CatFlesh.stop(akActor)
EndFunction

Bool Property DLC1HasLightfoot  Auto

ImageSpaceModifier property EtherealizeImod auto
Effectshader property EtherealizeShader01 auto
Effectshader property EtherealizeShader02 auto
VisualEffect property EtherealizeVFX auto

float Property fdelay  Auto  

ImageSpaceModifier Property IntroFX  Auto  
ImageSpaceModifier Property LoopFX  Auto  
ImageSpaceModifier Property OutroFX  Auto  

float Property FadeInTime  Auto  

ShaderParticleGeometry Property PSGD  Auto  
float Property fimodstrength  Auto  

Quest Property NecroLichRitualQuest auto

ObjectReference Property NecroLichRitualXMarkerHeading001 auto
ObjectReference Property NecroLichRitualXMarkerHeading002 auto

ObjectReference Property NecroLichTransformationRitualTarget01 auto
ObjectReference Property NecroLichTransformationRitualTarget02 auto
ObjectReference Property NecroLichTransformationPhylacteryFX001 auto
ObjectReference Property NecroLichTransformationPhylacteryFX002 auto

ReferenceAlias Property PlayerAlias auto

ImageSpaceModifier Property FadeToBlackHoldImod  Auto  
ImageSpaceModifier property IllusionLightMassiveImod auto
ImageSpaceModifier Property DLC2ApocryphaRewardBookEnter  Auto  
ImageSpaceModifier Property DLC2ApocryphaRewardBookExit  Auto  

VisualEffect Property FxfallingSandDragonPriestEffect auto
VisualEffect property TargetVFX auto
VisualEffect property CasterVFX auto

EffectShader Property NecroLichTransformPlayerFireFXShort auto
EffectShader property CasterFXS auto
EffectShader property TargetFXS auto

Armor Property SkinSkeletonNecroPriest auto 
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

Keyword Property ActorTypeUndead auto 

float Property PercentHealth = 100.0 Auto  

Quest Property NecroTrackingQuest Auto

bool bInBleedout

Explosion Property FXDragonPriestLargeExplosion auto

EffectShader Property DLC1VampMistFormFX auto
EffectShader Property TransitionFX auto
Effectshader Property Transition02FX auto

Spell Property NecroPhylacteryEtherealizeSpell auto
Spell Property DLC1VampireChangeStagger auto
Spell Property NecroPerkPhylacteryRegenAbility auto
Spell Property NecroAreaSoulTrap auto
Spell Property NecroRevert auto
Spell Property NecroDeathGrip auto
Spell Property LichTransform auto
Spell Property NecroUCLEtherealAbility Auto
Spell Property NecroUCLTranscendence Auto
Spell Property NecroUCLIceCoffin auto

Perk Property NecroPhylacteryRegenPerk auto
Perk Property LightFoot Auto
Perk Property NecroMassReanimatePerk Auto
Perk Property NecroTranscendencePerk Auto

SoulGem Property NecroSoulGemBlack Auto
SoulGem Property NecroSoulGemBlackFilled Auto

Race Property LichRace auto

GlobalVariable Property NecroLichColorValue Auto
GlobalVariable Property NecroLichModelValue Auto
GlobalVariable Property NecroLichRaceValue Auto
GlobalVariable Property NecroLichEyeFXValue Auto
GlobalVariable Property NecroRevenant_BlackedCount Auto
GlobalVariable Property NecroRevenant_ScrollCount Auto
GlobalVariable Property NecroPhylacterySoulCount Auto
GlobalVariable Property NecroDeathCount Auto

Actor Property PlayerRef Auto

ObjectReference Property NecroUCLRevenantMasterBeam01 auto
ObjectReference Property NecroUCLRevenantMasterBeam02 auto
ObjectReference Property NecroUCLRevenantMasterBeam03 auto

Weapon Property Staff auto

Message Property NecroTranscendenceMsg Auto
Message Property NecroUCLPhylacteryShatterMsg Auto
Message Property NecroUCLPhylacteryReformMsg Auto
Message Property NecroUCLSovngardeDeathMsg Auto
Message Property NecroUCLBelethorReformMsg Auto

Sound Property TransformExplosion02  Auto 

Perk Property NecroUCLDisableRespawnPerk Auto
Perk Property NecroUCLBelethorSlavePerk Auto
ObjectReference Property NecroLichPhylacteryMovable01 auto
ObjectReference Property NecroLichPhylacteryMovable02 auto
ObjectReference Property NecroLichPhylacteryMovableFX auto
ObjectReference Property NecroLichRitualXMarkerMovable01 auto
ObjectReference Property BelethorRef Auto

Worldspace Property Sovngarde Auto
bool bDoomedWorld = false

VisualEffect Property DragonPriestParticlesFX Auto
VisualEffect Property FXNecroUCLParticlesPurple Auto
VisualEffect Property FXNecroUCLParticlesBlue Auto
VisualEffect Property FXNecroUCLParticlesRed Auto
VisualEffect Property FXNecroUCLParticlesGreen Auto
{Floor based stream of upward particles}

VisualEffect Property DragonPriestEyeGlowFX Auto
VisualEffect Property FXNecroUCLEyeGlowPurple Auto
VisualEffect Property FXNecroUCLEyeGlowBlue Auto
VisualEffect Property FXNecroUCLEyeGlowRed Auto
VisualEffect Property FXNecroUCLEyeGlowGreen Auto
{eye glow fx}

VisualEffect Property FXNecroUCLEyeGlowPurple_LizardFlesh Auto
VisualEffect Property FXNecroUCLEyeGlowPurple_CatFlesh Auto
VisualEffect Property FXNecroUCLEyeGlowBlue_LizardFlesh Auto
VisualEffect Property FXNecroUCLEyeGlowBlue_CatFlesh Auto
VisualEffect Property FXNecroUCLEyeGlowRed_LizardFlesh Auto
VisualEffect Property FXNecroUCLEyeGlowRed_CatFlesh Auto
VisualEffect Property FXNecroUCLEyeGlowGreen_LizardFlesh Auto
VisualEffect Property FXNecroUCLEyeGlowGreen_CatFlesh Auto
{beast eye glow fx}