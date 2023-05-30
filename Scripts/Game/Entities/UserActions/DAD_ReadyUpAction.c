class DAD_ReadyUpAction : ScriptedUserAction {
	
	protected bool ready = false;

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		
		this.ready = !this.ready;
		SCR_ChimeraCharacter char = SCR_ChimeraCharacter.Cast(pUserEntity);
		FactionKey fac = char.GetFactionKey();
		
		if (this.ready) {
			pOwnerEntity.SetYawPitchRoll("0 0 180");

			SCR_HintManagerComponent.ShowCustomHint(string.Format("%1 Is Ready!", fac));
		} else {
			pOwnerEntity.SetYawPitchRoll("0 0 0");
			SCR_HintManagerComponent.ShowCustomHint(string.Format("%1 Is Ready!", fac));
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetActionNameScript(out string outName)
	{	
		string readyText;
		if (this.ready) {
			readyText = "Not Ready";
		} else {
			readyText = "Ready";
		}
		outName = (string.Format("Set Team as %1", readyText));
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}
	
	override bool CanBroadcastScript()
	{		
		return false;
	}
	
}
