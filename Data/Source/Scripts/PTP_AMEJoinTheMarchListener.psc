Scriptname PTP_AMEJoinTheMarchListener extends activemagiceffect  

Spell Property PTP_Necromancer_SPL_MinionResurrectSpell Auto

Actor vileNecromancer 
Actor kVictim 

Event OnEffectStart(Actor a_kTarget, Actor a_kCaster)

    vileNecromancer = a_kCaster
    kVictim = a_kTarget
EndEvent

Event OnDeath(Actor a_kKiller)

    PTP_Necromancer_SPL_MinionResurrectSpell.Cast(vileNecromancer, kVictim)
EndEvent