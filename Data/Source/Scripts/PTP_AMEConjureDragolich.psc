Scriptname PTP_AMEConjureDragolich extends ActiveMagicEffect  

Quest Property PTP_Powers_QST_DragolichFollowerQuest Auto

Event OnEffectStart(Actor akTarget, Actor akCaster)

	If (PTP_Powers_QST_DragolichFollowerQuest.IsRunning())

        PTP_Powers_QST_DragolichFollowerQuest.SetStage(50)
        PTP_Powers_QST_DragolichFollowerQuest.SetStage(25)
    Else

        PTP_Powers_QST_DragolichFollowerQuest.Start()
    EndIf
EndEvent