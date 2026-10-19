Scriptname PTP_AMEToggleLichight extends ActiveMagicEffect  

Spell Property PTP_Powers_SPL_abLichSightProc Auto
Spell Property PTP_Powers_SPL_abLichSightProcEnemy Auto 

Event OnEffectStart(Actor a_kTarget, Actor a_kCaster)
    
    If (a_kTarget.HasSpell(PTP_Powers_SPL_abLichSightProc) || a_kTarget.HasSpell(PTP_Powers_SPL_abLichSightProcEnemy))

        a_kTarget.DispelSpell(PTP_Powers_SPL_abLichSightProc)
        a_kTarget.RemoveSpell(PTP_Powers_SPL_abLichSightProc)
        a_kTarget.DispelSpell(PTP_Powers_SPL_abLichSightProcEnemy)
        a_kTarget.RemoveSpell(PTP_Powers_SPL_abLichSightProcEnemy)
    Else

        a_kTarget.AddSpell(PTP_Powers_SPL_abLichSightProc)
        a_kTarget.AddSpell(PTP_Powers_SPL_abLichSightProcEnemy)
    EndIf
EndEvent