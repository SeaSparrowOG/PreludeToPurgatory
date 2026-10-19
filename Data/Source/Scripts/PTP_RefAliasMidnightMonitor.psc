Scriptname PTP_RefAliasMidnightMonitor extends ReferenceAlias  

Event OnDying(Actor a_kKiller)

	Self.GetOwningQuest().Stop()
EndEvent