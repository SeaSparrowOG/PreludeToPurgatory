Scriptname PTP_TranscendenceProc Extends ActiveMagicEffect

Formlist Property CrimeFactions auto

Event OnEffectStart(Actor akTarget, Actor akCaster)

    Int pos = CrimeFactions.GetSize();
    While (pos > 0) 

        pos -= 1
        (CrimeFactions.GetAt(pos) as Faction).SetPlayerEnemy(False)
    EndWhile
EndEvent