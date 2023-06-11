modded class SCR_AIGroup
{
	override void OnAgentAdded(AIAgent child)
	{
		super.OnAgentAdded(child);
		DAD_AIGroupProxyInfoComponent info = DAD_AIGroupProxyInfoComponent.Cast(child.FindComponent(DAD_AIGroupProxyInfoComponent));
		if (info) info.UpdateInfo(this);
	}
};