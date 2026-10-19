Scriptname PTP_AMEIceTrail extends activemagiceffect  

Hazard Property PTP_Necromancer_HZD_IceTrailHazard Auto

Actor kFrozenZombie

Event OnEffectStart(Actor a_kTarget, Actor a_kCaster)

    kFrozenZombie = a_kTarget
    RegisterForSingleUpdate(0.1)
EndEvent

Event OnEffectFinish(Actor a_kTarget, Actor a_kCaster)

    UnRegisterForUpdate()
EndEvent

Event OnUpdate()

    ObjectReference iceWall = kFrozenZombie.PlaceAtMe(PTP_Necromancer_HZD_IceTrailHazard)
    iceWall.SetActorCause(kFrozenZombie)
    iceWall.SetScale(0.70)
    RegisterForSingleUpdate(0.6)
EndEvent