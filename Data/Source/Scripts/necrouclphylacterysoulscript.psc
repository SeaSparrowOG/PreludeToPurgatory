Scriptname NecroUCLPhylacterySoulScript extends ObjectReference  

int iSoulCount

Event OnActivate(ObjectReference akActionRef)

	if (Game.IsPluginInstalled("ccbgssse067-daedinv.esm")) && (Game.IsPluginInstalled("UndeathFixes - The Cause Patch.esp"))
		NecroUCLAyleidDLCIsPresent.SetValueInt(1)
;		Debug.Notification("The Cause dlc present.")
	else
		NecroUCLAyleidDLCIsPresent.SetValueInt(0)
;		Debug.Notification("The Cause DLC not present!")
	endif
	
	If PlayerRef.HasPerk(NecroPhylacteryRegenPerk)
		float SoulCount
		SoulCount = NecroPhylacterySoulCount.GetValue()
		
		int ibutton = NecroUCLSoulMenu.show(SoulCount)
		
		;SeaSparrow - Removing soul feeding mechanic here.
			if ibutton == 0
				;/
				If SoulCount >= 50.0
					NecroPhylacterySoulCount.SetValue(50.0)		
					Debug.Notification("You have reached the maximum soul capacity.")
					RefreshScalingEffects()
					HandleFrameworkExceptions()
				Else
					If (Game.GetPlayer().GetItemCount(SoulGemBlackFilled) >= 1) 
						Game.GetPlayer().RemoveItem(SoulGemBlackFilled, 1)
						NecroPhylacterySoulCount.Mod(1.0)						
						SoulTrapSoundFX.play(PlayerRef)
						Debug.Notification("Your phylactery absorbs a black soul.")
						RunFeedSoulFrameWork()
						HandleFrameworkExceptions()
						RefreshScalingEffects()
					Else
						Debug.Notification("You do not have any filled black soul gems to feed to your phylactery.")
						HandleFrameworkExceptions()
						RefreshScalingEffects()
					EndIf
				EndIf
				
			elseif ibutton == 1 
				/;
				ShowReconstructMenu()		
				
			elseif ibutton == 1
				;SeaSparrow - These are not needed anymore.
				;HandleFrameworkExceptions()
				;RefreshScalingEffects()
			EndIf
		
		
	EndIf

Endevent

Function ShowReconstructMenu()
	int iButton2 = NecroUCLReconstructMenu.show()
		
		if iButton2 == 0 
			; player chose flesh
			NecroLichModelValue.SetValueInt(1)
			ShowColorMenu()
		elseif iButton2 == 1
			; player chose skeletal
			NecroLichModelValue.SetValueInt(2)
			ShowColorMenu()
		elseif iButton2 == 2
			; player chose dragon priest
			NecroLichModelValue.SetValueInt(3)
			ShowLimitedColorMenu()
		elseif iButton2 == 3 
			; player chose ayleid
			NecroLichModelValue.SetValueInt(4)
			ShowLimitedColorMenu()
		elseif iButton2 == 4
		endif 
		
EndFunction

;SeaSparrow - Original value of 10000000 actually breaks the compiler. 
;Changing to more reasonable value.
Function ShowColorMenu()
	int iButton3 = NecroUCLColorMenu.show()
		
		if iButton3 == 0 
			;player chose red
			NecroLichColorValue.SetValueInt(1)
			PlayerRef.DamageActorValue("Health", 10000)
		elseif iButton3 == 1
			;player chose green
			NecroLichColorValue.SetValueInt(2)
			PlayerRef.DamageActorValue("Health", 10000)
		elseif iButton3 == 2
			;player chose black
			NecroLichColorValue.SetValueInt(3)
			PlayerRef.DamageActorValue("Health", 10000)
		elseif iButton3 == 3 
			;player chose white
			NecroLichColorValue.SetValueInt(4)
			PlayerRef.DamageActorValue("Health", 10000)
		elseif iButton3 == 4
			;player chose ethereal
			NecroLichColorValue.SetValueInt(5)
			PlayerRef.DamageActorValue("Health", 10000)
		elseif iButton3 == 5
		endif 
		
EndFunction

Function ShowLimitedColorMenu()
	int iButton4 = NecroUCLLimitedColorMenu.show()
	
	
		if iButton4 == 0 
			PlayerRef.DamageActorValue("Health", 10000)
			NecroLichColorValue.SetValueInt(3)
		elseif iButton4 == 1
			NecroLichColorValue.SetValueInt(5)
			PlayerRef.DamageActorValue("Health", 10000)
		elseif iButton4 == 2
		endif
EndFunction

Function RunFeedSoulFrameWork()
	iSoulCount = NecroPhylacterySoulCount.GetValueInt()
	if iSoulCount == 1
		NecroPhyMsg1.Show()		
		PlayerRef.AddSpell(NecroUCLManaFlood, true)
		PlayerRef.AddPerk(NecroUClSoulScaling)
	elseif iSoulCount == 2
		NecroPhyMsg2.Show()
		PlayerRef.AddSpell(NecroUCLBarrierSpell, true)
		
	elseif iSoulCount == 3
		NecroPhyMsg3.Show()
		PlayerRef.AddSpell(NecroLichDrainLifeSpell, true)
		
	elseif iSoulCount == 5
		NecroPhyMsg5.Show()
		PlayerRef.AddPerk(NecroUCLHealingPerk)
		
	elseif iSoulCount == 8
		NecroPhyMsg8.Show()
		PlayerRef.AddPerk(NecroUCLConduitPerk)
		
	elseif iSoulCount == 10
		NecroPhyMsg10.Show()
		
	elseif iSoulCount == 12
		NecroPhyMsg12.Show()
		
	elseif iSoulCount == 15
		NecroPhyMsg15.Show()
		
	elseif iSoulCount == 18
		NecroPhyMsg18.Show()
		
	elseif iSoulCount == 20
		NecroPhyMsg20.Show()
		PlayerRef.AddSpell(NecroLichMindFlaySpell, true)
		
	elseif iSoulCount == 22
	
		NecroPhyMsg22.Show()
		PlayerRef.AddSpell(NecroEnslaveMind, true)
		
	elseif iSoulCount == 25
		NecroPhyMsg25.Show()
		PlayerRef.AddPerk(NecroUCLMasqueradePerk)	


		;if player unlocks this perk in lich form, make all crime factions nonhostile
        int cfIndex = 0
        while (cfIndex < CrimeFactions.GetSize())
            ;Debug.Trace("LICH: Removing enemy flag from " + CrimeFactions.GetAt(cfIndex))
            (CrimeFactions.GetAt(cfIndex) as Faction).SetPlayerEnemy(false)
            cfIndex += 1
        endwhile		
		
		Utility.Wait(6.0)
		;runs once when player reaches 25 souls, to tell them they can now customize lich appearance. Yes, i use debug messagebox here cus im a lazy bum.
		Debug.Messagebox("You may now reconstruct the appearance of your physical form. At your phylactery, weave flesh and bone to conjure a visage most befitting of you.") 
		Utility.Wait(1.0)
		Debug.Notification("Activate your phylactery to reconstruct your lich appearance.")

	
	elseif iSoulCount == 30
		NecroPhyMsg30.Show()
		PlayerRef.AddSpell(NecroCommandUndead, true)
		
	elseif iSoulCount == 35
		NecroPhyMsg35.Show()
		PlayerRef.AddPerk(NecroMannimarcoWillPerk)
		
	elseif iSoulCount == 40
		NecroPhyMsg40.Show()
		PlayerRef.AddSpell(NecroMassReanimate, true)
		
	elseif iSoulCount == 45
		NecroPhyMsg45.Show()
		PlayerRef.AddSpell(NecroUCLSkeletalDragonSummon, true)		
		
	elseif iSoulCount == 50
		NecroPhyMsg50.Show()
		PlayerRef.AddSpell(NecroSiphonSoulSpell, true)
	else 	
		NecroPhyMsgBase.Show()		
	endif

endFunction


Function HandleFrameworkExceptions()
;	Debug.Notification("handle exceptions")
	
	iSoulCount = NecroPhylacterySoulCount.GetValueInt()
	if iSoulCount >= 1
		if !PlayerRef.HasSpell(NecroUCLManaFlood)
			PlayerRef.AddSpell(NecroUCLManaFlood, false)
		endif
	endif
	
	if iSoulCount >= 1
		if !PlayerRef.HasPerk(NecroUClSoulScaling)
			PlayerRef.AddPerk(NecroUClSoulScaling)
		endif
	endif	
	
	if iSoulCount >= 2
		if !PlayerRef.HasSpell(NecroUCLBarrierSpell)
			PlayerRef.AddSpell(NecroUCLBarrierSpell, false)
		endif
	endif
	
	if iSoulCount >= 3
		if !PlayerRef.HasSpell(NecroLichDrainLifeSpell)
			PlayerRef.AddSpell(NecroLichDrainLifeSpell, false)
		endif
	endif

	if iSoulCount >= 5
		if !PlayerRef.HasPerk(NecroUCLHealingPerk)
			PlayerRef.AddPerk(NecroUCLHealingPerk)
		endif
	endif

	if iSoulCount >= 8
		if !PlayerRef.HasPerk(NecroUCLConduitPerk)
			PlayerRef.AddPerk(NecroUCLConduitPerk)
		endif
	endif
	
	if iSoulCount >= 20
		if !PlayerRef.HasSpell(NecroLichMindFlaySpell)
			PlayerRef.AddSpell(NecroLichMindFlaySpell, false)
		endif
	endif
	
	if iSoulCount >= 22
		if !PlayerRef.HasSpell(NecroEnslaveMind)
			PlayerRef.AddSpell(NecroEnslaveMind, false)
		endif
	endif

	if iSoulCount >= 25
		if !PlayerRef.HasPerk(NecroUCLMasqueradePerk)
			PlayerRef.AddPerk(NecroUCLMasqueradePerk)

			;if player unlocks this perk in lich form, make all crime factions nonhostile
			int cfIndex = 0
			while (cfIndex < CrimeFactions.GetSize())
				;Debug.Trace("LICH: Removing enemy flag from " + CrimeFactions.GetAt(cfIndex))
				(CrimeFactions.GetAt(cfIndex) as Faction).SetPlayerEnemy(false)
				cfIndex += 1
			endwhile	
		
		endif
	endif	
	
	if iSoulCount >= 30
		if !PlayerRef.HasSpell(NecroCommandUndead)
			PlayerRef.AddSpell(NecroCommandUndead, false)
		endif
	endif	

	if iSoulCount >= 35
		if !PlayerRef.HasPerk(NecroMannimarcoWillPerk)
			PlayerRef.AddPerk(NecroMannimarcoWillPerk)
		endif
	endif	
	
	if iSoulCount >= 40
		if !PlayerRef.HasSpell(NecroMassReanimate)
			PlayerRef.AddSpell(NecroMassReanimate, false)
		endif
	endif	
	
	if iSoulCount >= 45
		if !PlayerRef.HasSpell(NecroUCLSkeletalDragonSummon)
			PlayerRef.AddSpell(NecroUCLSkeletalDragonSummon, false)
		endif
	endif	
	
	if iSoulCount >= 50
		if !PlayerRef.HasSpell(NecroSiphonSoulSpell)
			PlayerRef.AddSpell(NecroSiphonSoulSpell, false)
		endif
	endif
	
endFunction

Function RefreshScalingEffects()
	;update magnitudes
	
	float NewSoulCount
	NewSoulCount = NecroPhylacterySoulCount.GetValue()
	NecroUCLManaFlood.SetNthEffectMagnitude(0, 50 + (4 * NewSoulCount) )
	NecroUCLManaFlood.SetNthEffectMagnitude(1, NewSoulCount / 2 )
	NecroUCLBarrierSpell.SetNthEffectMagnitude(0, NewSoulCount / 2)
	NecroUCLBarrierSpell.SetNthEffectMagnitude(1, 2 * NewSoulCount)
	NecroUCLBarrierSpell.SetNthEffectMagnitude(2, NewSoulCount / 2)
	NecroUClSoulScaling.SetNthEntryValue(0, 0, 1 + (NewSoulCount / 200 ) )
;	NecroUClSoulScaling.SetNthEntryValue(1, 0, 1 + (NewSoulCount / 200 ) )
	
	NecroUClDarkConduit.SetNthEffectMagnitude(0, NewSoulCount)
;	NecroUClDarkConduit.SetNthEffectMagnitude(1, NewSoulCount)
	
	;readd spells so magnitude changes take effect
	If PlayerRef.HasSpell(NecroUCLManaFlood)
		PlayerRef.RemoveSpell(NecroUCLManaFlood)
		PlayerRef.AddSpell(NecroUCLManaFlood, false)
	EndIf
	If PlayerRef.HasSpell(NecroUCLBarrierSpell)
		PlayerRef.RemoveSpell(NecroUCLBarrierSpell)
		PlayerRef.AddSpell(NecroUCLBarrierSpell, false)
	EndIf
	
	If PlayerRef.HasSpell(NecroUClDarkConduit)
		PlayerRef.RemoveSpell(NecroUClDarkConduit)
		PlayerRef.AddSpell(NecroUClDarkConduit, false)
	EndIf	
EndFunction


Spell Property NecroUCLManaFlood Auto
Spell Property NecroUCLBarrierSpell Auto
Spell Property NecroEnslaveMind Auto
Spell Property NecroLichDrainLifeSpell Auto
Spell Property NecroLichMindFlaySpell Auto
Spell Property NecroSiphonSoulSpell Auto
Spell Property NecroMassReanimate Auto
Spell Property NecroUCLSkeletalDragonSummon Auto
Spell Property NecroCommandUndead Auto
Spell Property NecroUClDarkConduit Auto

Perk Property NecroMannimarcoWillPerk Auto
Perk Property NecroPhylacteryRegenPerk Auto
Perk Property NecroUClSoulScaling Auto
Perk Property NecroUCLHealingPerk Auto
Perk Property NecroUCLMasqueradePerk Auto
Perk Property NecroUCLConduitPerk Auto

SoulGem Property SoulGemBlackFilled Auto

Message Property NecroUCLSoulMenu Auto
Message Property NecroPhyMsg1 Auto
Message Property NecroPhyMsg2 Auto
Message Property NecroPhyMsg3 Auto
Message Property NecroPhyMsg5 Auto
Message Property NecroPhyMsg8 Auto	
Message Property NecroPhyMsg10 Auto
Message Property NecroPhyMsg12 Auto
Message Property NecroPhyMsg15 Auto
Message Property NecroPhyMsg18 Auto
Message Property NecroPhyMsg20 Auto
Message Property NecroPhyMsg22 Auto
Message Property NecroPhyMsg25 Auto
Message Property NecroPhyMsg30 Auto
Message Property NecroPhyMsg35 Auto
Message Property NecroPhyMsg40 Auto
Message Property NecroPhyMsg45 Auto
Message Property NecroPhyMsg50 Auto
Message Property NecroPhyMsgBase Auto
Message Property NecroUCLReconstructMenu Auto
Message Property NecroUCLColorMenu Auto
Message Property NecroUCLLimitedColorMenu Auto

GlobalVariable Property NecroPhylacterySoulCount Auto
GlobalVariable Property NecroLichColorValue Auto
GlobalVariable Property NecroLichModelValue Auto
GlobalVariable Property  NecroUCLAyleidDLCIsPresent Auto

Sound Property SoulTrapSoundFX Auto

Actor Property PlayerRef Auto

FormList Property CrimeFactions auto