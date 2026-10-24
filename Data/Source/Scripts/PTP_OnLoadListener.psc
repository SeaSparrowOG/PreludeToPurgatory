Scriptname PTP_OnLoadListener extends ReferenceAlias  
{This script listens for the OnPlayerLoadedGame event and then dispatches the maintenance function call to the owning quest.}

Event OnPlayerLoadGame()

	(GetOwningQuest() As PTP_MaintenanceScript).Maintenance()
EndEvent
