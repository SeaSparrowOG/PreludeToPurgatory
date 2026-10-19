;BEGIN FRAGMENT CODE - Do not edit anything between this and the end comment
;NEXT FRAGMENT INDEX 10
Scriptname PTP_QuestFrgmtMidnightManager Extends Quest Hidden

;BEGIN ALIAS PROPERTY PlayerAlias
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_PlayerAlias Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY DragolichAlias
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_DragolichAlias Auto
;END ALIAS PROPERTY

;BEGIN FRAGMENT Fragment_6
Function Fragment_6()
;BEGIN CODE
Alias_DragolichAlias.GetReference().Disable(true)
Alias_DragolichAlias.GetReference().Delete()
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_8
Function Fragment_8()
;BEGIN CODE
ObjectReference Maiden = Alias_DragolichAlias.GetReference()
ObjectReference Player = Alias_PlayerAlias.GetReference()

If (Player.GetDistance(Maiden) < 200.0)

    Stop()
Else

    Alias_DragolichAlias.GetReference().MoveTo(Alias_PlayerAlias.GetReference())
EndIf
;END CODE
EndFunction
;END FRAGMENT

;END FRAGMENT CODE - Do not edit anything between this and the begin comment
