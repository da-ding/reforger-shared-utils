[EntityEditorProps(category: "GameScripted/GameMode", description: "Spawn point entity", visible: false)]
class DAD_GroupSpawnPointClass : DAD_EntitySpawnPointClass
{
};

class DAD_GroupSpawnPoint : DAD_EntitySpawnPoint
{
	void SetGroup(SCR_AIGroup group)
	{
		SCR_AIGroup oldGroup = SCR_AIGroup.Cast(GetTargetEntity());
		if (oldGroup) oldGroup.GetOnEmpty().Remove(OnGroupEmpty);

		SetEntity(group.GetLeaderEntity());
		group.GetOnEmpty().Insert(OnGroupEmpty);
	}

	SCR_AIGroup GetGroup()
	{
		return SCR_AIGroup.Cast(m_TargetEntity);
	}

	void OnGroupEmpty()
	{
		OnEntityDeleted(m_TargetEntity);
	}

	override void OnFinalizeSpawnDone_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnData data, IEntity entity)
	{
		super.OnFinalizeSpawnDone_S(requestComponent, data, entity);
		SCR_AIGroup group = SCR_AIGroup.Cast(GetTargetEntity());
		if (!group) return;

		// Delete a member from the group
		array<AIAgent> agents = new array<AIAgent>();
		group.GetAgents(agents);
		int last = agents.Count() - 1;
		IEntity ai = agents.Get(last).GetControlledEntity();

		SCR_EntityHelper.DeleteEntityAndChildren(ai);
		group.RemoveAIEntityFromGroup(ai);

		// TODO: How do I get the player ID out of the IEntity?
		// Oh yeah, also, adding players to the group means that players might get yeeted
		// if someone spawns to a group that already has players!
		// group.AddPlayer(entity);
	}
};
